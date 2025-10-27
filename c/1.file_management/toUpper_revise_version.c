#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	int fd=open(argv[1],O_RDWR);
	ERROR_CHECK(fd,-1,"open");

	char buff[1024]={0};
	while(1) {
		memset(buff,0,1024);
		int ret=read(fd,buff,1024);
		for (int i=0;i<ret;i++) {
			if (buff[i]>='a' && buff[i]<='z') {
				buff[i] -= 32;
			}
		}	
		if (ret == 0) break;
		lseek(fd,-ret,SEEK_CUR);
		write(fd,buff,ret);
	}
	close(fd);
	return 0;
}
