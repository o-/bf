#ifndef BF_ALLOC_H
#define BF_ALLOC_H

#include <stack>

namespace BF {

using namespace std;

class Allocator {
  stack<void *> objects;

 public:
  template<typename Object>
  void New(Object ** toAllocate) {
    void   * m = malloc(sizeof(Object));
    Object * n = new (m) Object();
    objects.push(n);
    *toAllocate = n;
  }

  template<typename Object, typename Arg>
  void New(Object ** toAllocate, Arg arg) {
    void   * m = malloc(sizeof(Object));
    Object * n = new (m) Object(arg);
    objects.push(n);
    *toAllocate = n;
  }

  ~Allocator() {
    while (objects.size() > 0) {
      free(objects.top());
      objects.pop();
    }
  }
};

}

#endif
