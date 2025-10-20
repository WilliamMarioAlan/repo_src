#include <myfunc.h>
void* routine(void *arg) {
	printf("i'm child thread\n");
	return NULL;
}
int main(int argc,char*argv[]) {
	pthread_t tid;
	pthread_create(&tid,NULL,routine,NULL);
	printf("i'm main thread\n");
	usleep(100);
	return 0;
}
