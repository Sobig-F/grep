#!/bin/bash

COUNTER=0
SUCCESS=0
DIFF=""

s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )
flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
    "o"
    "i"
    "s"
    "e"
)
tests=(
"FLAGS Test test_files/test1.txt test_files/test2.txt"
"FLAGS ae test_files/test1.txt test_files/test2.txt"
"FLAGS of test_files/test1.txt test_files/test2.txt"
"FLAGS data test_files/test1.txt test_files/test2.txt"
"FLAGS Data test_files/test1.txt test_files/test2.txt"
"FLAGS can test_files/test1.txt test_files/test2.txt"
"FLAGS have test_files/test1.txt test_files/test2.txt"
)
manual=(
"-ce l test_files/test1.txt test_files/test2.txt"
"-e o -e test test_files/test1.txt test_files/test2.txt"
"-e hello -f test_files/test3.txt test_files/test1.txt test_files/test2.txt"
"-s test do_not_exist.txt"
)
run_test() {
    let "COUNTER++"
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param >> s21_grep.log
    "${sys_command[@]}" $param >> grep.log
    DIFF="$(diff -s s21_grep.log grep.log)"
    echo $COUNTER " ${s21_command[@]} $param" >> all_comm.log
    if [ "$DIFF" != "Files s21_grep.log and grep.log are identical" ]
    then
        echo $(diff -u s21_grep.log grep.log | grep -e '-')
        echo "$COUNTER ${s21_command[@]} $param" >> s21_grep_fail.log
        echo "$COUNTER ${sys_command[@]} $param" >> grep_fail.log
        echo "$COUNTER - Fail $param"
        exit
    else
        let "SUCCESS++"
        # echo "$COUNTER - Success $param"
        rm -f grep.log s21_grep.log
    fi
}

rm -f s21_grep.log grep.log s21_grep_fail.log grep_fail.log
for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3$var4"
                    run_test "$i"
                done
            done
        done
    done
done
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done
echo "ALL - ${COUNTER}"
echo "SUCCESS - ${SUCCESS}"