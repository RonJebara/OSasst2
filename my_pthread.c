#include "my_pthread.h"

my_pthread_tcb threads[15];
int i = 1;
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
	if (alternate == 0){
		printf("%s\n", "Switching to the child thread");
		alternate++;
		setup();		
		swapcontext(&threads[0].context, &threads[i].context);
	}
	else if(alternate == 1){
		printf("%s\n", "Switching to the parent thread");
		alternate--;
		setup();
		swapcontext(&threads[i].context, &threads[0].context);
	}
}

void setup(){
	printf("%s\n", "Setting up timer");
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &schedule;
        sigaction(SIGVTALRM, &sa, NULL);
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = TIME_QUANTUM_MS;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);

 }


/* Create a new TCB for a new thread execution context and add it to the queue
 * of runnable threads. If this is the first time a thread is created, also
 * create a TCB for the main thread as well as initalize any scheduler state.
 */
void my_pthread_create(my_pthread_t *thread, void*(*function)(void*), void *arg){
	if (flag == false){
		threads[0].tid = 0;
		threads[0].status = RUNNABLE;
		getcontext(&maincontext);
		threads[0].context = maincontext;
		flag = true;
	}
	threads[i].tid = i; 
	threads[i].status = RUNNABLE;
	getcontext(&temp);
	temp.uc_link = 0;
	temp.uc_stack.ss_sp = malloc(32768);
	temp.uc_stack.ss_size = 32768;
	makecontext(&temp, function, 0);
	threads[i].context = temp;
	thread = &threads[i].tid;
	schedule(0);
}

/* Give up the CPU and allow the next thread to run.
 */
void my_pthread_yield(){

  // Implement Here

}

/* The calling thread will not continue until the thread with tid thread
 * has finished executing.
 */
void my_pthread_join(my_pthread_t thread){

  // Implement Here //

}


/* Returns the thread id of the currently running thread
 */
my_pthread_t my_pthread_self(){

  // Implement Here //

  return 0; // temporary return, replace this

}

/* Thread exits, setting the state to finished and allowing the next thread
 * to run.
 */
void my_pthread_exit(){

  // Implement Here //

}
