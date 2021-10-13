#!/usr/bin/bash

make

if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

make clean

echo ""
echo "Executing..."
echo ""

# ./compilador ./python/wilcoxon.py
./compilador ./python/wilcoxon-bad.py
# ./compilador ./python/test4.py