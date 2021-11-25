#include <iostream>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fstream>
#include <sys/mman.h>
#include <string>
#include <semaphore.h>
#include <fcntl.h>
#include <wait.h>

int main() {

  int LINE_MAX_SIZE = 256;
  int val1;
  int val2;
  int val3;


  sem_t *semptr = sem_open("CurSem", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 2);
  if (sem_getvalue(semptr, &val1) != 0) {
    perror("SEM_GETVALUE");
    exit(EXIT_FAILURE);
  }
  while (val1++ < 2) {
    sem_post(semptr);
  }
  while (val1-- > 3){
    sem_wait(semptr);
  }

  sem_t *sem2 = sem_open("Sem2", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
  sem_getvalue(sem2,&val2);

  sem_t *sem3 = sem_open("Sem3", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);

  sem_getvalue(semptr,&val1);
  sem_wait(semptr);

  sem_getvalue(semptr,&val1);

  int fd = shm_open("BackingFile", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
  if (fd == -1) {
    perror("OPEN");
    exit(EXIT_FAILURE);
  }

  ftruncate(fd, (off_t)LINE_MAX_SIZE);

  char* memptr = (char*)mmap(
      NULL,
      LINE_MAX_SIZE,
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      fd,
      0);
  if (memptr == MAP_FAILED) {
    perror("MMAP");
    exit(EXIT_FAILURE);
  }

  int str_size = 0;
  char *in = (char *)calloc(1, sizeof(char));
  char c;


  pid_t pid_0; //Идентификатор текущего процесса
  if ((pid_0 = fork()) > 0) {

    while((c = getchar()) != EOF) {

      in[str_size] = c;
      in = (char *)realloc(in, (++str_size + 1) * sizeof(char));
      if (c == '\n') {
        in[str_size] = '\0';
        memset(memptr, '\0', LINE_MAX_SIZE);
        sprintf(memptr,"%s",in);
        sem_getvalue(semptr,&val1);
        sem_post(semptr);
        while(true) {
          sem_getvalue(sem3,&val3);
          if (val3 == 1) {
            printf("%s",memptr);
            break;
          }
        }
        str_size = 0;
      }

    }
    sem_post(sem2);
    sem_post(sem2);
    sem_post(sem2);
    sem_post(semptr);
    sem_post(semptr);
    sem_post(semptr);

    close(fd);
    sem_destroy(semptr);
    sem_destroy(sem2);
    sem_destroy(sem3);
    sem_getvalue(semptr,&val1);
    munmap(memptr,LINE_MAX_SIZE);

  } else if (pid_0 == 0) {
    while(true) {
      if (sem_getvalue(semptr, &val1) != 0) {
        perror("SEM_GETVALUE");
        exit(EXIT_FAILURE);
      }
      if (val1 == 2) {
        execl("4_lab_child_1", "4_lab_child_1", NULL);
        perror("EXECL");
        exit(EXIT_FAILURE);
      }
    }

  } else if (pid_0 < 0) {

    perror("FORK");
    exit(EXIT_FAILURE);

  }
  return 0;
}
