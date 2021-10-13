#!/usr/bin/bash

make

if [ $? -ne 0 ]; then
    echo "Compilation failed, cant clean"
    exit 1
fi

make clean