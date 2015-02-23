#ifndef BF_PARSER_H
#define BF_PARSER_H

#include <sstream>
#include <stack>

#include "alloc.h"
#include "ast.h"

namespace BF {

using namespace std;

class Parser {
  Allocator & alloc;
  Node * top;

 public:
  Parser(Allocator & alloc) : alloc(alloc) {}

  Node * parse(istream & is) {
    char c;
    stack<Test *> loops;

    SequentialNode * _top  = Start::New(alloc);
    top = _top;
    SequentialNode * cur = _top;
    while (is >> c) {
      switch (c) {
        case '+':
          cur = cur->next(Plus::New(alloc));
          break;
        case '-':
          cur = cur->next(Minus::New(alloc));
          break;
        case '<':
          cur = cur->next(Left::New(alloc));
          break;
        case '>':
          cur = cur->next(Right::New(alloc));
          break;
        case '.':
          cur = cur->next(Dot::New(alloc));
          break;
        case ',':
          cur = cur->next(Read::New(alloc));
          break;
        case '[': {
          Test      * test = Test::New(alloc);
          LoopStart * head = LoopStart::New(alloc);
          cur->next(test);
          test->then_node  = head;
          cur              = head;
          loops.push(test);
          break;
        }
        case ']': {
          Test      * test = loops.top();
          LoopStart * cont = LoopStart::New(alloc);
          LoopEnd * end    = LoopEnd::New(alloc);
          cur->next(end);
          end->next(test);
          test->else_node  = cont;
          cur              = cont;
          loops.pop();
          break;
        }
      }
    }

    cur->next(End::New(alloc));

    return top;
  }
};

}

#endif
