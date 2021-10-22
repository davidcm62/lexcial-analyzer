#!/usr/bin/bash

flex test.l && gcc -lfl lex.yy.c main.c && ./a.out