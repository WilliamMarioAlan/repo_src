#include "myfunc.h"
void* routine(const char* message) {
    printf("%s\n",message);
}
int main() {
    pthread_t tid;
    pthread_create(&tid,NULL,routine,"call thread");
    sleep(1);
}