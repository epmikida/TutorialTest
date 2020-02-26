#include "collective.decl.h"

int elements;
CProxy_Main mainProxy;

class Main : public CBase_Main {
public:
  Main(CkArgMsg* msg) {
    elements = 10;
    if (msg->argc > 1) elements = atoi(msg->argv[1]);

    mainProxy = thisProxy;

    CProxy_My2DArray proxy = CProxy_My2DArray::ckNew(elements, elements);
    proxy.hello();
  }

  void done() {
    CkExit();
  }
};

class My2DArray : public CBase_My2DArray {
public:
  My2DArray() {
    CkPrintf("Array element (%i, %i) created on PE %i\n",
        thisIndex.x, thisIndex.y, CkMyPe());
  }

  void hello() {
    CkPrintf("Hello from (%i, %i)!\n", thisIndex.x, thisIndex.y);
    contribute(CkCallback(CkReductionTarget(Main, done), mainProxy));
  }
};

#include "collective.def.h"
