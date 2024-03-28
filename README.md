# Radix Sort of Signed Floating Point Numbers
Author: [Jack Robbins](https://www.github.com/jackr276)

This project contains a C program that uses a hexadecimal radix sort to sort signed integers taken in from standard input. A simple runner script is also provided for user convenience and ease of testing.
## What is Radix Sort?
### Background
Most generic sorting algorithms like insertion sort, shell sort, quicksort, etc., work by comparing two values in an array and swapping them accordingly. Some methods of doing this are more efficient than others. Algorithms like Bubble Sort, the oldest and most basic algorithm, have a runtime of $`O(n^2)`$, and the most efficient known generic sorting algorithm, QuickSort, has an average runtime complexity of $`O(nlogn)`$. However, there is currently no known generic sorting algorithm that has a runtime complexity of $`O(n)`$, or linear runtime. 

This is because, for an algorithm to be generic, at some point the algorithm must compare two items and swap them. An interesting question then arises if we remove the requirement that the algorithm must be generic. When this requirement is dropped, it turns out that there are more efficient ways of sorting **some** kinds of data, specifically primitive kinds of data, whose binary represenation in memory we can manipulate. This is where Radix sort comes in.

### How does Radix Sort work?
Before answering this question it is important to recall how data is organized in memory. At its most fundamental level, all data are 1's and 0's. Both signed and unsigned integers are 4 Bytes, or 4*8 bits, of contiguous 1's and 0's in memory. With this we know that for each bit, there are only 2 possible values, 1 or 0, of which 1 is the largest and 0 is the smallest. We can leverage this fact to arrange data into "buckets", based on their values at a certain bit, Here is a simplified example, sorting 4 nibbles(half bytes):

![Untitled Diagram drawio](https://github.com/jackr276/Radix-Sort-of-Signed-Integers/assets/113046361/aac1c146-3f7d-46b4-8ca9-b175922a3fc9)

By keeping being consistent in how we move from the Least Significant Bit(LSB) up to the Most Significant Bit(MSB), and in how we update the array by merging both buckets together after we sort, the data arranges itself in order. The example above only goes bit by bit, and therefore needs two buckets. However, the program in this project impelements a **hexadecimal radix sort**. As opposed to going bit by bit, the program **radix_sort.c** goes nibble(4 bits) by nibble through each 32 bit integer, and uses 16 buckets, since for each nibble, there are $`2^4 = 16`$ possibilities. However, at a fundamental level, the concepts used in the program and depicted aboce are indentical.

Although this example is very basic and simple, its basic principle of "bucket sorting" based on bits can be applied to 32 bit values like floats, integers, and even 64 bit values like doubles. This program only implements radix sort on integers, but there is another repository that does the same for floating point numbers.

### Time Complexity
Since Radix sort requires us to go through the array of integers 8 times for a hexadecimal radix sort, since there are $`32/4 = 8`$ nibbles to apply our bucket sorting on, the time complexity of this algorithm would be $`8 * n = 8n`$, for sorting $`n`$ integers. Due to some added steps and corrections that are needed(this is all documented in the program), we actually end up with a time complexity around $`8(n + 16 + n) = 16n + 64`$. Regardless, this is still linear time, $`O(n)`$, since constants and coefficients are disregarded in Big O notation. As we can see, by discarding the requirement for our algorithm to be general, we can greatly improve efficiency **when** all we need to do is sort primitive data types.

### What about signed integers?
The above description of radix sort leaves out the fact that, for **signed** integers, the Most Significant Bit is always 1. For this reason, since radix sort looks at all values from an **unsigned** perspective, all of the negative values will be considered to be **greater** than the positive ones, and appear **after** the positive values if no corrections are made. 

Without a correction, sorting this `9 -1 2 -11 23` would yield `2 9 23 -11 -1`. Luckily, this is very easy to fix in an efficient way, simply by grabbing the negative subarray and rearranging the positive one to be after it. Since both positive and negative subsets of integers do come out properly sorted, no additional sorting is needed, only some rearranging.

The function:
```C
void to_signed_order(unsigned int* arr, int size);
```
fixes this problem in **radix_sort.c**.

### Programmatic Implementation
The program [radix_sort.c](https://github.com/jackr276/Radix-Sort-of-Signed-Integers/blob/main/src/radix_sort.c) implements a hexadecimal radix sort of signed integers. The program itself is very well documented, so I will not reiterate its functionality here. I would encourage anyone who is interested to step through the code to understand it.

## Using the runner script
The runner script [run.sh](https://github.com/jackr276/Radix-Sort-of-Signed-Integers/blob/main/src/run.sh) provides a convenient way to run the **radix_sort.c** program. The script takes in a positive integer value and generates the appropriate number of integers, saving them into a file named `integers`. It will then pass the
values in this file into **radix_sort.c** and into the standard `sort` command in Bash. The results of both of these procedures are stored in individual files, and compared using the `diff` command to demonstrate the correct functionality of **radix_sort.c**. An example of how to use this script is given below.

>[!NOTE]
>Be sure to grant executable permissions to `run.sh` by running `chmod +x run.sh`

```console
example@bash:~$ ./run.sh 500
Warning: using 500 integers will generate 3 files each of approximate size 2KB
Are you sure you want to proceed?[Y/N]: Y
# Program output follows this
```
