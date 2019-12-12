#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//Dynamic array to hold value of each fibonacci term.
int* fiboSequence;

// size of fibonacci sequence.
int fiboSequenceSize;

//Number of threads
int countThread;

//Function that creates the series
void* produceSeries(void* arg);

int main(int argc, char* argv[])
{
	//Check number of arguments received
	if (argc != 2)
	{
		printf("Two Arguments are required \n");
		return -1;
	}

	//Check whether is the number is positive or not
	if (atoi(argv[1]) < 0)
	{
		printf("%d , Number entered should be positive \n", atoi(argv[1]));
		return -1;
	}

	fiboSequenceSize = atoi(argv[1]);//convert the input received from command line to integer
	fiboSequence = (int*)malloc(fiboSequenceSize * sizeof(int));//dynamically allocate an array
	pthread_t* threads = (pthread_t*)malloc(fiboSequenceSize * sizeof(pthread_t));

	//create a thread attribute
	pthread_attr_t attrib;
	pthread_attr_init(&attrib);//initialize the created thread attribute

	//for loop used to run threads for each term in fibonacci series
	for (countThread = 0; countThread < fiboSequenceSize; countThread++)
	{
		pthread_create(&threads[countThread], &attrib, produceSeries, NULL);//creates a thread
		pthread_join(threads[countThread], NULL);//Join each threads
	}

	// Displaying the fibonacci series
	printf("The Fibonacci sequence using threads is : ");

	int j;
	for (j = 0; j < fiboSequenceSize; j++)
	{
		printf("%d,", fiboSequence[j]);//Display each term
	}
	return 0;
}

//function used
void* produceSeries(void* arg)
{
	//First term
	if (countThread == 0)
	{
		fiboSequence[countThread] = 0;
		pthread_exit(0);//exit a thread
	}
	if (countThread == 1)
	{
		//Second term
		fiboSequence[countThread] = 1;
		pthread_exit(0);
	}
	else
	{
		//All other terms , terms from 3 to fiboSequenceSize
		fiboSequence[countThread] = fiboSequence[countThread - 1] + fiboSequence[countThread - 2];
		pthread_exit(0);
	}
}