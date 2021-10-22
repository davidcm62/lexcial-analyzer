#!/usr/bin/bash

make

if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

echo ""
echo ""
echo ""


./bin/compiler ./python/wilcoxon.py
# ./compiler ./python/wilcoxon-bad.py