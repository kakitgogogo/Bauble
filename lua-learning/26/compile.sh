#!/bin/bash

gcc -g -o $1 $1.c -I /home/kakit/code/lua/src -llua5.1 -lm

#gcc -c -fpic dir.c -I /home/kakit/code/lua/src -llua5.1
#gcc -shared dir.o -o mylib.so