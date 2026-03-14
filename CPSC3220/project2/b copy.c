// just here to look pretty
#include "mythreads.h"
#include <stdlib.h>
#include <assert.h>
// empty functions to pass the compile deadline

// the type of function used to run your threads
// typedef void *(*thFuncPtr) (void *);

/*
Rules:
interrupts disabled only inside library
interrupts enabled during user code
disable before swapcontext (especially preemptive mode)
NEVER leave interrupts disabled on return
Wrap every public API:
interruptDisable();
// critical section
interruptEnable();
*/

int interruptsAreDisabled = 0;

static void interruptDisable()
{
    // assert(!interruptsAreDisabled);
    interruptsAreDisabled = 1;
}
static void interruptEnable()
{
    // assert(interruptsAreDisabled);
    interruptsAreDisabled = 0;
}

typedef enum
{
    RUNNING,
    READY,
    BLOCKED,
    FINISHED
} thread_state_t;

typedef struct thread
{
    int id;
    ucontext_t *context; // MUST be heap allocated
    void *stack;         // NULL for main thread
    thread_state_t state;

    void *return_value;

    struct thread *next;        // for queues
    struct thread *all_next;    // for list of all threads in the system, //?? change variable name later
    struct thread *join_waiter; // thread waiting on this one
} thread_t;

// make fifo ready queue to guarantee no starvation, do RR scheduling, so we have deterministic cooperative behavior
static thread_t *current_thread;
static thread_t *ready_queue_head;
static thread_t *ready_queue_tail;
static thread_t *finished_queue; // waiting to be cleaned up
static int next_thread_id;
// list of every thread in the system
static thread_t *all_threads = NULL;

static void enqueue_ready(thread_t *t)
{
    t->next = NULL;

    if (ready_queue_tail == NULL)
    {
        ready_queue_head = t;
        ready_queue_tail = t;
    }
    else
    {
        ready_queue_tail->next = t;
        ready_queue_tail = t;
    }
}

static thread_t *dequeue_ready()
{
    if (ready_queue_head == NULL)
        return NULL;

    thread_t *t = ready_queue_head;
    ready_queue_head = t->next;

    if (ready_queue_head == NULL)
        ready_queue_tail = NULL;

    t->next = NULL;
    return t;
}

static thread_t *find_thread(int id)
{
    thread_t *t = all_threads;

    while (t)
    {
        if (t->id == id)
            return t;

        t = t->all_next;
    }

    return NULL;
}

// Pick the next runnable thread from the ready queue
// Switch context from the current thread to that thread
// After the switch returns, clean up any finished threads
void schedule()
{
    interruptDisable();

    thread_t *prev_thread = current_thread;

    // get the next runnable thread
    thread_t *next_thread = dequeue_ready();

    if (next_thread == NULL)
    {
        // no more threads to run
        interruptEnable();
        exit(0);
    }

    // mark next thread as running
    next_thread->state = RUNNING;
    current_thread = next_thread;

    /*
    swapcontext saves the current thread state into prev_thread->context
    and switches execution to next_thread->context.
    When the previous thread runs again later,
    execution will resume right after this swapcontext call.
    */
    swapcontext(prev_thread->context, next_thread->context);
    /*
    When execution returns here, this thread has been scheduled again.
    Now we can safely clean up finished threads because we are NOT
    running on their stack anymore.
    */

    while (finished_queue)
    {
        thread_t *t = finished_queue;
        finished_queue = finished_queue->next;

        free(t->stack);     // free stack allocated for the thread
        free(t->context);   // free context structure
        free(t);            // free thread control block
    }

    interruptEnable();
}

//////////////////////////////////////////////////////////////////////////////////

extern void threadInit(void)
{
    /*
    Disable interrupts
    initialize global state
    allocate a tcb for the main thread
    allocate its context on the heap
    capture its context with getcontext
    set main thread stack pointer to NULL to indicate it’s the main thread
    initialize metadata
    set as current thread
    enable interrupts
    */

    interruptDisable();
    current_thread = malloc(sizeof(thread_t));
    current_thread->id = 0;
    current_thread->context = malloc(sizeof(ucontext_t));
    getcontext(current_thread->context);
    current_thread->stack = NULL; // indicates main thread
    current_thread->state = RUNNING;
    current_thread->return_value = NULL;
    current_thread->next = NULL;
    current_thread->all_next = all_threads;
    // added this, forgot?
    current_thread->join_waiter = NULL;
    all_threads = current_thread;
    ready_queue_head = NULL;
    ready_queue_tail = NULL;
    finished_queue = NULL;
    next_thread_id = 1; // start from 1 since main thread is 0
    interruptEnable();
}

