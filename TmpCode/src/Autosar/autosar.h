#include "message.h"
#include "../../include/interface.h"

class Autosar : public IAutosar{
public:
    Autosar()=default;
    ~Autosar()=default;

    virtual int process(message* msg);
    
};
// 封装C接口
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