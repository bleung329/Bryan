#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define KEY 1024

int readLastLine()
{
	int shmloc = shmget(KEY,sizeof(int),0600);
	int * lineSize = shmat(shmloc,NULL,0);
	char * line = malloc(256);
	printf("%ld bytes\n",(long)lineSize);
	int fd = open("story.txt",O_RDWR);
	printf("Still going\n");
	lseek(fd, -1*(long int)lineSize, SEEK_END);
	read(fd, line, (long unsigned int)lineSize);
	printf("%s\n",line);
	shmdt(lineSize);
	free(line);
	return 0;
}
int writeLastLine(char* str)
{
	int shmloc = shmget(KEY,sizeof(int),0600);
	int * lineSize = shmat(shmloc,0,0);
	int hope = sizeof(str);
	lineSize = &hope;
	int fd = open("story.txt",O_APPEND);
	write(fd,str,sizeof(str));
	shmdt(lineSize);
	return 0;
}



int downSemaphore(int semval)
{
	printf("Checking if file available...\n");
	struct sembuf cmdbuf;
	cmdbuf.sem_num = 0;
	cmdbuf.sem_flg = SEM_UNDO;
	cmdbuf.sem_op = -1;
	semop(semval,&cmdbuf,1);
	printf("File is available!\n");
	return 0;
}
int upSemaphore(int semval)
{
	struct sembuf cmdbuf;
	cmdbuf.sem_num = 0;
	cmdbuf.sem_flg = SEM_UNDO;
	cmdbuf.sem_op = 1;
	semop(semval,&cmdbuf,1);
	return 0;
}


int main()
{
	readLastLine();
	int semval = semget(KEY, 1, 0600);
	if (semval<0)
	{
		printf("Something went wrong, maybe try creating a semaphore?\n");
		return -1;
	}
	downSemaphore(semval);

	printf("Enter another line: ");
	char* stringToAdd = malloc(256);
	fgets(stringToAdd,256,stdin);
	writeLastLine(stringToAdd);

	upSemaphore(semval);
	free(stringToAdd);
	return 0;

}