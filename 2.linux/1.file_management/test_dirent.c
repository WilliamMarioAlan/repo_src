#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc,char* argv[]) {
	if (argc != 2) {
		printf("error:%s\n",strerror(errno));
		exit(1);
	} 
	DIR* ptr = opendir(argv[1]);
	if (!ptr) {
		printf("Can't open %s:%s\n",argv[1],strerror(errno));
		exit(1);
	}

	struct dirent* prev;
	errno = 0;
	while ((prev = readdir(ptr)) != NULL) {
		printf("d_ino = %-10llu, d_reclen = %-5hu, d_type = %-5d,\
				d_namlen = %-5d, n_name = %s\n",
				prev->d_ino,
				prev->d_reclen,
				prev->d_type,
				prev->d_namlen,
				prev->d_name);
	}
	if (errno) {

		printf("readdir error:%s",strerror(errno));
		exit(1);
	}
	closedir(ptr);
	return 0;
} 

