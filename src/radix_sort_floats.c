/**
 * Author: Jack Robbins
 * 03/29/2024
 * Homework 05, only problem(Radix sort of signed floats)
 */

#include <stdlib.h>
#include <stdio.h>

int main(void){
	//Predeclare the functions that will be used
	void radix_sort_unsigned(unsigned int* arr, int size);
	void to_signed_order(unsigned int* arr, int size);

	//We assume that the first thing entered is the number of floats to be sorted
	int numFloats;
	scanf("%d", &numFloats);
	
	//Allocate the appropriate amount of memory for our floats(float is 4 bytes)
	unsigned int* array = (unsigned int*)malloc(numFloats * sizeof(float));

	float received;
	//Grab "numFloats" floats from the command line
	for(int i = 0; i < numFloats; i++){
		scanf("%f", &received);
		//Save the float as an unsigned int value in array
		array[i] = *((unsigned int*)&received);
	}


	for(int i = 0; i < numFloats; i++){
		printf("%f\n", *(float*)&array[i]);
	}



	//free when done
	free(array);
	return 0;
}
