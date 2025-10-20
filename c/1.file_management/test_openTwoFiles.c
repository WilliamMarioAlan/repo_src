/* Use open system call opens two files */

#include <stdio.h>
#include<error.h>
#include<fcntl.h>
#include<errno.h>
int main(int argc,char** argv) {
	/* 惯用 判断输入参数是否合法 */
	if (argc != 3) {
		error(1,errno,"Usage:%s file1 file2",argv[0]);	
	}	

	/* 获取第一个fd 应该为3
	 * 0 is stdin
	 * 1 is stdout
	 * 2 is stderr
	 * */
	int fd=open(argv[1],O_RDWR| O_CREAT | O_EXCL,0777);
	if (fd < 0) {
		error(1,errno,"open %s failed",argv[1]);
	}
	printf("fd=%d\n",fd);	
	
	/* 获取第二个fd 会选择最小的可用的index*/
	int fd1=open(argv[2],O_RDWR| O_CREAT | O_EXCL,0777);
	if (fd1 < 0) {
		error(1,errno,"open %s failed",argv[2]);
	}
	printf("fd1=%d\n",fd1);	

	return 0;
	
}
