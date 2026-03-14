// just here to help me keep track of project notes
// i didn't like how the project would have been organized across several files
// so i just put everything in b.c and put notes in here

// provided code snippets for reference

/* Listing 2: Methods for enabling/disabling of interrupts.
s t a t i c void interruptDisable () {
assert (! interruptsAreDisabled );
interruptsAreDisabled = 1;
}
s t a t i c void interruptEnable () {
assert ( interruptsAreDisabled );
interruptsAreDisabled = 0;
}
*/

/* Listing 3: Creating a new execution context

ucontext_t newcontext ; // in your project , this should be on
the heap , not on the stack .
getcontext (& newcontext ); // save the current context
// allocate and initialize a new call stack
newcontext . uc_stack . ss_sp = malloc ( STACK_SIZE );
newcontext . uc_stack . ss_size = STACK_SIZE ;
newcontext . uc_stack . ss_flags = 0;
// modify the context , so that when activated
// func will be called with the 3 arguments , x , y , and z
// in your code , func will be replaced by whatever function you
want
// to run in your new thread .
makecontext (& newcontext ,
( void (*) ( void )) func , 3, x , y , z);

*/

/*Listing 4: Swapping two contexts.
// points to a saved context somewhere in memory
ucontext_t * savedcontext ;
// points to the place in memory where
ucontext_t * where_to_save_currently_running_context ;
// save the running context , and give the saved context the CPU
swapcontext ( where_to_save_currently_running_context , saved
context );
// note that swapcontext () will not return until the initial
context is swapped back 

*/

/*

getcontext

setcontext (available BUT DONT USE THIS IN THE PROJECT)

makecontext (context, then hey when i switch im putting a new stack frame to this thread function i.e. run this code when i switch, then how many args, then the args itself)

swapcontext (save current to first one, switch to second one)

[cpscece3220-s26/project2 at main · sorber/cpscece3220-s26](https://github.com/sorber/cpscece3220-s26/tree/main/project2)

start using gdb. get very well versed about the project in preparation for the project exam. also get familiar with his provided code on github 

disable interrupts when running our code and enable it when we are done and give it back

when writing thread create

when called, we are given a func pointer and an arg

we want to run that func and pass the arg in a new thread

somewhere in thread create

somewhere is a call makecontext

in makecontext you need to pass ( _, func pointer,_) 

you do NOT want to pass the given func pointer to makecontext

you will not be able to enable/disable interrupts, and you cannot figure out when threads are finished

important that you need to pass in a wrapper function that i wrote that gives control over before and after of the actual func given

**basically: pass in my function in makecontext to control before and after the given func pointer and arg**

void mythreadhandler (args)

//when makecontext is called, give this func to it so when we swap to the new context we run mythreadhandler which accepts the original func pointer and its args

- getcontext
- add new stack
- makecontext

call NO pthreads in this project

dont copy context_t structs NO MATTER WHAT

will often have pointers that point to memory within the struct (internal references)

copying a struct to a new point in memory may point to the old location in memory

malloc space for contexts and leave it there until the thread is done

pass a pointer of a struct not a direct struct to a function bc that still copies it

dont pass user to main (?)

make sure to getcontext to initialize and not segfault

 

NO STARVING THREADS
i.e. if there are two and one yields then go to the other, if there are 3 or more make sure none starve


important notes from the pdf:
<ul>
<p>
General
<p> * Remember that your main thread is a bit of a special case. You need to keep track
of it, so that you can schedule it along with the rest of the threads, however you
don’t need to allocate a stack for it (it already has a stack—trying to replace its
stack will end badly). You just need to save its context at the appropriate time.
<p> * Keep in mind that freeing a thread’s resources when it’s finished requires some
care. Normally, when a thread completes, you need to deallocate (free) the fin‑
ished thread’s stack and any other memory you allocated for it. The trick is that
the exiting thread can’t free its own stack safely (that’s like folding up the ladder
you’re standing on). Another thread will need to free its resources after it is fin‑
ished. Also, you can’t (and shouldn’t try) to free the main thread’s resources. Its
stack is not allocated on the heap, and so trying to free it will probably not end
well.
<p> 
1. 
<p> 
2. 
<p>* Both structs must be defined in your library or the test programs will not compile. 
<p>* You should not do any deadlock detection. If a thread calls threadLock() on a
lock that it already holds, it should deadlock. Just let it lock up
<p>
3. Instead, your library can make operations atomic by temporarily disabling these “interrupts”
by setting the interruptsAreDisabled variable to a value of 1. 
This variable is shown in Listing ??. Note that it is extern and must be defined in your library. 
Setting this variable equal to 1 will disable interrupts and prevent the current running code 
from being interrupted. Setting this variable back to 0 will enable interrupts. 
Interrupts should only ever be disabled when executing in one of your library's functions. 
When the user's code is running, interrupts must be enabled. 
I strongly recommend using the two functions shown in Listing ?? to enable and disable interrupts. 
<p>* 
<p>* Hint: you should get your library working in cooperative mode first (without
preemption). Things are much much easier to debug in cooperative mode.
<p>
4. already handled in makefile
<p>
5.
<p>* please don’t copy ucontext_t structs. Store them (using getcontext or swapcontext) 
and then leave them where they are.
<p>* For each thread you create, your library should assign that thread to a unique
identifier (the number returned by threadCreate), in addition to its context.
<p>* When moving on to preemptive mode, interrupts must be disabled when
you swap contexts. Leaving interrupts enabled when calling swapcontext will
almost certainly invite problems (usually seg faults) in the preemptive tests.
<p>* tudents often wonder how they should tell when a thread com‑
pletes. One way to handle this is by using the uc_link field in the context (see
the man page for makecontext). This is the hard way, and I do not recommend
it. Instead, think about how you would determine when a function in a sequen‑
tial program is finished. Note that the function you pass to makecontext doesn’t
have to be the user’s thread function—and your job will be much easier, if it is
not.
</ul>
*/