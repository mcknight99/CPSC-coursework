#include "mythreads.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

int interruptsAreDisabled = 0;

static void interruptDisable()
{
    interruptsAreDisabled = 1;
}

static void interruptEnable()
{
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
    int id;               // thread ID
    ucontext_t *context;  // execution context
    void *stack;          // pointer to allocated stack (NULL for main thread)
    thread_state_t state; // current state of the thread

    void *return_value; // value to return to joiner (valid if state == FINISHED)

    struct thread *next;        // for ready queue or condition variable wait queue
    struct thread *all_next;    // for list of all threads in the system, //?? change variable name later //?? redundant?
    struct thread *join_waiter; // thread waiting on this one
} thread_t;

static thread_t *current_thread;     // pointer to currently running thread
static thread_t *ready_queue_head;   // head of ready queue
static thread_t *ready_queue_tail;   // tail of ready queue
static thread_t *finished_queue;     // threads waiting to be cleaned up after they finish
static int next_thread_id;           // for assigning thread IDs
static thread_t *all_threads = NULL; // list of every thread in the system

// helper function to add a thread to the ready queue list
static void enqueue_ready(thread_t *thread)
{
    thread->next = NULL;

    if (ready_queue_tail == NULL)
    {
        ready_queue_head = thread;
        ready_queue_tail = thread;
    }
    else
    {
        ready_queue_tail->next = thread;
        ready_queue_tail = thread;
    }
}

// helper function to remove and return the first thread from the ready queue list, or NULL if empty
static thread_t *dequeue_ready()
{
    if (ready_queue_head == NULL)
    {
        return NULL;
    }

    thread_t *thread = ready_queue_head;
    ready_queue_head = thread->next;

    if (ready_queue_head == NULL)
    {
        ready_queue_tail = NULL;
    }

    thread->next = NULL;
    return thread;
}

// helper function to find a thread by ID in the all_threads list, or return NULL if not found
static thread_t *find_thread(int id)
{
    thread_t *thread = all_threads;

    while (thread != NULL)
    //^^ change from last submission
    {
        if (thread->id == id)
        {
            return thread;
        }

        thread = thread->all_next;
    }
    return NULL;
}

// project was getting extremely difficult to debug with a bunch of repeated code,
// and i was thinking how did he manage to do it with only one swap context
// then i realized if im writing a scheduler why not write a function to do that
// and that it would only use swapcontext that one time, and i can use it in a helper function
// Pick the next runnable thread from the ready queue
// Switch context from the current thread to that thread
void schedule()
{
    interruptDisable();

    // cleanup finished threads
    // used to come later but took longer by waiting to clean up
    while (finished_queue != NULL)
    {
        thread_t *t = finished_queue;
        finished_queue = finished_queue->next;

        free(t->stack);
        free(t->context);
        free(t);
    }

    // pick next thread to run
    thread_t *prev = current_thread;
    thread_t *next = dequeue_ready();

    // if no threads are ready then just return
    if (next == NULL)
    {
        interruptEnable();
        return;
    }

    next->state = RUNNING;
    current_thread = next;

    // swap context from prev to next thread
    swapcontext(prev->context, next->context);

    interruptEnable();
}

