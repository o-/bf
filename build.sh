#!/bin/sh

g++ -O2 -g -std=c++11 -pedantic -Wall src/ast.cpp src/main.cpp src/visitor.cpp -o bin/bf

./bin/bf e test/hello.bf > out/hello_eval.txt
diff out/hello_eval.txt test/hello.result

./bin/bf c test/hello.bf > out/hello.cpp
g++ -O2 -g out/hello.cpp -o out/hello
out/hello > out/hello_comp.txt
diff out/hello_comp.txt test/hello.result
