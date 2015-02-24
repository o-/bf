#!/usr/bin/env python

import os
import sys

def exp(expected = 'test/hello.result'):
    a = open(expected).read().rstrip('\n')
    b = open('out/out.txt').read().rstrip('\n')

    #TODO: Not sure why this happens with metacat
    if (ord(b[0]) == 0):
        b = b[1:]

    if not a == b:
        print("> Expected:")
        print(a)
        print([ord(c) for c in a])
        print("< But got")
        print(b)
        print([ord(c) for c in b])
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

print("")
print("Testing cat")

run(generic_call % ("e", "test/cat.bf", "test/hello.result", "out/out.txt"))
exp()

run(generic_call % ("x", "test/interpreter.bf", "test/cat.bf", "out/metacat.bf"))
run(generic_call % ("e", "out/metacat.bf", "test/hello.result", "out/out.txt"))
exp()

run(generic_call % ("b", "out/metacat.bf", "test/hello.result", "out/out.txt"))
exp()
run(generic_call % ("t", "out/metacat.bf", "test/hello.result", "out/out.txt"))
exp()