/*
threadInit -> initialize main thread struct, set up ready queue and finished queue, set next_thread_id to 1

threadStub -> wrapper function to run user function to create my own context surrounding the user function, so i can control before and after of the user function

threadCreate -> create thread struct
    allocate context and stack
    initialize context with getcontext
    set up context stack and makecontext to run thread_stub which runs the user function and then threadExit
    add to all_threads list
    add to ready queue

threadYield -> if ready queue not empty, mark self READY, add to ready queue, schedule()

threadJoin -> find target thread by ID, if not finished, set join_waiter to self, mark self BLOCKED, schedule(), when target finishes and wakes us up, read return value, free target thread resources (stack, context, and struct) since "Once threadJoin has been called on a thread, you may free its results"

threadExit -> mark self FINISHED, store return value, schedule() to switch to another thread, when we switch back to this thread, if join_waiter is set, mark it READY and add to ready queue, add self to finished_queue for cleanup by joiner
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void threadInit(void)
{
    interruptDisable();

    next_thread_id = 0; // main thread gets ID 0, initialize value for next thread to start at 1

    // initialize main thread struct and context
    current_thread = malloc(sizeof(thread_t));
    current_thread->id = next_thread_id++; // bump next_thread_id to 1 for the first created thread after main
    current_thread->context = malloc(sizeof(ucontext_t));
    getcontext(current_thread->context);

    // initialize main thread struct fields, which should be go mostly unused but just to make me happy
    current_thread->stack = NULL;
    current_thread->state = RUNNING;
    current_thread->return_value = NULL;
    current_thread->next = NULL;
    current_thread->all_next = all_threads;
    current_thread->join_waiter = NULL;

    // add main thread to all_threads list
    all_threads = current_thread;

    // initialize ready and finished queues
    ready_queue_head = NULL;
    ready_queue_tail = NULL;
    finished_queue = NULL;

    interruptEnable();
}

// recommended for us to do this by the prof in class, to have a wrapper around the actual function
static void thread_stub(thFuncPtr func, void *arg)
{
    interruptEnable();        // so user code runs with interrupts enabled // didn't have this for the longest of time was a little silly
    void *result = func(arg); // run user function
    threadExit(result);       // handle cleanup properly
}

extern int threadCreate(thFuncPtr funcPtr, void *argPtr)
{

    interruptDisable();

    // create thread struct and allocate context and stack
    thread_t *new_thread = malloc(sizeof(thread_t));
    new_thread->id = next_thread_id++; // assign thread ID and increment for next thread
    new_thread->context = malloc(sizeof(ucontext_t));
    new_thread->stack = malloc(STACK_SIZE);

    // initialize context with getcontext, set up stack, and makecontext to run thread_stub which runs the user function and then threadExit
    getcontext(new_thread->context);
    /*
    https://www.ibm.com/docs/en/zos/3.1.0?topic=functions-makecontext-modify-user-context
    */
    new_thread->context->uc_stack.ss_sp = new_thread->stack; // segfaults without some of these 4 lines lol
    new_thread->context->uc_stack.ss_size = STACK_SIZE;      // happens because makecontext expects the stack to be set up like this, and it won't work if we just pass in a pointer to the stack without setting up the context stack fields
    new_thread->context->uc_stack.ss_flags = 0;              // didnt see a difference with or without this?
    new_thread->context->uc_link = NULL;                     // didnt see a difference with or without this?
                                                             // keeping the above two just to follow the documentation

    // makecontext using the stub wrapper
    makecontext(new_thread->context, (void (*)())thread_stub, 2, funcPtr, argPtr);

    // add to all_threads list and ready queue
    new_thread->state = READY;
    new_thread->join_waiter = NULL;
    new_thread->next = NULL;
    new_thread->all_next = all_threads;
    all_threads = new_thread;
    enqueue_ready(new_thread);

    // immediately run by yield // autograder error big points off here
    threadYield();
    //^ added

    /* might add this instead if above still doesnt work
    current_thread->state = READY;
    enqueue_ready(current_thread);

    schedule();
    */

    interruptEnable();
    return new_thread->id;
}

extern void threadYield(void)
{
    interruptDisable();

    // if no threads are ready then just return and keep running current thread,
    if (ready_queue_head == NULL)
    {
        interruptEnable();
        return;
    }

    // otherwise mark self READY, add to ready queue, and schedule() to switch to another thread
    current_thread->state = READY;
    enqueue_ready(current_thread);

    schedule();

    interruptEnable();
}
extern void threadJoin(int thread_id, void **result)
{
    interruptDisable();

    // find target thread by ID,
    thread_t *target = find_thread(thread_id);

    // making sure thread could be found. ideally won't happen when working properly
    if (target == NULL)
    {
        interruptEnable();
        return;
    }

    // if not finished, set join_waiter to self, mark self BLOCKED, schedule()
    if (target->state != FINISHED)
    {
        target->join_waiter = current_thread;
        current_thread->state = BLOCKED;

        schedule();
    }

    // when target finishes and wakes us up, read return value
    if (result != NULL)
    {
        *result = target->return_value;
    }

    // "Once threadJoin has been called on a thread, you may free its results"
    free(target->stack);
    free(target->context);
    free(target);

    interruptEnable();
}

extern void threadExit(void *result)
{
    interruptDisable();

    // mark self FINISHED and store returned value
    current_thread->state = FINISHED;
    current_thread->return_value = result;

    // when we switch back to this thread, if join_waiter is set (not null),
    if (current_thread->join_waiter != NULL)
    {
        // mark it READY and add to ready queue
        current_thread->join_waiter->state = READY;
        enqueue_ready(current_thread->join_waiter);
    }

    // schedule to switch to another thread
    schedule(); 

    // add self to finished_queue for cleanup by schedule later 
    current_thread->next = finished_queue;
    finished_queue = current_thread;
}

struct mutexlock
{
    int locked;      // 0 for unlocked, 1 for locked
    thread_t *owner; // thread that currently holds the lock, NULL if unlocked

