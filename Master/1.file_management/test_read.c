#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	int fd=open(argv[1],O_RDONLY);
	ERROR_CHECK(fd,-1,"open");

	char buff[1024]={0};
	int sret;
	sret=read(fd,buff,10);
	ERROR_CHECK(sret,-1,"read");
	puts(buff);
	printf("sret=%d\n",sret);


	memset(buff,0,sizeof(buff));
	sret=read(fd,buff,10);
	ERROR_CHECK(sret,-1,"read");
	puts(buff);
	printf("sret=%d\n",sret);


	close(fd);
	return 0;
}
