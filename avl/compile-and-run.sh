#!/bin/bash

if g++ -Wall -Werror -g -std=c++14 *.cpp -o avl.out ;
then ./avl.out
fi