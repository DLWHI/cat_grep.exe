#!/bin/bash

SUCCESS=0
FAIL=0
FILE_NAME="s21_cat.c"
DIFF_OUTPUT=""
echo "Checking memory leaks..."
for flag_1 in '' e n s  #--number-nonblank --number --squeeze-blank
do
    for flag_2 in '' e n s t # --number-nonblank --number --squeeze-blank
    do
        for flag_3 in '' e n s # --number-nonblank --number --squeeze-blank
        do
            if [[ $flag_1 != $flag_2 ]] && [[ $flag_1 != $flag_3 ]] && [[ $flag_2 != $flag_3 ]]
            then
                OPTIONS="-$flag_1$flag_2$flag_3$flag_4 $FILE_NAME"
                #echo "sh: $OPTIONS"

                eval "valgrind -q ./s21_cat $OPTIONS" >> mem_report.txt 2>&1
            fi
        done
    done
done

echo "Memory report in file mem_report.txt"
