/*
 *两个thread并发执行累加 出现竞争 结果非预期
 * */
#include "myfunc.h"
#define NUM 1000000
void* routine(void* args) {
	int* pval = (int*)args;
	for (int i = 0;i < NUM;i++) {
		(*pval)++;
	}
	pthread_exit(NULL);
} 
int main() {
	pthread_t tid1;
	int val = 0;
	pthread_create(&tid1,NULL,routine,&val);
	for (int i = 0;i < NUM;i++) {
		val++;
	}
	pthread_join(tid1,NULL);
	printf("val = %d\n",val);

	return 0;
}
