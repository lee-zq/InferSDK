
#include "cv_server/cv_server.h"

extern "C"{
    int createCVServer(void** inst){
        auto q = new CVServer;
        *inst = q;
        return 0;
    }
    int destroyCVServer(void* CVServer){
        void *q = (void*)CVServer;
        delete q;
        return 0;
    }
    int processCVServer(void* CVServer, message* msg){
        void *q = (void*)CVServer;
        return q->process(msg);
    }
}