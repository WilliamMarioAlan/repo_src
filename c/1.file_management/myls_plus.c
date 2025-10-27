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
		printf("%o %ju %d %d %ld %s %s\n",
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