#!/bin/bash

PARSER="dictionary_parser.py"
DIR=`pwd`

echo "Generate:"
echo "python3 ${PARSER} -i ${2} -o ${4} -id ${6} -t ${8} -m ${10}"
python3 ${PARSER} -i $2 -o $4 --id $6 -t $8 -m ${10}