static void thread_stub(thFuncPtr func, void *arg)
{
    interruptEnable();        // user code runs with interrupts enabled
    void *result = func(arg); // run user function
    threadExit(result);       // handle cleanup properly
}
extern int threadCreate(thFuncPtr funcPtr, void *argPtr)
{
    /*
    disable interrupts
    allocate TCB
    allocate ucontext_t on heap
    call getcontext
    allocate stack
    assign stack to context
    call:
        makecontext(new_context,
            (void (*)()) thread_stub,
            2,
            funcPtr,
            argPtr);
    enqueue into ready queue
    immediately schedule it (swapcontext)
    enable interrupts
    */
    interruptDisable();
    thread_t *new_thread = malloc(sizeof(thread_t));
    new_thread->id = next_thread_id++;
    new_thread->context = malloc(sizeof(ucontext_t));
    new_thread->all_next = all_threads;
    all_threads = new_thread;
    getcontext(new_thread->context);
    new_thread->stack = malloc(STACK_SIZE);
    new_thread->context->uc_stack.ss_sp = new_thread->stack;
    new_thread->context->uc_stack.ss_size = STACK_SIZE;
    new_thread->context->uc_stack.ss_flags = 0;
    makecontext(new_thread->context, (void (*)())thread_stub, 2, funcPtr, argPtr);
    new_thread->state = READY;
    // added this, forgot?
    new_thread->join_waiter = NULL;
    new_thread->next = NULL; // is this necessary?
    enqueue_ready(new_thread);
    /* immediately schedule*/
    thread_t *prev_thread = current_thread;
    thread_t *next_thread = dequeue_ready();
    next_thread->state = RUNNING;
    current_thread = next_thread;
    swapcontext(prev_thread->context, next_thread->context);

    interruptEnable();
    return new_thread->id;
}

extern void threadYield(void)
{
    /*
    threadYield()
    disable interrupts
    if no other READY thread → return
    move current to READY
    dequeue next READY thread
    mark it RUNNING
    swapcontext
    enable interrupts
    This ensures:
    no starvation
    fairness
    matches grading rubric
    */
    interruptDisable();
    if (ready_queue_head == NULL)
    {
        interruptEnable();
        return;
    }
    thread_t *prev_thread = current_thread;
    prev_thread->state = READY;
    enqueue_ready(prev_thread);
    thread_t *next_thread = dequeue_ready();
    // forgot this line below
    next_thread->state = RUNNING;
    current_thread = next_thread;
    swapcontext(prev_thread->context, next_thread->context);
    interruptEnable();
}
extern void threadJoin(int thread_id, void **result)
{
    /*
    If thread already FINISHED → return immediately
    If thread doesn’t exist → return immediately
    Otherwise:
    mark current as BLOCKED
    associate it with target thread
    schedule next thread
    When target thread finishes:
    wake blocked joiners
    keep finished thread’s return value stored until join happens.
    After join:
    free its stack
    free its context
    free TCB
    allow ID reuse (optional)
    */
    interruptDisable();
    if (current_thread != NULL && current_thread->id == thread_id)
    {
        // joining on self, just return
        interruptEnable();
        return;
    }

    // thread_t *target_thread = finished_queue;
    thread_t *target_thread = find_thread(thread_id);

    if (target_thread == NULL)
    {
        // thread doesn't exist
        interruptEnable();
        return;
    }

    if (target_thread->state == FINISHED)
    {
        if (result)
        {
            *result = target_thread->return_value;
        }
        interruptEnable();
        return;
    }

    current_thread->state = BLOCKED;

    target_thread->join_waiter = current_thread;

    thread_t *next_thread = dequeue_ready();

    if (next_thread == NULL)
    {
        interruptEnable();
        exit(0);
    }

    next_thread->state = RUNNING;

    thread_t *prev = current_thread;
    current_thread = next_thread;

    swapcontext(prev->context, next_thread->context);

    if (result)
        *result = target_thread->return_value;

        // cleanup i suppose
    // free(target_thread->stack);
    // free(target_thread->context);
    // free(target_thread);

    interruptEnable();
}

// exits the current thread -- closing the main thread, will terminate the program
extern void threadExit(void *result)
{
    /*
    disable interrupts
    mark self FINISHED
    store return value
    wake any joining threads
    remove self from scheduling
    schedule next thread
    NEVER return
    Cleanup must be done by another thread (for example inside threadJoin or during scheduling).
    This mirrors real OS process reaping.
    */
    interruptDisable();
    current_thread->state = FINISHED;
    current_thread->return_value = result;
    /* Wake thread waiting in join */ // added hopefully
    if (current_thread->join_waiter)
    {
        current_thread->join_waiter->state = READY;
        enqueue_ready(current_thread->join_waiter);
    }
    // add to finished queue
    current_thread->next = finished_queue;
    finished_queue = current_thread;
    thread_t *next_thread = dequeue_ready();
    if (next_thread == NULL)
    {
        // no more threads to run, just exit
        interruptEnable();
        exit(0);
    }
    next_thread->state = RUNNING;
    thread_t *prev_thread = current_thread;
    current_thread = next_thread;
    swapcontext(prev_thread->context, next_thread->context);
}

// mutex lock types and functions.
struct mutexlock; // opaque type -- you need to implement this in your source file
typedef struct mutexlock mutexlock_t;

extern mutexlock_t *lockCreate(void)
{
    return NULL;
}
extern void lockDestroy(mutexlock_t *lock)
{
    ;
}
extern void threadLock(mutexlock_t *lock)
{
    ;
}
extern void threadUnlock(mutexlock_t *lock)
{
    ;
}

// condition variable types and functions
struct condvar; // opaque type -- you need to implement this in your source file
typedef struct condvar condvar_t;

extern condvar_t *condvarCreate(void)
{
    return NULL;
}
extern void condvarDestroy(condvar_t *cv)
{
    ;
}
extern void threadWait(mutexlock_t *lock, condvar_t *cv)
{
    ;
}
extern void threadSignal(mutexlock_t *lock, condvar_t *cv)
{
    ;
}
