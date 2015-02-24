#!/usr/bin/env python

import filecmp
import os
import sys

def exp(expected = 'test/hello.result'):
    if not filecmp.cmp('out/out.txt', expected):
        print("> Expected:")
        print(open(expected).read())
        print("< But got")
        print(open('out/out.txt').read())
        sys.exit(1)
    print("ok")

def test_3(call, methods=["e","b","t"], expected = 'test/hello.result'):
    for m in methods:
        run(call % m)
        exp(expected)

def run(cmd):
    print("-> " + cmd)
    os.system(cmd)

generic_call = "./bin/bf %s %s > %s"
hello_call = "./bin/bf %s test/hello.bf > out/out.txt"

print("")
print("Testing execution modes")

test_3(hello_call)

run(generic_call % ("c", "test/hello.bf", "out/out.cpp"))
run("g++ out/out.cpp -o out/out && out/out > out/out.txt")
exp()


print("")
print("Testing execution of meta-interpreter")

interpreter_hello_call = "./bin/bf %s test/interpreter.bf < test/hello.bf > out/out.txt"
test_3(interpreter_hello_call)

print("")
print("Testing partial evaluation")

test_3(interpreter_hello_call, ["y"])

generic_call = "./bin/bf %s %s < %s > %s"

run(generic_call % ("x", "test/interpreter.bf", "test/hello.bf", "out/out.bf"))
run(generic_call % ("t", "out/out.bf", "/dev/null", "out/out.txt"))
exp()

print("")
print("Testing partial evaluation of meta-interpreter (stacked 2x)")

run(generic_call % ("x", "test/interpreter.bf", "test/interpreter.bf", "out/out.bf"))
run(generic_call % ("t", "out/out.bf", "test/hello.bf", "out/out.txt"))
exp()

print("")
print("Testing partial evaluation of meta-interpreter (stacked 2x) to C++")

run(generic_call % ("z", "test/interpreter.bf", "test/hello.bf", "out/out.cpp"))
run("g++ out/out.cpp -o out/out && out/out > out/out.txt")
exp()

