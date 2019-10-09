#include "my_pthread.h"

my_pthread_tcb threads[15];
int i = 1;
int tempnum=0;
bool flag = false;
int alternate = 0;
ucontext_t temp, maincontext;
struct sigaction sa;
struct itimerval timer;

/* Scheduler State */
 // Fill in Here //



/* Scheduler Function
 * Pick the next runnable thread and swap contexts to start executing
 */
void schedule(int signum){
	//Needs alot of work. RN this just works for ThreadRun
	setup();
	alternate++;
	tempnum=alternate-1;
	if(threads[alternate].tid==-1){	//checks if next thread is invalid (end of list)
		alternate=0;
		swapcontext(&threads[tempnum].context, &threads[alternate].context);
	}
	else{ //next thread is valid
		if(threads[alternate].status==FINISHED){ //checks to next thread can be ran
			while(threads[alternate].status!=RUNNABLE){//looks for the next runnable threadl
			alternate++;
				if(threads[alternate].tid==-1){
				alternate=0;
				}
			}
		}	
	swapcontext(&threads[tempnum].context, &threads[alternate].context);
	}
	
}

void setup(){
	//Dont touch this part
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &schedule;
        sigaction(SIGPROF, &sa, NULL);
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = TIME_QUANTUM_MS;
	setitimer(ITIMER_PROF, &timer, NULL);

 }


/* Create a new TCB for a new thread execution context and add it to the queue
 * of runnable threads. If this is the first time a thread is created, also
 * create a TCB for the main thread as well as initalize any scheduler state.
 */
void my_pthread_create(my_pthread_t *thread, void*(*function)(void*), void *arg){
	//Creates the main thread
	if (flag == false){
		threads[0].tid = 0;
		threads[0].status = RUNNABLE;
		getcontext(&maincontext);
		threads[0].context = maincontext;
		flag = true;
	}
	//Creates the first thread. Need to make another variable besided i to iterate through the array as i will become the variable that tracks what thread is executing
	threads[i].tid = i;
	threads[i+1].tid=-1; 
	threads[i].status = RUNNABLE;
	getcontext(&temp);
	temp.uc_link = 0;
	temp.uc_stack.ss_sp = malloc(32768);
	temp.uc_stack.ss_size = 32768;
	makecontext(&temp, function, 0);
	threads[i].context = temp;
	*thread = threads[i].tid;
	i++;
	setup();
}

/* Give up the CPU and allow the next thread to run.
 */
void my_pthread_yield(){
	schedule(0);
  // Implement Here

}

/* The calling thread will not continue until the thread with tid thread
 * has finished executing.
 */
void my_pthread_join(my_pthread_t thread){
	while(threads[thread].status==RUNNABLE){ //repeats
/*	if(threads[thread].status==RUNNABLE){ //checks for status
		tempnum=alternate;
		alternate=thread;
		swapcontext(&threads[thread].context,&threads[tempnum].context);} //sets the context back to thread
	else{
	return;} */ 
  // Implement Here //
	}
}


/* Returns the thread id of the currently running thread
 */
my_pthread_t my_pthread_self(){

  // Implement Here //

  return threads[alternate].tid; // altnernate should be set to current thread

}

/* Thread exits, setting the state to finished and allowing the next thread
 * to run.
 */
void my_pthread_exit(){

  // Implement Here //
	threads[alternate].status=FINISHED;
	schedule(1);
}
