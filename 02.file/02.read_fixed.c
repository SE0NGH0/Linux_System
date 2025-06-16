#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define BUFSIZE	4096

typedef struct _INFO {
	char name[20];
	int age;
	float height;
} INFO;

int main(int argc, char *argv[]){
	int fd, ret, maxread;
	char buf[BUFSIZE];
	off_t filesize;
	struct stat fileinfo;
	off_t pos;
	
	if(argc < 2){
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}
	
	fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("open()");
		return 1;
	}
	
	// fstat(fd, &fileinfo);
	stat(argv[1], &fileinfo);
	filesize = fileinfo.st_size;
	
	if(filesize >= BUFSIZE)
		maxread = BUFSIZE - 1;
	else
		maxread = filesize;

	printf("BUF-PTR : %p\n", &buf);
	printf("FILE-POS : %lx\n", lseek(fd, 0, SEEK_CUR));
	pos=lseek(fd, 0, SEEK_CUR);
	while(1) {
		//memset(buf, 0, sizeof(buf));
		//ret=read(fd, buf, maxread);
        char tmpbuf[sizeof(INFO)];
        ret=read(fd, tmpbuf, sizeof(INFO));
		
		if(ret == 0)
			break;

        
        INFO *tmp = (INFO *)tmpbuf;

        printf("name : %s\n", tmp->name);
        printf("age : %d\n", tmp->age);
        printf("height : %f\n", tmp->height);
		//printf("data : %s\n", buf);
        //printf("data : %d\n", buf + 20);
		pos=lseek(fd, 0, SEEK_CUR);
		printf("BUF-PTR : %p\n", &tmpbuf[pos]);
		printf("FILE-POS : %lx\n", lseek(fd, 0, SEEK_CUR));
        printf("\n");
	}
	close(fd);
	return 0;
}
