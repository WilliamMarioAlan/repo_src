#include <myfunc.h>
int main(int argc,char*argv[]) {
	if(argc != 3) {
		error(1,0," Usage %s file length",argv[0]);
	}	
	int fd = open(argv[1],O_RDWR);
	if(fd == -1) {
		error(1,errno,"open systemcall error");
	}
	
	int len=atoi(argv[2]);
	ftruncate(fd,len);	

	char* p = (char*)mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	ERROR_CHECK(p,MAP_FAILED,"mmap");

	for (int i=0;i < len;i++) {
		if (p[i] >= 'a' && p[i] <= 'z') {
			p[i] -= 32;
		}
	}

	munmap(p,len);
	close(fd);
	return 0;
}
