#include <myfunc.h>
void* routine() {
	pthread_exit(NULL);
}
int main() {
	pthread_t tid1;
	pthread_create(&tid1,NULL,routine,NULL);
	void* ret;
	pthread_join(tid1,&ret);
	printf("ret = %ld",(long)ret);
	return 0;
}
