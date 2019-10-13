#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define NUMTHREADS 4 // Number of threads.

int SharedVariable = 0;
pthread_mutex_t lock;
pthread_barrier_t barr;

void *SimpleThread(void *arg)
{
	int threadId = *((int*)arg);
	int num, val;
	for (num = 0; num < 20; num++)
	{
		// Compilation macro
		#ifdef PTHREAD_SYNC
		pthread_mutex_lock(&lock);
		#endif
		// Increment shared variable
		val = SharedVariable;
		printf("*** thread %d sees value %d\n", threadId, val);
		SharedVariable = val + 1;

		#ifdef PTHREAD_SYNC
		pthread_mutex_unlock(&lock);
		#endif
	}
	val = SharedVariable;
	pthread_barrier_wait(&barr);
	printf("Thread %d sees final value %d\n", threadId, val);
}

int main(int argc, char* argv[])
{
	int i, err;

	// Number of threads is 4.
	pthread_t threads[NUMTHREADS];
	int threadIds[NUMTHREADS] = { 0 };

	// Initialize mutex.
	pthread_mutex_init(&lock, NULL);
	// Initialize barrier.
	pthread_barrier_init(&barr, NULL, NUMTHREADS);

	for (i = 0; i < NUMTHREADS; i++)
	{
		// Giving thread ids.
		threadIds[i] = i;

		// Creating threads
		err = pthread_create(&threads[i], NULL, SimpleThread, (void*)&threadIds[i]);

		if (err)
		{
			printf("ERROR creating threads, return code %d\n", err);
			exit(-1);
		}
	}

	// Waiting for threads to finish.
	for (i = 0; i < NUMTHREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}
