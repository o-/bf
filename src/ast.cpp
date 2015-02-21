#include "visitor.h"
#include "ast.h"

namespace BF {

#define DEF_ACCEPT_METHOD(klass)                  \
  void klass::accept(Visitor * visitor) { \
    visitor->visit##klass(this);                  \
  }

AST_NODE_LIST(DEF_ACCEPT_METHOD)

}
