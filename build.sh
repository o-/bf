#!/bin/sh

g++ -O0 -g -std=c++11 -Wall src/ast.cpp src/main.cpp src/visitor.cpp -o bin/bf

# test all execution modes
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

# run metacircular bf interpreter all execution modes
./bin/bf e test/interpreter.bf < test/hello.bf > out/hello_eval.txt
diff out/hello_eval.txt test/hello.result

./bin/bf b test/interpreter.bf < test/hello.bf > out/hello_bc.txt
diff out/hello_bc.txt test/hello.result

./bin/bf t test/interpreter.bf < test/hello.bf > out/hello_tbc.txt
diff out/hello_tbc.txt test/hello.result

./bin/bf c test/interpreter.bf > out/interpreter.cpp
g++ -O3 out/interpreter.cpp -o out/interpreter
out/interpreter < test/hello.bf > out/hello_comp.txt
diff out/hello_comp.txt test/hello.result

# stacked interpreters
./bin/bf e test/interpreter.bf < test/interpreter.bf < test/interpreter.bf < test/hello.bf > out/hello_eval.txt
diff out/hello_eval.txt test/hello.result

./bin/bf t test/interpreter.bf < test/interpreter.bf < test/interpreter.bf < test/interpreter.bf < test/hello.bf > out/hello_tbc.txt
diff out/hello_tbc.txt test/hello.result

# test trace compiler
./bin/bf x test/interpreter.bf < test/hello.bf  > out/hello_meta.txt
diff out/hello_meta.txt test/hello.result
./bin/bf x test/interpreter.bf < test/interpreter.bf < test/hello.bf > out/hello_meta.txt
diff out/hello_meta.txt test/hello.result

# takes long...
#./bin/bf y test/interpreter.bf < test/hello.bf > out/hello_meta.cpp
#g++ -O0 out/hello_meta.cpp -o out/hello_meta
#out/hello_meta > out/hello_meta.txt
#diff out/hello_meta.txt test/hello.result 

