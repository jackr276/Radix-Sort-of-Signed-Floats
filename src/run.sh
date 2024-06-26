#Author: Jack Robbins
#A simple runner script that allows users to test the floating point radix sort

#!/bin/bash
count=$1

#recompile
gcc -Wall -Wextra radix_sort_floats.c -o radix_sort_floats

#warn users about large file size generation
if [[ count -gt 249 ]]; then
	echo "Warning: using ${count} floats will generate 3 files each of approximate size $(($count*4/1000))KB"
	read -p "Are you sure you want to proceed[Y/N]: " ANSWER

	#abort if user does not agree
	if [[ $ANSWER != "Y" ]] && [[ $ANSWER != "y" ]]; then 
		echo "Aborting"
		exit 0
	fi
fi

#Remove all old files
rm floats
rm radix_sorted 
rm standard_sorted

#Randomly generate floating point input
echo "================== input ======================"
echo ${count} | tee floats 
for (( i=0; i<${count}; i++ )); do
    #generate random decimal values
	printf "%d.%d\n" $((($RANDOM-$RANDOM)%1000)) $RANDOM
done | tee -a floats

#Display the radix sort execution result
echo "============= execution result ================"
cat floats | ./radix_sort_floats | xargs printf "%.2f\n" | tee radix_sorted
tail -n +2 floats | sort -n | xargs printf "%.2f\n" > standard_sorted

#Display any differences between the generic sort and radix sort
echo "====== differences from correct result ======="
diff radix_sorted  standard_sorted
