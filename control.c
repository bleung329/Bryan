#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define KEY 1024

void print_story() {
  struct stat sb;

  char story[4096];

  stat("story.txt", &sb);

  int fd = open("story.txt", O_RDONLY, 0644);
  if (fd == -1) {
    printf("Error opening file: %s", strerror(errno));
    return;
  }

  read(fd, &story, sb.st_size);
  close(fd);

  printf("%s\n", story);
}

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
    int sd = semget(KEY, 1, IPC_EXCL | IPC_CREAT | 0644);

    if (sd == -1) {
      printf("Error creating semaphore: %s", strerror(errno));
      return 0;
    }

    data.val = 1;
    int ret = semctl(sd, 0, SETVAL, data.val);
    if (ret == -1) {
      printf("Semaphore operation error: %s", strerror(errno));
      return 0;
    }

    printf("[%d] Created semaphore!\n", sd);

    int shd = shmget(KEY, sizeof(int), IPC_EXCL | IPC_CREAT | 0600);
    if (shd == -1) {
      printf("Error creating shared mem: %s", strerror(errno));
      return 0;
    }

    int fd = open("story.txt", O_RDWR | O_CREAT | O_EXCL);
    printf("[%d] Created file!\n", fd);
    close(fd);
  }

  else if (!(strcmp(argv[1], "-v"))) {
    print_story();
  }

  else if (!(strcmp(argv[1], "-r"))) {
    int sd = semget(KEY, 0, 0644);

    struct sembuf op;
    op.sem_op = -1;
    op.sem_num = 0;
    op.sem_flg = SEM_UNDO;
    semop(sd, &op, 1);

    int ret = semctl(sd, 0, IPC_RMID);
    if (ret == -1) {
      printf("Semaphore operation error: %s", strerror(errno));
    }

    printf("[%d] Removed semaphore!\n", sd);

    int shd = shmget(KEY, sizeof(int), 0600);
    ret = shmctl(shd, IPC_RMID, 0);
    if (ret == -1) {
      printf("Semaphore operation error: %s", strerror(errno));
    }

    printf("[%d] Shared memory removed!\n", shd);

    print_story();

    ret = remove("story.txt");

    if (ret == -1) {
      printf("Error deleting file: %s", strerror(errno));
    }

    printf("Story removed!\n");
  }

  return 0;
}
