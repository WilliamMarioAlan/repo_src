#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	DIR* dirp;
	dirp = opendir(argv[1]);
	ERROR_CHECK(dirp,NULL,"opendir");
	struct dirent* pdirent; 
	while((pdirent=readdir(dirp)) != NULL) {
		printf("d_ino=%ld\td_reclen=%d\td_type=%d\td_name=%s\n",
				pdirent->d_ino,
				pdirent->d_reclen,
				pdirent->d_type,
				pdirent->d_name);
	}
	closedir(dirp);
	return 0;
}
