#!/bin/bash

SUCCESS=0
FAIL=0
FILE_NAME="s21_grep.c"
PATTERN="grep"
DIFF_OUTPUT=""
echo "Checking memory leaks..."
for flag_1 in i n c l v  #--number-nonblank --number --squeeze-blank
do
    OPTIONS="-$flag_1 $PATTERN $FILE_NAME"
    echo "sh: $OPTIONS"

    eval "leaks -atExit -- ./s21_grep $OPTIONS" >> mem_report.txt 2>&1
done

echo "Memory report in file mem_report.txt"
