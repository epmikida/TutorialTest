#include "main.decl.h"

class Main : public CBase_Main {
public:
  Main(CkArgMsg* m) {
    int argc = m->argc;
    char** argv = m->argv;

    CkPrintf("Number of command-line arguments: %i\n", argc);
    for (int i = 0; i < argc; i++) {
      CkPrintf("Argument %i: %s\n", i, argv[i]);
    }
    delete m;

    CkPrintf("Hello World!\n");

    CkExit();
  }
};

#include "main.def.h"
