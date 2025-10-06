/* 两个windows进行售票 一共20张 */
#include <myfunc.h>
typedef struct shareRes_s {
	int tickets;
	pthread_mutex_t mutex;
}shareRes_t;

//对比wrong版本 对共享资源进行访问时 无论读写 都需要上锁
void *sellWindow1(void* arg) {
	shareRes_t* pshareRes = (shareRes_t*)arg;
	while(1) {
		pthread_mutex_lock(&pshareRes->mutex);
		if (pshareRes->tickets <= 0) {
			pthread_mutex_unlock(&pshareRes->mutex);
			break;
		}
		pshareRes->tickets--;
		printf("After sell,tickets = %d\n",pshareRes->tickets);
		pthread_mutex_unlock(&pshareRes->mutex);
	}
	pthread_exit(NULL);
}

void *sellWindow2(void* arg) {
	shareRes_t* pshareRes = (shareRes_t*)arg;
	while(1) {
		pthread_mutex_lock(&pshareRes->mutex);
		if (pshareRes->tickets <= 0) {
			pthread_mutex_unlock(&pshareRes->mutex);
			break;
		}
		pshareRes->tickets--;
		printf("After sell,tickets = %d\n",pshareRes->tickets);
		pthread_mutex_unlock(&pshareRes->mutex);
	}
	pthread_exit(NULL);
}

int main(int argc,char*argv[]) {
	/* create two threads 并对资源进行初始化 */
	pthread_t tid1,tid2;
	shareRes_t shareRes;
	shareRes.tickets = 20;
	pthread_mutex_init(&shareRes.mutex,NULL);
	pthread_create(&tid1,NULL,sellWindow1,&shareRes);
	pthread_create(&tid2,NULL,sellWindow2,&shareRes);


	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
