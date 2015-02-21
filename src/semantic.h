#ifndef BF_SEMANTIC_H
#define BF_SEMANTIC_H

namespace BF {

using namespace std;


/* BF semantics */

#define INIT_STORAGE                    \
  unsigned char data[1000] = {0};       \
  unsigned char * ptr      = data + 500

#define S_PLUS  (*ptr)++
#define S_MINUS (*ptr)--
#define S_LEFT  ptr--
#define S_RIGHT ptr++
#define S_DOT   std::cout << *ptr; std::cout.flush()
#define S_TEST  *ptr != 0


/* Abstract Semantic template */

class Semantic {

 public:
  virtual void plus(Plus * plus, Visitor * v) = 0;
  virtual void minus(Minus * minus, Visitor * v) = 0;
  virtual void left(Left * left, Visitor * v) = 0;
  virtual void right(Right * right, Visitor * v) = 0;
  virtual void dot(Dot * dot, Visitor * v) = 0;
  virtual void test(Test * test, Visitor * v) = 0;
  virtual void end(End *, Visitor * v) {};
  virtual void loopEnd(LoopEnd *, Visitor * v) {};
  virtual void start(Start *, Visitor * v) {};
  virtual void loopStart(LoopStart *, Visitor * v) {};
};

}

#endif
