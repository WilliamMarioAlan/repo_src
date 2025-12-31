#include "myfunc.h"
int main() {
	int i = 0x11223344;
	char* p = (char*)&i;
	/* 地址从低到高 取第一个byte */
	/* low addr存low byte为小端法 反之则为大端法 */
	printf("byte = 0x%x\n",*p);
	return 0;

}
