#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

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
	pthread_barrier_wait(&barr);
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", threadId, val);
}

int main(int argc, char* argv[])
{
	int i, err;

	if (argc != 2)
	{
		printf("Invalid number of arguments.\n" +
			"Usage: ./pthread [number of threads]");
		exit(-1);
	}
	else if (strspn(argv[1], "0123456789") != strlen(argv[1])
	{
		printf("There is an invalid character(s) in the argument. Only numbers.");
		printf("\nUsage: ./pthread [number of threads]");
		exit(-1);
	}

	int numThreads = atoi(argv[1]);

	// Number of threads is 4.
	pthread_t threads[numThreads];
	int threadIds[numThreads] = { 0 };

	// Initialize mutex.
	pthread_mutex_init(&lock, NULL);
	// Initialize barrier.
	pthread_barrier_init(&barr, NULL, numThreads);

	for (i = 0; i < numThreads; i++)
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
	for (i = 0; i < numThreads; i++)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}
