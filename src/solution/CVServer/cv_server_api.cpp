
#include "cv_server/cv_server.h"

extern "C"{
    int CreateCVServer(void** inst){
        auto server_ptr = new CVServer();
        *inst = server_ptr;
        return 0;
    }
    int DestroyCVServer(void* cv_server){
        CVServer *server_ptr = (CVServer*)cv_server;
        delete server_ptr;
        return 0;
    }
    int ProcessCVServer(void* cv_server, message* msg){
        CVServer *server_ptr = (CVServer*)cv_server;
        return server_ptr->process(msg);
    }
    int InitCVServer(void* cv_server, const char* cfg){
        CVServer *server_ptr = (CVServer*)cv_server;
        return server_ptr->init(cfg);
    }
}