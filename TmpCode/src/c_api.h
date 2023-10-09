
typedef struct _message{
    int pid;
    int fid;
    void* input;
    void* output;
}message;


int createAndroid(void** inst);
int destroyAndroid(void* android);
int initAndroid(void* android, void* autosar);
int processAndroid(void* android, message* msg);

int createAutosar(void** inst);
int destroyAutosar(void* Autosar);
int processAutosar(void* Autosar, message* msg);
