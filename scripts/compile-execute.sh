#!/usr/bin/bash

# Execute this script from project root folder

flex -o src/lexical/lex.yy.c src/lexical/python.l

if [ $? -ne 0 ]; then
    echo "Flex failed"
    exit 1
fi

make

if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

echo ""
echo ""
echo ""


./bin/compiler ./python/wilcoxon.py
# ./bin/compiler ./python/wilcoxon-bad.py
# ./bin/compiler ./python/input.py