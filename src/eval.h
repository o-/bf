#ifndef BF_CUSTOM_SEMANTICS_H
#define BF_CUSTOM_SEMANTICS_H

namespace BF {

#include "semantic.h"

using namespace std;

#define __STR(a) #a
#define ST(a) __STR(a)


/* eval AST */

class InterpretSemantic : public Semantic {
  INIT_STORAGE;

 public:
  void plus(Plus * p, Visitor * v)   { S_PLUS;  }
  void minus(Minus * p, Visitor * v) { S_MINUS; }
  void left(Left * p, Visitor * v)   { S_LEFT;  }
  void right(Right * p, Visitor * v) { S_RIGHT; }
  void dot(Dot * p, Visitor * v)     { S_DOT;   }
  void read(Read * p, Visitor * v)   { S_READ;  }

  void test(Test * p, Visitor * v) {
    if (S_TEST) {
      v->push(p->then_node);
    } else {
      v->push(p->else_node);
    }
  }

  void loopEnd(LoopEnd * e, Visitor * v) {
    v->push(e->next());
  }

  Constrtuctor1(InterpretSemantic)
};

/* compile AST to c++ */

class CompileSemantic : public Semantic {
  const string preamble = "#include <iostream>\nint main() {";

 public:
  void start(Start *, Visitor * v)   { cout << preamble
                                            << ST(INIT_STORAGE;); }

  void plus(Plus * p, Visitor * v)   { cout << ST(S_PLUS;);       }
  void minus(Minus * p, Visitor * v) { cout << ST(S_MINUS;);      }
  void left(Left * p, Visitor * v)   { cout << ST(S_LEFT;);       }
  void right(Right * p, Visitor * v) { cout << ST(S_RIGHT;);      }
  void dot(Dot * p, Visitor * v)     { cout << ST(S_DOT;);        }
  void read(Read * p, Visitor * v)   { cout << ST(S_READ;);       }

  void test(Test * p, Visitor * v) {
    cout << "while(" << ST(S_TEST) << "){";
    v->push(p->else_node);
    v->push(p->then_node);
  }

  void loopEnd(LoopEnd *, Visitor * v) {
    cout << "}";
  }
  void end(End *, Visitor * v) {
    cout << "}";
  }

  Constrtuctor1(CompileSemantic)
};

}

#endif
