#!/bin/bash

TMP_FILE="tmp.hex"
IAS_OUT="ias.out"

# caminho para o executavel do simulador ias
IAS_EXEC_PATH="./ias"


STR_DIV="-------------------------------------------"


TEST[0]="105 000000000A"    # X = 10   
TEST[1]="105 0000000DAC"    # X = 3500 

RESULT[0]="0x000000000A"
RESULT[1]="0x00000000BB"

if [ $# -lt 1 ]; then
    echo "ERRO!"
    echo "FORMA DE USO:"
    echo " $0 raXXXXXX.hex"
else

    for((I = 0; I < "${#TEST[@]}"; I++))
    do
        rm -f $TMP_FILE $IAS_OUT
        touch $TMP_FILE
        touch $IAS_OUT

        echo $STR_DIV
        echo "${TEST[$I]}"

        while read line           
        do             
            
            NEW_LINE=`echo "${line%%#*}" | tr -d ' '  `

            if [ "${NEW_LINE:0:1}" == '1' ] \
                && [ "${NEW_LINE:1:1}" == "0" ] \
                && [ "${NEW_LINE:2:1}" ==  "5" ]; then
                echo ${TEST[$I]}  >> $TMP_FILE

            elif [ "${NEW_LINE}" != "" ]; then
                NEW_LINE="${NEW_LINE:0:1}${NEW_LINE:1:1}${NEW_LINE:2:1} ${NEW_LINE:3}"
                echo ${NEW_LINE} >> $TMP_FILE
            fi
        done < $1


        $IAS_EXEC_PATH --load=$TMP_FILE >& $IAS_OUT

        STR=`grep "+ AC: " $IAS_OUT | tail -1`

        STR_AC=`echo $STR | cut -d\  -f3`
        STR_AC=`echo $STR_AC | tr -d ' ' `

        STR_PC="${STR#*PC: }"


        if [ "$STR_AC" == "${RESULT[$I]}" ] \
            && [ "$STR_PC" == "0x0000000400" ]; then
            echo "CORRETO"        
        else
            echo "ERRO"
        fi

        echo $STR_DIV
    done
fi
