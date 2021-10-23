#!/usr/bin/bash

flex test.l && gcc lex.yy.c main.c && ./a.out