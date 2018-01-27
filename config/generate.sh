#!/bin/bash

PARSER="dictionary_parser.py"
DIR=`pwd`
# INPUT=$2
# OUTPUT=$4
# ID=$6
# TYPE=$8
# MEMBER=${10}
INPUT="sdo_dictionary.csv"
OUTPUT="output.json"
ID="config/can/master"
TYPE="driver/mnt/canopen/Mount"
MEMBER="deviceInfo"
echo "Generate:"
echo "python3 ${PARSER} -i ${INPUT} -o ${OUTPUT} --id ${ID} -t ${TYPE} -m ${MEMBER}"
echo "Finished"
