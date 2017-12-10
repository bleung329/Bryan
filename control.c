#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define KEY 1024

int main( int argc, char *argv[] ) {

  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } data;

  if (argc != 2) {
    printf("Invalid arguments.\n");
    return 0;
  }

  if (!(strcmp(argv[1], "-c"))) {
    int sd = semget(KEY, 1, IPC_EXCL | IPC_CREAT | 0600);
	int fd = open("story.txt",O_TRUNC|O_CREAT|O_EXCL);
	int shmloc = shmget(KEY,256,IPC_EXCL|IPC_CREAT|0600);
	int *shmatted = shmat(shmloc,NULL,0);
	if (shmatted == -1)
	{
		printf("ALERT ALERT\n");
	}
	shmatted = 69;
	shmdt(shmatted);
    if (sd == -1) {
      printf("Error creating semaphore: %s", strerror(errno));
      return 0;
    }
    data.val = 1;
    semctl(sd, 0, SETVAL, data.val);
    printf("[%d] Created semaphore!\n", sd);
  }

  else if (!(strcmp(argv[1], "-v"))) {
    int sd = semget(KEY, 0, 0644);
    int val = semctl(sd, 0, GETVAL);
    if (val == -1) {
      printf("Error getting semaphore value: %s\n", strerror(errno));
    } else {
      printf("[%d] Semaphore value: %d\n", sd, val);
    }
  }

  else if (!(strcmp(argv[1], "-r"))) {
    int sd = semget(KEY, 0, 0644);
    int val = semctl(sd, 0,  IPC_RMID);
    int shm = shmget(KEY, 256, 0600);
    shmctl(shm,IPC_RMID,0);
    if (val == -1) {
      printf("Error: %s\n", strerror(errno));
    }
    else {
      printf("[%d] Semaphore removed %d\n", sd, val);
    }

  }

  return 0;
}
