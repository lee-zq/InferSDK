#include "android.h"

int Android::Send(message* msg){
    autosar_->process(msg);
    return 0;
}