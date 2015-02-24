#!/bin/sh

g++ -O0 -g -std=c++11 -Wall src/ast.cpp src/main.cpp src/visitor.cpp -o bin/bf
