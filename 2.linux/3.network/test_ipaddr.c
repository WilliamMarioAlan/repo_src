#include <myfunc.h>
int main(int argc,char*argv[]) {
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	/* 点分十进制转化为32bit的ipv4 */
	char *ip = "192.168.10.174";
	inet_aton(ip,&addr.sin_addr);
	printf("ip = 0x%x\n",addr.sin_addr.s_addr);
	return 0;
}
