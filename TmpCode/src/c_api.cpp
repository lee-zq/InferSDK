
#include "autosar/autosar.h"

extern "C"{
    int createAutosar(void** inst){
        auto q = new Autosar;
        *inst = q;
        return 0;
    }
    int destroyAutosar(void* Autosar){
        IAutosar *q = (IAutosar*)Autosar;
        delete q;
        return 0;
    }
    int processAutosar(void* Autosar, message* msg){
        IAutosar *q = (IAutosar*)Autosar;
        return q->process(msg);
    }
}