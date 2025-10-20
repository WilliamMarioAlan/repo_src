#include <myfunc.h>
typedef struct shareRes_s{
	int global_flag; //0->todo 1->done
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}shareRes_t;


void A() {
	puts("start A");
	/* sleep(1); */
	puts("end A");
}

void B() {
	puts("start B");
	/* sleep(1); */
	puts("end B");
}
void* routine1(void* args) {
	shareRes_t* pshareRes = (shareRes_t*)args;
	pthread_mutex_lock(&pshareRes->mutex);
    //A没做完
	if (pshareRes->global_flag == 0) {
		pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
	}	
	pthread_mutex_unlock(&pshareRes->mutex);

	B();
	pthread_exit(NULL);
}

int main(int argc,char*argv[]) {
	shareRes_t shareRes;
	shareRes.global_flag = 0;
	pthread_mutex_init(&shareRes.mutex,NULL);
	pthread_t tid1;
	pthread_create(&tid1,NULL,routine1,&shareRes);
	pthread_cond_init(&shareRes.cond,NULL);

	//开始和child thread并发执行
	A();
	pthread_mutex_lock(&shareRes.mutex);
	shareRes.global_flag = 1;
	//unlock and signal先后不影响语义
	pthread_cond_signal(&shareRes.cond);
	pthread_mutex_unlock(&shareRes.mutex);

	/* puts("start main"); */
	/* sleep(1); */
	/* puts("end main"); */

	pthread_join(tid1,NULL);
	return 0;
}
