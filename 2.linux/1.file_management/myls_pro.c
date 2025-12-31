#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	DIR* dirp;
	dirp = opendir(argv[1]);
	ERROR_CHECK(dirp,NULL,"opendir");
	struct dirent* pdirent; 
	while((pdirent=readdir(dirp)) != NULL) {
		char str[1024]={0};
		sprintf(str,"%s%s%s",argv[1],"/",pdirent->d_name); //文件名=>path
		struct stat statbuff;
		int ret=stat(str,&statbuff);		
		ERROR_CHECK(ret,-1,"stat");
		char type;
		switch (statbuff.st_mode & S_IFMT) {
			 case S_IFBLK:  type='b';        break;
			 case S_IFCHR:  type='c';        break;
			 case S_IFDIR:  type='d';        break;
			 case S_IFIFO:  type='p';        break;
			 case S_IFLNK:  type='l';        break;
			 case S_IFREG:  type='r';        break;
			 case S_IFSOCK: type='s';        break;
			 default:       type='-';        break;
        }
		printf("%c %o %ju %d %d %ld %s %s\n",
				type,
				statbuff.st_mode,
				statbuff.st_nlink,
				statbuff.st_uid,
				statbuff.st_gid,
				statbuff.st_size,
				ctime(&statbuff.st_mtime),
				pdirent->d_name
				);
	}
	closedir(dirp);
	return 0;
}
