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

  void start(Start * p, Visitor * v) { c->start(p, v);  i->start(p, v); }
  void end(End * p, Visitor * v)     { c->end(p, v);    i->end(p, v);   }
  void plus(Plus * p, Visitor * v)   { c->plus(p, v);   i->plus(p, v);  }
  void minus(Minus * p, Visitor * v) { c->minus(p, v);  i->minus(p, v); }
  void left(Left * p, Visitor * v)   { c->left(p, v);   i->left(p, v);  }
  void right(Right * p, Visitor * v) { c->right(p, v);  i->right(p, v); }
  void dot(Dot * p, Visitor * v)     { c->dot(p, v);    }

  stack<Semantic*> order;
  void test(Test * p, Visitor * v) {
    if (cin.eof()) {
      c->test(p, v);
      order.push(c);
    } else {
      i->test(p, v);
      order.push(i);
    }
  }

  void loopEnd(LoopEnd * e, Visitor * v) {
    Semantic * s = order.top();
    order.pop();
    s->loopEnd(e, v);
  }

  void read(Read * p, Visitor * v)   {
    if (cin.eof()) {
      c->read(p, v);
    } else {
      char in;
      cin >> noskipws >> in;
      store(in);
    }
  }

  void store(char value) {
    i->store(value);
    c->store(value);
  }

  Constrtuctor3(TraceCompileSemantic, Semantic*, Semantic*)
};


}

#endif
