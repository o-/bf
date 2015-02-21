#ifndef BF_VISITOR_H
#define BF_VISITOR_H

#include <stack>

#include "ast.h"
#include "semantic.h"
#include "alloc.h"
#include "common.h"

namespace BF {

using namespace std;

class Visitor {
  Semantic * semantic;
  stack<Node *> visit;

 public:
  Visitor(Semantic * semantic) : semantic(semantic) {}

  void push(Node * n) {
    visit.push(n);
  }

  void run(Node * ast) {
    push(ast);
    while (visit.size() > 0) {
      Node * c = visit.top();
      visit.pop();
      c->accept(this);
    }
  }

#define DEF_VISIT(klass) \
  virtual void visit##klass(klass *);

  AST_NODE_LIST(DEF_VISIT)

  Constrtuctor2(Visitor, Semantic *)
};

}

#endif
