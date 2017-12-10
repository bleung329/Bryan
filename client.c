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
	int shmdesc = shmget(KEY,sizeof(int),0666);
	int lineSize = shmat(shmdesc,0,0);
}
int writeLastLine(char* str)
{

}

int downSemaphore(int semval)
{
	struct sembuf cmdbuf;
	cmdbuf.sem_num = 0;
	cmdbuf.sem_flg = SEM_UNDO;
	cmdbuf.sem_op = -1;
	semop(semval,&cmdbuf,1);
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
	upSemaphore(semval);
	return 0;

}