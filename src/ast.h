#ifndef BF_AST_H
#define BF_AST_H

#include <iostream>

#include "common.h"
#include "alloc.h"

namespace BF {

using namespace std;

class Visitor;

#define AST_NODE_LIST(V) \
 V(Plus)                 \
 V(Minus)                \
 V(Left)                 \
 V(Right)                \
 V(Dot)                  \
 V(Read)                 \
 V(Start)                \
 V(LoopStart)            \
 V(End)                  \
 V(LoopEnd)              \
 V(Test)

#define VisitConstructor(ClassName)              \
  Constrtuctor1(ClassName)                       \
  virtual void accept(Visitor * visitor)


class Node {
 public:
  virtual void accept(Visitor * visitor) = 0;
};

class SequentialNode : public Node {
  Node * _next;
 public:
  Node * next(Node * n) {
    _next = n;
    return n;
  }

  SequentialNode * next(SequentialNode * n) {
    _next = n;
    return n;
  }

  Node * next() const {
    return _next;
  }
};


template<char sym>
class SequentialNodeTemplate : public SequentialNode {
  public:
   const char symbol = sym;
};

class Test : public Node {
 public:
  Node * then_node;
  Node * else_node;

  VisitConstructor(Test);
};

class Start : public SequentialNodeTemplate<'S'> {
 public:
  VisitConstructor(Start);
};

class LoopStart : public SequentialNodeTemplate<' '> {
 public:
  VisitConstructor(LoopStart);
};

class LoopEnd : public Node {
  Test * _next;
 public:
  void next(Test * test) {
    _next = test;
  }
  Node * next() {
    return _next;
  }
  VisitConstructor(LoopEnd);
};

class End : public Node {
 public:
  VisitConstructor(End);
};

class Plus : public SequentialNodeTemplate<'+'> {
 public:
  VisitConstructor(Plus);
};

class Minus : public SequentialNodeTemplate<'-'> {
 public:
  VisitConstructor(Minus);
};

class Left : public SequentialNodeTemplate<'<'> {
 public:
  VisitConstructor(Left);
};

class Right : public SequentialNodeTemplate<'>'> {
 public:
  VisitConstructor(Right);
};

class Dot : public SequentialNodeTemplate<'.'> {
 public:
  VisitConstructor(Dot);
};

class Read : public SequentialNodeTemplate<','> {
 public:
  VisitConstructor(Read);
};

}

#endif
