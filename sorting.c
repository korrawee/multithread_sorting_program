#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

#define arr_size 6
typedef struct{
	int* a;
	int* b;
}arrays;

typedef struct{
	int* array;
	int thread;

}data;

void* merge_array(void* parm);
void* bubble_sort(void *parm);


int main(int argc, char **argv[]){

	int sample[arr_size] = {5,2,1,9,19,4}; // Sample array of Integers
	int* array = (int*) malloc(sizeof(arr_size));
	for(int i=0;i< arr_size;i++)
	{
		array[i] = sample[i];
	}

	// create struct of sample and set values	
	data threads[2];
	threads[0].array = array;  
	threads[1].array = array;  
	threads[0].thread = 0;
	threads[1].thread = 1;
	// Declare thread ids
	pthread_t thread_id[3];

	// Declare result 
	int* result1;
	int* result2;
	int* result3;
	printf("BEFORE SORTING\n");

	for(int i=0;i<arr_size;i++)
	{
		printf("%d ",array[i]);
	}
	printf("\n\n");
	// Run first 2 threads
	pthread_create(&thread_id[0], NULL, bubble_sort, &threads[0]);
	pthread_create(&thread_id[1], NULL, bubble_sort, &threads[1]);

	pthread_join(thread_id[0], (void**)&result1);
	pthread_join(thread_id[1], (void**)&result2);
	
	//create arrays struct
	arrays arrs;
	arrs.a = result1;
	arrs.b = result2;

	// Run the final thread to merge 2 results	
	pthread_create(&thread_id[3], NULL, merge_array, &arrs);
	pthread_join(thread_id[3], (void**)&result3);
	
	printf("FINISHED.\n");

	// Print sorted result
	for(int i=0;i<arr_size;i++)
	{
		printf("%d ",result3[i]);
	}
	free(array); // free memory
	printf("\n");
	return 0;
}

void* bubble_sort(void *parm)
{
	//cast parameter to struct
	data* d = (data*)parm;
	int thread = d->thread;

	// do bubble sort each thread
	if(thread == 0){
		printf("thread 0 is RUNNING\n\n");
		for(int i=0;i < arr_size/2-1;i++)
		{
			for(int j=0;j < (arr_size/2)-i-1;j++)
			{
				if(d->array[j] > d->array[j+1])
				{
					int tmp = d->array[j];
					d->array[j] = d->array[j+1];
					d->array[j+1] = tmp;
				}
			}
		}
	}else{
		printf("thread 1 is RUNNING\n\n");
		for(int i=0;i < arr_size/2;i++)
		{
			for(int j=arr_size/2;j < (arr_size)-i-1;j++)
			{
				if(d->array[j] > d->array[j+1])
				{
					int tmp = d->array[j];
					d->array[j] = d->array[j+1];
					d->array[j+1] = tmp;
				}
			}
		}
	}
	pthread_exit(d->array);
}


void* merge_array(void* parm)
{
	printf("Final thread is RUNNING\n\n");
	arrays* d = (arrays*)parm;
	int* result = (int*)malloc(sizeof(arr_size));

	for(int i=0;i<arr_size;i++)
	{
		if(i < arr_size/2){
			result[i] = d->a[i];	
		}else{
			result[i] = d->b[i];	
		}
	}
	for(int i=0;i < arr_size;i++)
	{
		for(int j=0;j < (arr_size)-i-1;j++)
		{
			if(result[j] > result[j+1])
			{
				int tmp = result[j];
				result[j] = result[j+1];
				result[j+1] = tmp;
			}
		}
	}
	pthread_exit(result);
}
