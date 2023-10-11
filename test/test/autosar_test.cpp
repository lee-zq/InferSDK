
#include "../src/c_api.h"
#include "stdio.h"

int clientTest(){
    void* Client = NULL;
    void* cv_server = NULL;
    int ret = 0;
    ret = createClient(&Client);
    ret = createCVServer(&cv_server);
    ret = initClient(Client, cv_server);
    message msg;
    while (1)
    {
        //从标准输入读取数据指定fid
        printf("please input fid: ");
        //从标准输入读取数据指定fid33
        scanf("%d", &msg.fid);
        ret = processCVServer(Client, &msg);
        //输出结果
        printf("msg.output: %p\n", msg.output);

   }
    
    return 0;
}

int main(){
    test1();
    return 0;
}
