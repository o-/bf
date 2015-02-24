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
        case Bytecode::READ:
          S_READ;
          break;
        case Bytecode::SET:
          S_STORE (*(bc++));
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
  static const int buffer_len = 50000000;
  void * threaded_code[buffer_len];

  INIT_STORAGE;

  inline label_t readJmpTarget(void ** pc) {
    return * reinterpret_cast<label_t*>(pc);
  }

  label_t readJmpTarget(bytecode_t * bc) {
    return * reinterpret_cast<label_t*>(bc);
  }

 public:
  void eval(bytecode_t * bc) {
    { /* Translate Bytecode to Threaded Code */
      unsigned int pc = 0;
      Bytecode b;

      do {
        if (pc >= buffer_len) __asm("int3");

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
          case Bytecode::READ:
            threaded_code[pc] = &&READ_OP;
            break;
          case Bytecode::SET:
            threaded_code[pc]   = &&SET_OP;
            threaded_code[pc+1] = reinterpret_cast<void*>(bc[pc+1]);
            pc++;
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

#define NEXT() _pc++; goto **(_pc-1)
#define GETOP() *reinterpret_cast<char*>(_pc++)

      void ** _pc = threaded_code;

      NEXT();

      PLUS_OP:
        S_PLUS;
        NEXT();

      MINUS_OP:
        S_MINUS;
        NEXT();

      LEFT_OP:
        S_LEFT;
        NEXT();

      RIGHT_OP:
        S_RIGHT;
        NEXT();

      DOT_OP:
        S_DOT;
        NEXT();

      READ_OP:
        S_READ;
        NEXT();

      SET_OP:
        S_STORE (GETOP());
        NEXT();

      BACK_JUMP_OP:
        _pc -= readJmpTarget(_pc);
        NEXT();

      COND_JUMP_OP:
        if (S_TEST) {
          _pc += LabelSize;
        } else {
          _pc += readJmpTarget(_pc);
        }
        NEXT();

      RETURN_OP:
        return;

#undef NEXT
    }
  }

  Constrtuctor1(ThreadedBytecodeInterpreter)
};


}

#endif
