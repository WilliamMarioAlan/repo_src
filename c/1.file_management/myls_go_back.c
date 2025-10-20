#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	DIR* dirp;
	dirp = opendir(argv[1]);
	ERROR_CHECK(dirp,NULL,"opendir");
	struct dirent* pdirent; 
	long loc;
	while((pdirent=readdir(dirp)) != NULL) {
		printf("d_ino=%ld\td_reclen=%d\td_type=%d\td_name=%s\n",
				pdirent->d_ino,
				pdirent->d_reclen,
				pdirent->d_type,
				pdirent->d_name);
		if(strcmp("test_fread.c",pdirent->d_name)==0) {
			loc=telldir(dirp); //获取当前position pointer位置
			ERROR_CHECK(loc,-1,"telldir");
		}
	}
	puts("-------------------------------------------");
	/* seekdir(dirp,loc); //set position pointer 到loc位置 */
	rewinddir(dirp);
	pdirent=readdir(dirp); 
	//但是结果不是loc位置到信息 
	//因为readdir的时候 position pointer已经后移 打印的是后一个目录的信息
	printf("d_ino=%ld\td_reclen=%d\td_type=%d\td_name=%s\n",
		pdirent->d_ino,
		pdirent->d_reclen,
		pdirent->d_type,
		pdirent->d_name);

	closedir(dirp);

	return 0;
}
