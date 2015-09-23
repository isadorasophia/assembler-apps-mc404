#!/bin/bash

# directories path
TESTS_PATH=./casos-de-teste

CORRECT=/corretos

INCORRECT=/errados

TEMP_DIR=results

# ias-as executable
EXE=./ias-as

# create temp directory
mkdir ${TEMP_DIR}

# create output to all the correct test cases
for i in $(ls ${TESTS_PATH}${CORRECT} -1 | grep .txt$)
do
	${EXE} ${TESTS_PATH}${CORRECT}/${i} ${TEMP_DIR}/${i}.hex > /dev/null 2>&1
done

# compare the result
for i in $(ls ${TESTS_PATH}${CORRECT} -1 | grep .txt.hex$)
do
	echo -n "Checking file ${i}:"
	if diff ${TESTS_PATH}${CORRECT}/${i} ${TEMP_DIR}/${i} > /dev/null; then
		echo -e "\tSuccess!"
	else
		echo -e "\tFailed :("
	fi
done

for i in $(ls ${TESTS_PATH}${INCORRECT} -1 | grep .txt$)
do
	echo -n "Checking file ${i}:"
	${EXE} ${TESTS_PATH}${INCORRECT}/${i} ${TEMP_DIR}/${i}.hex > /dev/null 2>&1
	result=$?
	if [ result != 0 ]; then
		echo -e "\t\tSuccess!"
	else
		echo -e "\t\tFailed :("
	fi
done

# delete temp files
rm -r ${TEMP_DIR}
