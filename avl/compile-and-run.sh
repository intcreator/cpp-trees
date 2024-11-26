#!/bin/bash

if 
pmccabe -v *.h *.cpp
then if
    g++ -Wall -Werror -g -std=c++14 *.cpp -o avl.out
    then valgrind --leak-check=full ./avl.out
    # then gdb avl.out
    fi
fi