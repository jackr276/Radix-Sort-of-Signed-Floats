# Radix Sort of Signed Floating Point Numbers
Author: [Jack Robbins](https://www.github.com/jackr276)

This project contains a C program that uses a hexadecimal radix sort to sort signed floating point numbers taken in from standard input. A simple runner script is also provided for user convenience and ease of testing.

## What is Radix Sort?
### Background
Most generic sorting algorithms like insertion sort, shell sort, quicksort, etc., work by comparing two values in an array and swapping them accordingly. Some methods of doing this are more efficient than others. Algorithms like Bubble Sort, the oldest and most basic algorithm, have a runtime of $`O(n^2)`$, and the most efficient known generic sorting algorithm, QuickSort, has an average runtime complexity of $`O(nlogn)`$. However, there is currently no known generic sorting algorithm that has a runtime complexity of $`O(n)`$, or linear runtime. 

This is because, for an algorithm to be generic, at some point the algorithm must compare two items and swap them. An interesting question then arises if we remove the requirement that the algorithm must be generic. When this requirement is dropped, it turns out that there are more efficient ways of sorting **some** kinds of data, specifically primitive kinds of data, whose binary represenation in memory we can manipulate. This is where Radix sort comes in.

### How does Radix Sort work?
Before answering this question it is important to recall how data is organized in memory. At its most fundamental level, all data are 1's and 0's. Both floating point numbers and integers(signed or unsigned) are 4 Bytes, or 4*8 bits, of contiguous 1's and 0's in memory. With this we know that for each bit, there are only 2 possible values, 1 or 0, of which 1 is the largest and 0 is the smallest. We can leverage this fact to arrange data into "buckets", based on their values at a certain bit, Here is a simplified example, sorting 4 nibbles(half bytes):

![Untitled Diagram drawio](https://github.com/jackr276/Radix-Sort-of-Signed-Integers/assets/113046361/aac1c146-3f7d-46b4-8ca9-b175922a3fc9)

By keeping being consistent in how we move from the Least Significant Bit(LSB) up to the Most Significant Bit(MSB), and in how we update the array by merging both buckets together after we sort, the data arranges itself in order. The example above only goes bit by bit, and therefore needs two buckets. However, the program in this project impelements a **hexadecimal radix sort**. As opposed to going bit by bit, the program **radix_sort_floats.c** goes nibble(4 bits) by nibble through each 32 bit integer, and uses 16 buckets, since for each nibble, there are $`2^4 = 16`$ possibilities. However, at a fundamental level, the concepts used in the program and depicted aboce are indentical.

Although this example is very basic and simple, its basic principle of "bucket sorting" based on bits can be applied to 32 bit values like floats, integers, and even 64 bit values like doubles. This program only implements radix sort on integers, but there is another repository that does the same for floating point numbers.

### Time Complexity
Since Radix sort requires us to go through the array of integers 8 times for a hexadecimal radix sort, since there are $`32/4 = 8`$ nibbles to apply our bucket sorting on, the time complexity of this algorithm would be $`8 * n = 8n`$, for sorting $`n`$ integers. Due to some added steps and corrections that are needed(this is all documented in the program), we actually end up with a time complexity around $`8(n + 16 + n) = 16n + 64`$. Regardless, this is still linear time, $`O(n)`$, since constants and coefficients are disregarded in Big O notation. As we can see, by discarding the requirement for our algorithm to be general, we can greatly improve efficiency **when** all we need to do is sort primitive data types.

## Radix sort with floating point numbers
As stated before, both floating point numbers and integers are stored in memory as 4 Bytes of data. Using this fact, we can **choose** to interpret each floating point number as an unsigned integer, and apply the regular radix sort algorithm to them to order them in unsigned integer order. However, because of the way in which floating point numbers are represented using the IEE-754 specification, radix sort will correctly sort the positive floating point numbers in ascending order, but incorrectly sort the negative floating point numbers in descending order. 

Here is a quick example to illustrate the problem:

`-0.23, 1.09, -1.23, 1111.2, 19.1, -54` will be sorted as `1.09 19.1 1111.2 -0.23 -1.23 -54`

Conveniently for us though, it does correctly sort the negative floating point numbers in a descending manner, so it is programmatically simple to fix the order after the radix sort has been applied.

The function: 
```C
void to_signed_order(unsigned int* arr, int size)
```
is called after the inital radix sort is performed to fix the final order of the array.

## Programmatic Implementation
The program [radix_sort_floats.c](https://github.com/jackr276/Radix-Sort-of-Signed-Floats/blob/main/src/radix_sort_floats.c) implements a hexadecimal radix sort of signed floating point numbers. The program itself is very well documented, so I will not reiterate its functionality here. I would encourage anyone who is interested to step through the code to understand it.

## Using the runner script
The runner script [run.sh](https://github.com/jackr276/Radix-Sort-of-Signed-Floats/blob/main/src/run.sh) provides a convenient way to run the **radix_sort_floats.c** program. The script takes in a positive integer value and generates the appropriate number of integers, saving them into a file named `floats`. It will then pass the
values in this file into **radix_sort_floats.c** and into the standard `sort` command in Bash. The results of both of these procedures are stored in individual files, and compared using the `diff` command to demonstrate the correct functionality of **radix_sort_floats.c**. An example of how to use this script is given below.

>[!IMPORTANT]
>Be sure to grant executable permissions to `run.sh` by running `chmod +x run.sh`

```console
example@bash:~$ ./run.sh 500
Warning: using 500 floats will generate 3 files each of approximate size 2KB
Are you sure you want to proceed?[Y/N]: Y
# Program output follows this
```

>[!NOTE]
>Due to differences in the way that printf works in `Bash` versus `C`, when running radix sort on particularly large sets of floating point numbers, there is a chance that you may get something that looks like this:
>```console
>====== differences from correct result =======
>50c50
>< -911.32
>---
>> -911.31
>841c841
>< 637.14
>---
>> 637.13
>```
>Notice how the rounding of these floating point numbers is off by about a hundredth or so. The chance of inconsistencies like this is very low, but when using large sets of floating point numbers, it is normal to have one or two come out like this. It does not affect the sorting algorithm at its core at all, and is merely an issue
>with printing/interpreting the floating point numbers.
