#ifndef BF_META_EVAL_H
#define BF_META_EVAL_H

namespace BF {

#include <stack>

#include "semantic.h"
#include "eval.h"

using namespace std;

class TraceCompileSemantic : public Semantic {
  Semantic * i;
  Semantic * c;

 public:
  TraceCompileSemantic(Semantic * interpret,
                       Semantic * compile) : i(interpret), c(compile) {}

  bool _known = true;
  bool known() {
    return _known;
  }

  void start(Start * p, Visitor * v) {
    c->start(p, v);
    if (known()) i->start(p, v);
  }
  void end(End * p, Visitor * v) {
    c->end(p, v);
    if (known()) i->end(p, v);
  }
  void plus(Plus * p, Visitor * v) {
    c->plus(p, v);
    if (known()) i->plus(p, v);
  }
  void minus(Minus * p, Visitor * v) {
    c->minus(p, v);
    if (known()) i->minus(p, v);
  }
  void left(Left * p, Visitor * v) {
    c->left(p, v);
    if (known()) i->left(p, v);
  }
  void right(Right * p, Visitor * v) {
    c->right(p, v);
    if (known()) i->right(p, v);
  }
  void store(char value) {
    c->store(value);
    if (known()) i->store(value);
  }

  void dot(Dot * p, Visitor * v) {
    c->dot(p, v);
  }

  stack<Semantic*> order;
  void test(Test * p, Visitor * v) {
    if (known()) {
      i->test(p, v);
      order.push(i);
    } else {
      c->test(p, v);
      order.push(c);
    }
  }

  void loopEnd(LoopEnd * e, Visitor * v) {
    Semantic * s = order.top();
    order.pop();
    s->loopEnd(e, v);
  }

  bool sawEOF = false;
  void read(Read * p, Visitor * v)   {
    if (known()) {
      if (cin.eof()) {
        _known = !sawEOF;
        sawEOF = true;
      }
      char in;
      cin >> in;
      store(in);
    } else {
      c->read(p, v);
    }
  }

  Constrtuctor3(TraceCompileSemantic, Semantic*, Semantic*)
};


}

#endif
