
#include "../src/c_api.h"
#include "stdio.h"
/*
typedef struct _message{
    int pid;
    int fid;
    void* input;
    void* output;
}message;

extern "C"{
    int createAndroid(void** inst);
    int destroyAndroid(void* android);
    int initAndroid(void* android, void* autosar);
    int createAutosar(void** inst);
    int destroyAutosar(void* Autosar);
    int processAutosar(void* Autosar, message* msg);
}
*/
int test1(){
    void* android = NULL;
    void* autosar = NULL;
    int ret = 0;
    ret = createAndroid(&android);
    ret = createAutosar(&autosar);
    ret = initAndroid(android, autosar);
    message msg;
    while (1)
    {
        //从标准输入读取数据指定fid
        printf("please input fid: ");
        //从标准输入读取数据指定fid33
        scanf("%d", &msg.fid);
        ret = processAutosar(android, &msg);
        //输出结果
        printf("msg.output: %p\n", msg.output);

   }
    
    return 0;
}

int main(){
    test1();
    return 0;
}
