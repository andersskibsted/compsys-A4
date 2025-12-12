#!/bin/bash


make

echo "Testing fib.elf with 4"
./sim fib.elf -l fib4log.txt -- 4

echo "Testing fib.elf with 16"
./sim fib.elf -l fib16log.txt -- 16

echo "Testing erat.elf and printing the last 1000 bytes of the output"
./sim erat.elf -l eratlog.txt > eratoutput.txt

tail -c 1000 eratoutput.txt

echo "Testing hello.elf"
./sim hello.elf -l hellolog.txt
echo " "
echo " "
echo "Running tests on single instructions"
./sim single_instr_test.elf

rm eratoutput.txt
