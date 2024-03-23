/**
 * Author: Jack Robbins
 * 03/29/2024
 * Homework 05, only problem(Radix sort of signed floats)
 */

#include <stdlib.h>
#include <stdio.h>

int main(void){
	//Predeclare the functions that will be used
	void radix_sort_unsigned_int(unsigned int* arr, int size);
	void to_signed_order(unsigned int* arr, int size);

	//We assume that the first thing entered is the number of floats to be sorted
	int numFloats;
	scanf("%d", &numFloats);
	
	//Allocate the appropriate amount of memory for our floats(float and int is 4 bytes)
	unsigned int* array = (unsigned int*)malloc(numFloats * sizeof(float));

	float received;
	//Grab "numFloats" floats from the command line
	for(int i = 0; i < numFloats; i++){
		scanf("%f", &received);
		//Save the float as an unsigned int value in array
		array[i] = *((unsigned int*)&received);
	}

	radix_sort_unsigned_int(array, numFloats);

	printf("\n\n");

	for(int i = 0; i < numFloats; i++){
		printf("%.2f\n", *(float*)&array[i]);
	}

	//free when done
	free(array);
	return 0;
}


void radix_sort_unsigned_int(unsigned int* arr, int size){
	//predeclare resetArr function
	void resetArr(int* arr, int size);

	//The buffer array will be saved to when we do our bucket sort
	unsigned int* buffer = (unsigned int*)malloc(size * sizeof(float));

	//We need this for in the loop. Each 4 bits is a nibble
	unsigned int nibble;
	//This mask will be used for extracting nibbles
	unsigned int mask = 0xF;
	//We also need an array of counters to represent the size of each bucket
	int* counters = (int*)malloc(16*sizeof(int));
	//Maintain an array of pointers that will point to the next available spot in each bucket
	unsigned int** bucketStart = (unsigned int**)malloc(16*sizeof(int*));

	//Using hexadecimal radix sort, 4 bit chunks, 32/4=8, so 8 iterations needed
	for(int i = 0; i < 8; i++){
		//We'll need to reset counters every iteration
		resetArr(counters, 16);
		printf("here1\n");
		//First go through arr, building up the counters array(number of elements per bucket)
		for(int j = 0; j < size; j++){
			//Grab the "ith" nibble
			nibble = (arr[j] >> (4*i)) & mask;
			printf("%d", nibble);
			//The nibble is used as our index. Increment the count and nibble
			counters[nibble]++;
		}

		int sum = 0;
		printf("here2");
		//Set the pointers to point appropriately into the array
		for(int k = 1; k < 16; k++){
			sum += counters[k-1];
			//tells us the address of where each bucket will start
			bucketStart[k] = &buffer[sum];
		}

		//Now we can use bucketStart to appropriately place each element in the buffer
		for(int j = 0; j < size; j++){
			//Grab the "ith" nibble again
			nibble = (arr[j] >> (4*i)) & mask;
			//Dereference the pointer and place the element arr[j] in the buffer
			*bucketStart[nibble] = arr[j];
			//Increment pointer by 1 for the next element in this bucket
			bucketStart[nibble]++;
		}

		printf("here3");
		//swap both arrays
		unsigned int* temp = arr;
		arr = buffer;
		buffer = temp;

		printf("here4");
	}
	

	//Free when done sorting
	free(counters);
	free(buffer);
	//Bucket start only contains pointers(not malloc'd) so we can free normally
	free(bucketStart);

}


/**
 * A simple helper function that resets every element to 0(needed for counters)
 */
void resetArr(int* arr, int size){
	for(int i = 0; i < size; i++){
		arr[i] = 0;
	}
}
