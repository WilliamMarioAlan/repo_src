#include "myfunc.h"
int main(int argc,char*argv[]) {
	/* htons(host to network short) */
	short port = 0x1234; 
	short big_endian_port = htons(port);
	printf("big endian port = 0x%x\n",big_endian_port);
	return 0;
}
