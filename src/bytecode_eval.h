#ifndef BF_BYTECODE_EVAL_H
#define BF_BYTECODE_EVAL_H

#include "bytecode.h"

namespace BF {

class BytecodeInterpreter {
  INIT_STORAGE;

  inline label_t readJmpTarget(bytecode_t * bc) {
    return * reinterpret_cast<label_t*>(bc);
  }

 public:
  void eval(bytecode_t * bc) {
    while (true) {
      Bytecode b = static_cast<Bytecode>(*(bc++));

      switch(b) {
        case Bytecode::PLUS:
          S_PLUS;
          break;
        case Bytecode::MINUS:
          S_MINUS;
          break;
        case Bytecode::LEFT:
          S_LEFT;
          break;
        case Bytecode::RIGHT:
          S_RIGHT;
          break;
        case Bytecode::DOT:
          S_DOT;
          break;
        case Bytecode::BACK_JUMP:
          bc -= readJmpTarget(bc);
          break;
        case Bytecode::COND_JUMP:
          if (S_TEST) {
            bc += LabelSize;
          } else {
            bc += readJmpTarget(bc);
          }
          break;
        case Bytecode::RETURN:
          return;
        default:
          __asm("int3");
      }
    }
  }

  Constrtuctor1(BytecodeInterpreter)
};



class ThreadedBytecodeInterpreter {
  void * threaded_code[50000];

  INIT_STORAGE;

  inline label_t readJmpTarget(void ** pc) {
    return * reinterpret_cast<label_t*>(pc);
  }

  inline label_t readJmpTarget(bytecode_t * bc) {
    return * reinterpret_cast<label_t*>(bc);
  }

 public:
  void eval(bytecode_t * bc) {
    { /* Translate Bytecode to Threaded Code */
      unsigned int pc = 0;
      Bytecode b;
      do {
        b = static_cast<Bytecode>(bc[pc]);

        switch(b) {
          case Bytecode::PLUS:
            threaded_code[pc] = &&PLUS_OP;
            break;
          case Bytecode::MINUS:
            threaded_code[pc] = &&MINUS_OP;
            break;
          case Bytecode::LEFT:
            threaded_code[pc] = &&LEFT_OP;
            break;
          case Bytecode::RIGHT:
            threaded_code[pc] = &&RIGHT_OP;
            break;
          case Bytecode::DOT:
            threaded_code[pc] = &&DOT_OP;
            break;
          case Bytecode::BACK_JUMP:
            threaded_code[pc]   = &&BACK_JUMP_OP;
            threaded_code[pc+1] =
              reinterpret_cast<void*>(readJmpTarget(&bc[pc+1]));
            pc += LabelSize;
            break;
          case Bytecode::COND_JUMP:
            threaded_code[pc]   = &&COND_JUMP_OP;
            threaded_code[pc+1] =
              reinterpret_cast<void*>(readJmpTarget(&bc[pc+1]));
            pc += LabelSize;
            break;
          case Bytecode::RETURN:
            threaded_code[pc]   = &&RETURN_OP;
            break;
        }
        pc++;
      } while(b != Bytecode::RETURN);
    }

    { /* Execute Threaded Code */

      register void ** _pc = threaded_code;

#define NEXT() _pc++; goto **(_pc-1)

#define __FENCE __asm__ __volatile__ ("nop");

__FENCE
      NEXT();

__FENCE
PLUS_OP:
      S_PLUS;
      NEXT();

__FENCE
MINUS_OP:
      S_MINUS;
      NEXT();

__FENCE
LEFT_OP:
      S_LEFT;
      NEXT();

__FENCE
RIGHT_OP:
      S_RIGHT;
      NEXT();

__FENCE
DOT_OP:
      S_DOT;
      NEXT();

__FENCE
BACK_JUMP_OP:
      _pc -= readJmpTarget(_pc);
      NEXT();

__FENCE
COND_JUMP_OP:
      if (S_TEST) {
        _pc += LabelSize;
      } else {
        _pc += readJmpTarget(_pc);
      }
      NEXT();

__FENCE
RETURN_OP:
      return;
    }
  }

  Constrtuctor1(ThreadedBytecodeInterpreter)
};


}

#endif
