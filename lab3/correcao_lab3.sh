#!/bin/bash

#Temporary file
TMP_FILE="tmp.hex"

#Output file
IAS_OUT="ias.out"

#Path to the simulator
IAS_EXEC_PATH="./ias"

# Function to report error
function error
{
    echo "ERROR: $@"
    exit 1
}

# Check simulator
[ -f "${IAS_EXEC_PATH}" ] || \
    error "Could not find simulator on path: ${IAS_EXEC_PATH}"

[ -x "${IAS_EXEC_PATH}" ] || \
    error "Simulator file is not executable"

TEST[0]="tests/test0.hex"
TEST[1]="tests/test1.hex"
TEST[2]="tests/test2.hex"

# Check test files

for((I = 0; I < "${#TEST[@]}"; I++))
do
    [ -f "${TEST[$I]}" ] || \
	error "${TEST[$I]} is not a valid input test file."
done

STR_DIV="-------------------------------------------"

if [ $# -lt 1 ]; then
    error "Usage: $0 raXXXXXXX.hex"
else

    [ -f "$1" ] || error "\"$1\" is not a valid file."

    for((I = 0; I < "${#TEST[@]}"; I++))
    do
        rm -f $TMP_FILE $IAS_OUT
        touch $TMP_FILE
        touch $IAS_OUT

        echo $STR_DIV
        echo "Test: ${TEST[$I]}"

        while read line           
        do             
            
            NEW_LINE=`echo "${line%%#*}" | tr -d ' '  `
            
            if [ "$NEW_LINE" != "" ]; then

                ADDRESS="${NEW_LINE:0:1}""${NEW_LINE:1:1}""${NEW_LINE:2:1}"

                ADDRESS=`echo "$ADDRESS" | tr '[:lower:]' '[:upper:]'`
                ADDRESS=`echo "ibase=16; $ADDRESS" | bc`

                if [ "$ADDRESS" -le 64  ]; then
                    NEW_LINE="${NEW_LINE:0:1}${NEW_LINE:1:1}${NEW_LINE:2:1} ${NEW_LINE:3}"
                    echo $NEW_LINE  >> $TMP_FILE
                fi
            fi
        done < $1


        while read line           
        do             
            
            NEW_LINE=`echo "${line%%#*}" | tr -d ' '  `
            
            if [ "$NEW_LINE" != "" ]; then

                ADDRESS="${NEW_LINE:0:1}""${NEW_LINE:1:1}""${NEW_LINE:2:1}"

                if [ "$ADDRESS" = "400"  ]; then
                    RESULT="0x${NEW_LINE:3}"
                else
                    NEW_LINE="${NEW_LINE:0:1}${NEW_LINE:1:1}${NEW_LINE:2:1} ${NEW_LINE:3}"
                    echo $NEW_LINE  >> $TMP_FILE
                fi
            fi
        done < ${TEST[$I]}


        $IAS_EXEC_PATH --load=$TMP_FILE >& $IAS_OUT

        STR=`grep "+ AC: " $IAS_OUT | tail -1`


        STR_AC=`echo $STR | cut -d\  -f3`
        STR_AC=`echo $STR_AC | tr -d ' ' `

        STR_PC="${STR#*PC: }"
        
        STR_PC=`echo "ibase=16; ${STR_PC:2}" | bc`

        if [ "$STR_AC" == "$RESULT" ] \
            && [ "$STR_PC" -ge 1024 ]; then
            echo "CORRETO"        
        else
            echo "ERRO"
        fi

        echo $STR_DIV
    done
fi

rm -f $TMP_FILE $IAS_OUT
