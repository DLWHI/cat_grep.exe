#!/bin/bash

SUCCESS=0
FAIL=0
FILE_NAME="files/bethoven9.txt"
PATTERN="-e D"
REGEX="D?"
DIFF_OUTPUT=""
echo "Testing grep with pattern $PATTERN..."
for flag_1 in '' -i -v -c -l -n -h -s -o
do
    for flag_2 in '' -i -v -c -l -n -h -s -o
    do
        for flag_3 in '' -i -v -c -l -n -h -s -o
        do
            for flag_4 in '' -i -v -c -l -n -h -s -o
            do
                if [[ $flag_1 != $flag_2 ]] && [[ $flag_1 != $flag_3 ]] && 
                  [[ $flag_1 != $flag_4 ]] && [[ $flag_2 != $flag_3 ]] && 
                  [[ $flag_2 != $flag_4 ]] && [[ $flag_3 != $flag_4 ]]
                then
                    OPTIONS="$flag_1 $flag_2 $flag_3 $flag_4 $PATTERN $FILE_NAME"
                    #echo "sh: $OPTIONS"

                    ./s21_grep $OPTIONS > s21_temp.txt
                    grep $OPTIONS > temp.txt

                    DIFF_OUTPUT="$(diff -s s21_temp.txt temp.txt)"
                    if [ "$DIFF_OUTPUT" == "Files s21_temp.txt and temp.txt are identical" ]
                        then
                            (( SUCCESS++ ))
                        else
                            (( FAIL++ ))
                            echo "$(cmp s21_temp.txt temp.txt)"
                            exit 1
                    fi
                fi
            done
        done
    done
done
echo "Testing grep with pattern $REGEX..."
for flag_1 in '' -i -v -c -l -n -h -s -o
do
    for flag_2 in '' -i -v -c -l -n -h -s -o
    do
        for flag_3 in '' -i -v -c -l -n -h -s -o
        do
            for flag_4 in '' -i -v -c -l -n -h -s -o
            do
                if [[ $flag_1 != $flag_2 ]] && [[ $flag_1 != $flag_3 ]] && 
                  [[ $flag_1 != $flag_4 ]] && [[ $flag_2 != $flag_3 ]] && 
                  [[ $flag_2 != $flag_4 ]] && [[ $flag_3 != $flag_4 ]]
                then
                    OPTIONS="$flag_1 $flag_2 $flag_3 $flag_4 $REGEX $FILE_NAME"
                    #echo "sh: $OPTIONS"

                    ./s21_grep $OPTIONS > s21_temp.txt
                    grep $OPTIONS > temp.txt

                    DIFF_OUTPUT="$(diff -s s21_temp.txt temp.txt)"
                    if [ "$DIFF_OUTPUT" == "Files s21_temp.txt and temp.txt are identical" ]
                        then
                            (( SUCCESS++ ))
                        else
                            (( FAIL++ ))
                            echo "$(cmp s21_temp.txt temp.txt)"
                            exit 1
                    fi
                fi
            done
        done
    done
done
rm s21_temp.txt temp.txt
echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"
