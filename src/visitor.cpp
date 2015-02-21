#include "visitor.h"
#include "ast.h"
#include "semantic.h"

namespace BF {

void Visitor::visitStart(Start * s) {
  semantic->start(s, this);
  visit.push(s->next());
}

void Visitor::visitLoopStart(LoopStart * s) {
  semantic->loopStart(s, this);
  visit.push(s->next());
}

void Visitor::visitEnd(End * e) {
  semantic->end(e, this);
}

void Visitor::visitLoopEnd(LoopEnd * e) {
  semantic->loopEnd(e, this);
}

void Visitor::visitPlus(Plus * p) {
  semantic->plus(p, this);
  visit.push(p->next());
}

void Visitor::visitMinus(Minus * p) {
  semantic->minus(p, this);
  visit.push(p->next());
}

void Visitor::visitLeft(Left * p) {
  semantic->left(p, this);
  visit.push(p->next());
}

void Visitor::visitRight(Right * p) {
  semantic->right(p, this);
  visit.push(p->next());
}

void Visitor::visitDot(Dot * p) {
  semantic->dot(p, this);
  visit.push(p->next());
}

void Visitor::visitTest(Test * p) {
  semantic->test(p, this);
}

}
