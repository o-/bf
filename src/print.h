#ifndef BF_PRINT_H
#define BF_PRINT_H

namespace BF {

#include "semantic.h"

using namespace std;

class PrintSemantic : public Semantic {
  int _indent = 0;
  int _line   = 0;

 public:
  string indent() {
    return string(_indent, ' ');
  }

  void br() {
    if (_line + _indent > 72) {
      cout << endl << indent();
      _line = 0;
    }
    _line++;
  }

  void plus(Plus * p, Visitor * v) {
    br(); cout << '+';
  }
  void minus(Minus * p, Visitor * v) {
    br(); cout << '-';
  }
  void left(Left * p, Visitor * v) {
    br(); cout << '<';
  }
  void right(Right * p, Visitor * v) {
    br(); cout << '>';
  }
  void dot(Dot * p, Visitor * v) {
    br(); cout << '.';
  }
  void read(Read * p, Visitor * v) {
    br(); cout << ',';
  }
  void store(char) { }
  void test(Test * p, Visitor * v) {
    _line = 0;
    cout << endl << indent() << '[';
    _indent += 2;
    cout << endl << indent();
    v->push(p->else_node);
    v->push(p->then_node);
  }
  void loopEnd(LoopEnd * e, Visitor * v) {
    _line = 0;
    _indent -= 2;
    cout << endl << indent() << ']' << endl << indent();
  }
  void end(End *, Visitor*) {
    cout << endl;
  }

  Constrtuctor1(PrintSemantic)
};


}

#endif
