#ifndef BF_BYTECODE_H
#define BF_BYTECODE_H

namespace BF {

typedef uint8_t  bytecode_t;
typedef uint16_t label_t;
typedef uint32_t pc_t;

#define LabelSize (sizeof(label_t) / sizeof(bytecode_t))


enum class Bytecode : bytecode_t {
  PLUS      = 1,
  MINUS     = 2,
  LEFT      = 3,
  RIGHT     = 4,
  DOT       = 5,
  COND_JUMP = 6,
  BACK_JUMP = 7,
  RETURN    = 8
};


class BytecodeCompileSemantic : public Semantic {
  bytecode_t code_buffer[500000] = {0};
  pc_t pc = 0;

  void put(Bytecode byte) {
    code_buffer[pc++] = static_cast<bytecode_t>(byte);
  }

  stack<pc_t> toLink;

  void putBranch() {
    toLink.push(pc);
    put(Bytecode::COND_JUMP);
    pc += LabelSize;
  }

 public:
  void plus(Plus * p, Visitor * v)   { put(Bytecode::PLUS);   }
  void minus(Minus * p, Visitor * v) { put(Bytecode::MINUS);  }
  void left(Left * p, Visitor * v)   { put(Bytecode::LEFT);   }
  void right(Right * p, Visitor * v) { put(Bytecode::RIGHT);  }
  void dot(Dot * p, Visitor * v)     { put(Bytecode::DOT);    }

  void test(Test * p, Visitor * v) {
    putBranch();
    v->push(p->else_node);
    v->push(p->then_node);
  }

  void putJmpTarget(label_t offset, pc_t at) {
    bytecode_t * pos       = &code_buffer[at];
    label_t    * linkLabel = reinterpret_cast<label_t*>(pos);
    *linkLabel = offset;
  }


  void loopEnd(LoopEnd *, Visitor * v) {
    pc_t loopHead = toLink.top();

    // Back jump to loop head
    put(Bytecode::BACK_JUMP);
    putJmpTarget(pc - loopHead, pc);
    pc += LabelSize;

    // Resolve jump target for loop test
    putJmpTarget(pc - loopHead - 1, loopHead + 1);

    toLink.pop();
  }

  void end(End *, Visitor *) {
    put(Bytecode::RETURN);
  }

  bytecode_t * getBytecode() {
    return code_buffer;
  }

  Constrtuctor1(BytecodeCompileSemantic)
};


}

#endif
