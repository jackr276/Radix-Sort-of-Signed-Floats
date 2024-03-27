/**
 * Author: Jack Robbins
 * 03/29/2024
 * Homework 05, only problem(Radix sort of signed floats)
 */

#include <stdlib.h>
#include <stdio.h>


/**
 * The main function disregards command line paramaters, receives input from the command
 * line, and calls the necessary functions. Following this, it also prints the sorted
 * array back to the command line, one float per line
 */
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
	
	//Perform the radix sort. Note: array is treated as unsigned, so order will be wrong
	radix_sort_unsigned_int(array, numFloats);

	//flip the array into signed order
	to_signed_order(array, numFloats);

	//Print out the now sorted array, floats are rounded to the nearest hundredth
	for(int i = 0; i < numFloats; i++){
		printf("%.2f\n", *((float*)&array[i]));
	}

	//free when done
	free(array);
	return 0;
}


/**
 * This function treats the original float array as an array of unsigned ints, and sorts them
 * using a hexadecimal radix sort(4 bits per iteration). The resultant array is not sorted in
 * proper signed float order, but is sorted in unsigned int order. Another function handles the
 * changing into signed order
 */
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
		//First go through arr, building up the counters array(number of elements per bucket)
		for(int j = 0; j < size; j++){
			//Grab the "ith" nibble
			nibble = (arr[j] >> (4*i)) & mask;
			//The nibble is used as our index. Increment the count and nibble
			counters[nibble]++;
		}

		int sum = 0;
		//Ensure we initialize bucketStart[0] to the first element of buffer
		bucketStart[0] = buffer;
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
			//	*(bucketStart[nibble]) = arr[j];
			**(bucketStart + nibble) = arr[j];
			//Increment pointer by 1 for the next element in this bucket
			bucketStart[nibble]++;
		}

		//swap both arrays
		unsigned int* temp = arr;
		arr = buffer;
		buffer = temp;
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


/**
 * This function takes in an unsigned int array that has already been sorted in unsigned order,
 * and converts it into signed order, using the knowledge that radix sort will sort positive floats
 * in ascending order, and negative floats in descending order
 */
void to_signed_order(unsigned int* arr, int size){
	//First, we need to find the first negative number
	int firstNeg = 0;
	for(; firstNeg < size; firstNeg++){
		//break out once we find the first negative
		if((int)arr[firstNeg] < 0){
			break;
		}
	}

	//Get the subset of all positive floats (still treating them as ints for this purpose)
	unsigned int* posSubset = (unsigned int*)malloc(firstNeg * sizeof(int));
	for(int i = 0; i < firstNeg; i++){
		posSubset[i] = arr[i];
	}

	//Get the subset of all negative floats(still treating them as ints for this purpose)
	unsigned int* negSubset = (unsigned int*)malloc((size-firstNeg) * sizeof(int));
	//We will be mutating this pointer, so make a duplicate to preserve negSubset 
	unsigned int* negSubsetP = negSubset;
	//load them in backwards
	for(int i = size - 1; i >= firstNeg; i--){
		//use pointer for convenience
		*negSubsetP = arr[i];
		negSubsetP++;
	}

	//now both subsets should be in signed order, we can simply combine them
	for(int i = 0; i < size; i++){
		//The first size-firstneg numbers will be negative
		if(i < size - firstNeg){
			arr[i] = negSubset[i];
		} else {
			//The remaining numbers will be positive
			arr[i] = posSubset[i-(size - firstNeg)];
		}
	}

	//Once we're done, free the subset arrays
	free(negSubset);
	free(posSubset);
}

