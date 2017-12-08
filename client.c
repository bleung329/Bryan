#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <shm.h>
#include <sem.h>
#define KEY 101

int readLastLine()
{
	int shmdesc = shmget(KEY,sizeof(int),0666);
	int lineSize = shmat(shmdesc,0,0);
}
int writeLastLine(char* str)
{

}
int checkSemaphore()
{
	int semloc = semget(101,1,0666);
}

int main()
{
	checkSemaphore();
	printf("Enter another line: ");
	char* stringToAdd = malloc(256);
	fgets(stringToAdd,256,stdin);

}