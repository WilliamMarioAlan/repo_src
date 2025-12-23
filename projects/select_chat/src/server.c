#include "../include/myfunc.h"
int main(int argc,char*argv[]) {
	// ./server 192.168.10.174:1234
	ARGS_CHECK(argc,3);
    /*AF_INET:ipv4
     * SOCK_STREAM:TCP流式protocol
    */
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    //inet_addr为ipv4的32Bytes 把点分十进制转化为ip
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"bind func");

    //sockfd为socket监听fd
    listen(sockfd,50);

    struct sockaddr_in clientAddr;
    //socklen must assign the initial value
    socklen_t socklen = sizeof(clientAddr);

    //netfd为新建的socket通信fd
    int netfd = accept(sockfd,(struct sockaddr*)&clientAddr,&socklen);
    printf("client ip = %s, port = %d\n",
            inet_ntoa(clientAddr.sin_addr),
            clientAddr.sin_port); 
    fd_set rdset;


   	return 0;
}


