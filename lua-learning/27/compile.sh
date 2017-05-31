#!/bin/bash

gcc -g -o $1 $1.c -I /home/kakit/code/lua/src -llua5.1 -lm