    // allow the lock to keep track of the threads waiting on it
    thread_t *wait_head; // head of queue of threads waiting for the lock
    thread_t *wait_tail; // tail of queue of threads waiting for the lock
};
typedef struct mutexlock mutexlock_t;

extern mutexlock_t *lockCreate(void)
{
    interruptDisable();

    // allocate and initialize lock struct
    mutexlock_t *lock = malloc(sizeof(mutexlock_t));

    // initialize lock fields
    lock->locked = 0;
    lock->owner = NULL;
    lock->wait_head = NULL;
    lock->wait_tail = NULL;

    interruptEnable();
    // finalize creation of lock by returning pointer to it
    return lock;
}
extern void lockDestroy(mutexlock_t *lock)
{
    // do we need to check if nobody is waiting on the lock? or has a lock? or is that assumed to be the responsibility of the user?
    // answer, no: pdf says "This will never be called if a thread holds the lock or is waiting on the lock"
    interruptDisable();
    free(lock);
    interruptEnable();
}

extern void threadLock(mutexlock_t *lock)
{
    interruptDisable();
    /*
    if the lock is free
        take it
    else
        block thread
        add to wait queue
        schedule() to switch to another thread
    */

    if (!lock->locked)
    {
        lock->locked = 1;
        lock->owner = current_thread;
        interruptEnable();
        return;
    }

    current_thread->state = BLOCKED;
    current_thread->next = NULL;

    if (lock->wait_tail == NULL)
    {
        lock->wait_head = current_thread;
        lock->wait_tail = current_thread;
    }
    else
    {
        lock->wait_tail->next = current_thread;
        lock->wait_tail = current_thread;
    }

    schedule();

    // after waking, lock is ours
    lock->owner = current_thread;
    lock->locked = 1;

    interruptEnable();
}

extern void threadUnlock(mutexlock_t *lock)
{
    interruptDisable();

    /*
    if the current thread doesn't hold the lock, do nothing
    if there are threads waiting for the lock
        wake up the first waiting thread (mark it READY and add to ready queue)
        transfer ownership of the lock to that thread
    else
        release the lock (mark it unlocked and owner NULL)
    */

    if (lock->owner != current_thread)
    {
        interruptEnable();
        return;
    }

    if (lock->wait_head)
    {
        thread_t *t = lock->wait_head;

        lock->wait_head = t->next;
        if (lock->wait_head == NULL)
            lock->wait_tail = NULL;

        t->state = READY;
        enqueue_ready(t);

        lock->owner = t;
        lock->locked = 1;
    }
    else
    {
        lock->locked = 0;
        lock->owner = NULL;
    }

    interruptEnable();
}

struct condvar
{
    // pretty similar implementation to threading queue and lock queue, just a queue of threads waiting on the condition variable
    thread_t *wait_head; // head of queue of threads waiting on this condition variable
    thread_t *wait_tail; // tail of queue of threads waiting on this condition variable
};
typedef struct condvar condvar_t;

extern condvar_t *condvarCreate(void)
{
    interruptDisable();

    // allocate and initialize condition variable struct
    condvar_t *cv = malloc(sizeof(condvar_t));
    cv->wait_head = NULL;
    cv->wait_tail = NULL;

    interruptEnable();
    return cv;
}
extern void condvarDestroy(condvar_t *cv)
{
    interruptDisable();
    free(cv);
    interruptEnable();
}

extern void threadWait(mutexlock_t *lock, condvar_t *cv)
{
    interruptDisable();
    /*
    add thread to CV wait queue
    release the lock
    block the thread (schedule)
    when awakened reacquire the lock
    */

    current_thread->state = BLOCKED;
    current_thread->next = NULL;

    if (cv->wait_tail == NULL)
    {
        cv->wait_head = current_thread;
        cv->wait_tail = current_thread;
    }
    else
    {
        cv->wait_tail->next = current_thread;
        cv->wait_tail = current_thread;
    }

    threadUnlock(lock);

    schedule();

    threadLock(lock);

    interruptEnable();
}
extern void threadSignal(mutexlock_t *lock, condvar_t *cv)
{
    interruptDisable();
    /*
    if there are threads waiting for the condition variable,
        wake up the first waiting thread (mark it READY and add to ready queue)
    */

    if (cv->wait_head)
    {
        thread_t *t = cv->wait_head;

        cv->wait_head = t->next;
        if (cv->wait_head == NULL)
            cv->wait_tail = NULL;

        t->state = READY;
        enqueue_ready(t);
    }

    interruptEnable();
}
