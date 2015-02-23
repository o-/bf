#!/bin/sh

g++ -O3 -g -std=c++11 -Wall src/ast.cpp src/main.cpp src/visitor.cpp -o bin/bf

./bin/bf e test/hello.bf > out/hello_eval.txt
diff out/hello_eval.txt test/hello.result

./bin/bf b test/hello.bf > out/hello_bc.txt
diff out/hello_bc.txt test/hello.result

./bin/bf t test/hello.bf > out/hello_tbc.txt
diff out/hello_tbc.txt test/hello.result

./bin/bf c test/hello.bf > out/hello.cpp
g++ -O3 out/hello.cpp -o out/hello
out/hello > out/hello_comp.txt
diff out/hello_comp.txt test/hello.result
