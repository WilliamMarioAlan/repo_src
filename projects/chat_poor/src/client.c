//./client <server ipv4> <server port>
#include "../myfunc.h"
int main(int argc,char*argv[]) {
	ARGC_CHECK(argc,3);
    /*AF_INET:ipv4
     * SOCK_STREAM:TCP流式protocol
    */
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    //inet_addr把点分十进制转化为ip
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

    ERROR_CHECK(ret,-1,"connect func");
    char buff[4096] = {0};
    recv(sockfd,buff,sizeof(buff),0);
    /*printf("recv = %s\n",buff);*/
    /*send(sockfd,"hello socket",13,0);*/
	return 0;
}
