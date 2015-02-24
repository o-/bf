#include <fstream>
#include <iostream>

#include "bf.h"

using namespace BF;

int main(int argc, char * argv[]) {
  if (argc == 2 || argc == 3) {
    { Allocator zone;

      char evalType = 'e';
      if (argc == 3) {
        evalType = *argv[1];
      }

      BytecodeCompileSemantic * bcc = NULL;

      Parser p(zone);
      ifstream infile(argv[argc-1]);
      Node * ast = p.parse(infile);

      Semantic * s;
      switch(evalType) {
        case 'p': s = PrintSemantic::New(zone); break;
        case 'c': s = CompileSemantic::New(zone); break;
        case 'e': s = InterpretSemantic::New(zone); break;
        case 'b':
        case 't':
          s = bcc = BytecodeCompileSemantic::New(zone);
          break;
        case 'x':
          bcc = BytecodeCompileSemantic::New(zone);
          s = TraceCompileSemantic::New(
                        zone,
                        InterpretSemantic::New(zone),
                        bcc);
          break;
        case 'y': s = TraceCompileSemantic::New(
                        zone,
                        InterpretSemantic::New(zone),
                        CompileSemantic::New(zone)); break;
        default : goto fail;
      }

      Visitor * v = Visitor::New(zone, s);
      v->run(ast);

      if (bcc != NULL) {
        if (evalType == 'b') {
          BytecodeInterpreter::New(zone)->eval(bcc->getBytecode());
        } else {
          ThreadedBytecodeInterpreter::New(zone)->eval(bcc->getBytecode());
        }
      }
    }
    return 0;
  }

fail:
  cout << "usage: " << argv[0] << " [p|c|e|b] input.bf" << endl;
  return 0;
}
