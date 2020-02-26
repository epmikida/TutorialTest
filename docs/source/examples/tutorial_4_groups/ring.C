#include "ring.decl.h"

int elements;
CProxy_Main mainProxy;

class Main : public CBase_Main {
public:
  Main(CkArgMsg* msg) {
    elements = 10;
    if (msg->argc > 1) elements = atoi(msg->argv[1]);

    mainProxy = thisProxy;

    CProxy_My1DArray proxy = CProxy_My1DArray::ckNew(elements);
    proxy[0].hello();
  }

  void done() {
    CkExit();
  }
};

class My1DArray : public CBase_My1DArray {
public:
  My1DArray() {
    CkPrintf("Array element %i created on PE %i\n", thisIndex, CkMyPe());
  }

  void hello() {
    CkPrintf("Hello from %i!\n", thisIndex);
    if (thisIndex + 1 < elements) {
      thisProxy[thisIndex + 1].hello();
    } else {
      mainProxy.done();
    }
  }
};

#include "ring.def.h"
