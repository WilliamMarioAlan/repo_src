//mutex是为了解决多个pthread访问共享资源
#include "myfunc.h"
#define NUM 100000000
typedef struct shareRes_s { //_s表示struct
	int val;
	pthread_mutex_t mutex;
}shareRes_t;                  //_t表示是一个typedef
//把 mutex and resources 打包成一个struct

void* routine(void* args) {
	shareRes_t *shareRes1 = (shareRes_t*)args;  //先转化为本来的type再使用
											
	for (int i = 0;i < NUM;i++) {
		pthread_mutex_lock(&shareRes1->mutex);  //临界区尽可能小 避免出现饥饿问题
		shareRes1->val++;
		pthread_mutex_unlock(&shareRes1->mutex);
	}
	pthread_exit(NULL);
} 
int main() {
	pthread_t tid1;
	shareRes_t shareRes1;
	shareRes1.val = 0;
	pthread_mutex_init(&shareRes1.mutex,NULL);
	pthread_create(&tid1,NULL,routine,&shareRes1);
	for (int i = 0;i < NUM;i++) {
		pthread_mutex_lock(&shareRes1.mutex);
		shareRes1.val++;
		pthread_mutex_unlock(&shareRes1.mutex);
	}
	pthread_join(tid1,NULL);
	printf("val = %d\n",shareRes1.val);

	return 0;
}
