#include <fstream>
#include <iostream>

#include "bf.h"

using namespace BF;

int main(int argc, char * argv[]) {
  if (argc == 2 || argc == 3) {
    { Allocator zone;

      Parser p(zone);
      ifstream infile(argv[argc-1]);
      Node * ast = p.parse(infile);

      Semantic * s;
      if (argc == 3) {
        switch(*argv[1]) {
          case 'p': s = PrintSemantic::New(zone); break;
          case 'c': s = CompileSemantic::New(zone); break;
          case 'e': s = InterpretSemantic::New(zone); break;
          default : goto fail;
        }
      } else {
        s = InterpretSemantic::New(zone);
      }

      Visitor * v = Visitor::New(zone, s);
      v->run(ast);
    }
    return 0;
  }

fail:
  cout << "usage: " << argv[0] << " [p|c] input.bf" << endl;
  return 0;
}
