#include "multiple.decl.h"

class Main : public CBase_Main {
public:
  Main(CkArgMsg* msg) {
    // Default to 10 workers
    int num_workers = 10;

    // Check command-line for number of workers to make
    if (msg->argc > 1) {
      num_workers = atoi(msg->argv[1]);
    }
    delete msg;

    // Create the coordinator chare, telling it how many workers to expect
    CProxy_Coordinator cProxy = CProxy_Coordinator::ckNew(num_workers);

    // Create all the workers, introducing each one to the coordinator
    for (int i = 0; i < num_workers; i++) {
      cProxy.introduce(CProxy_Worker::ckNew(i + 1));
    }
  }
};

class Coordinator : public CBase_Coordinator {
private:
  int total, received;
public:
  Coordinator(int t) : total(t), received(0) {
    CkPrintf("Coordinator created on %i!\n", CkMyPe());
  }

  void introduce(CProxy_Worker wProxy) {
    wProxy.work(thisProxy);
  }

  void done() {
    received++;
    if (received == total) {
      CkPrintf("All work completed!\n");
      CkExit();
    }
  }
};

class Worker : public CBase_Worker {
private:
  int number;
public:
  Worker(int n) : number(n) {
    CkPrintf("Worker #%i created on %i!\n", number, CkMyPe());
  }

  void work(CProxy_Coordinator cProxy) {
    CkPrintf("Worker #%i is working...\n", number);
    cProxy.done();
  }
};

#include "multiple.def.h"
