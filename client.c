/*
Brian Leung & Ryan Siu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define KEY 1024

int main() {
  int sd = semget(KEY, 1, 0644);
  if (sd == -1) {
    printf("Error getting semaphore: %s", strerror(errno));
    return -1;
  }

  struct sembuf op;
  op.sem_op = -1;
  op.sem_num = 0;
  op.sem_flg = SEM_UNDO;
  semop(sd, &op, 1);

  int shd = shmget(KEY, sizeof(int), 0600);
  if (shd == -1) {
    printf("Error getting shared mem: %s", strerror(errno));
    return 1;

  }

  int *len = shmat(shd, 0, 0);

  int fd = open("story.txt", O_RDWR | O_APPEND, 0644);
  if (fd == -1) {
    printf("Error opening story: %s", strerror(errno));
    return 1;
  }

  int ret = lseek(fd, -(*len), SEEK_END);
  if (ret == -1) {
    printf("Error getting story position: %s", strerror(errno));
    return 1;
  }

  char *line = (char *) malloc(*len + 1);
  read(fd, line, *len);

  printf("Previous contribution:\n");
  printf("%s\n", line);
  printf(">> ");

  char input[128];
  fgets(input, sizeof(input), stdin);

  write(fd, input, strlen(input));
  close(fd);

  *len = strlen(input);
  shmdt(len);

  op.sem_op = 1;
  semop(sd, &op, 1);

  return 0;

}
