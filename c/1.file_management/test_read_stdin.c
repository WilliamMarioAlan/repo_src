#include <myfunc.h>
int main(int argc,char*argv[]) {
	char buff[3]={0};
	int sret=read(0,buff,sizeof(buff));
	ERROR_CHECK(sret,-1,"read");
	printf("sret=%ld buff=%s\n",sret,buff);
	return 0;
}
