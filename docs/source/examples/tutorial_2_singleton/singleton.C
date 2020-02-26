#include "singleton.decl.h"

class Main : public CBase_Main {
public:
  Main(CkArgMsg* msg) {
    delete msg;
    CProxy_MyChare proxy = CProxy_MyChare::ckNew(1);
    proxy.hello(true);

    // Entry method invocation is asynchronous, so this message is printed
    // immediately calling proxy.hello(...). The actual invocation will reach
    // the chare and be executed at some later time.
    CkPrintf("Main constructor complete!\n");
  }
};

class MyChare : public CBase_MyChare {
private:
  int number;
public:
  MyChare(int n) : number(n) {
    CkPrintf("Chare #%i created!\n", number);
  }

  void hello(bool shutdown) {
    CkPrintf("Chare #%i says hello\n", number);
    if (shutdown) {
      CkPrintf("...but now it's time to say goodbye\n");
      CkExit();
    }
  }
};

#include "singleton.def.h"
