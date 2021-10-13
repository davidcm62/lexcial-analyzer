#!/usr/bin/bash

./compile.sh

if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

echo ""
echo ""
echo ""

# 453 comp lexicos
./bin/compiler ./python/wilcoxon.py
# ./compiler ./python/wilcoxon-bad.py
# ./compiler ./python/test4.py