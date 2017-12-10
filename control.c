#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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
    int sd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (sd == -1) {
      printf("Error creating semaphore: %s", strerror(errno));
      return 0;
    }
    data.val = 1;
    semctl(sd, 0, SETVAL, data.val);
    printf("[%d] Created semaphore!", sd);
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
    if (val == -1) {
      printf("Error: %s\n", strerror(errno));
    }
    else {
      printf("[%d] Semaphore removed %d\n", sd, val);
    }
  }

  return 0;
}
