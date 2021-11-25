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

int main(int argc, char **argv) {
  int val1;
  int val2;
  int LINE_MAX_SIZE = 256;

  int map_fd = shm_open("BackingFile", O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
  if (map_fd < 0) {
    perror("SHM_OPEN");
    exit(EXIT_FAILURE);
  }

  char* memptr = (char*)mmap(
      NULL,
      LINE_MAX_SIZE,
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      map_fd,
      0);
  if (memptr == MAP_FAILED) {
    perror("MMAP");
    exit(EXIT_FAILURE);
  }

  sem_t *semptr = sem_open("CurSem", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 2);
  if (semptr == SEM_FAILED) {
    perror("SEM_OPEN");
    exit(EXIT_FAILURE);
  }
  if (sem_wait(semptr) != 0) {
    perror("SEM_WAIT");
    exit(EXIT_FAILURE);
  }

  if (sem_getvalue(semptr, &val1) != 0) {
    perror("SEM_GETVALUE");
    exit(EXIT_FAILURE);
  }

  while (val1++ < 3) {
    sem_post(semptr);
  }
  while (val1-- > 4){
    sem_wait(semptr);
  }

  sem_t *sem2 = sem_open("Sem2", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
  sem_getvalue(sem2,&val2);
  while (val2++ < 0) {
    sem_post(sem2);
  }
  while (val2-- > 1){
    sem_wait(sem2);
  }

  sem_getvalue(semptr,&val1);

  pid_t pid_0; //Идентификатор текущего процесса
  if ((pid_0 = fork()) > 0) {

    while(true) {
      if (sem_getvalue(semptr, &val1) != 0) {
        perror("SEM_GETVALUE");
        exit(EXIT_FAILURE);
      }
      if (val1 == 4) {
        munmap(memptr, LINE_MAX_SIZE);
        close(map_fd);
        break;
      } else if (val1 == 3) {

        //cout << "New String" << endl;
        int i = 0;
        memptr[0] = tolower(memptr[0]);
        char c = memptr[0];
        while (c != '\0') {
          ++i;
          c = memptr[i];
          memptr[i] = tolower(memptr[i]);
        }
        ++i;
        //printf("%s",memptr);

        sem_post(sem2);
        sem_wait(semptr);
        //sem_post(sem2);
        sem_getvalue(sem2,&val2);
        //cout << "VAL2 = " << val2 << endl;
        sem_getvalue(semptr,&val1);
        //cout << "VALuE = " << val1 << endl;

      }
    }

  } else if (pid_0 == 0) {
    while(true) {
      if (sem_getvalue(sem2, &val2) != 0) {
        perror("SEM_GETVALUE");
        exit(EXIT_FAILURE);
      }
      if (val2 == 1) {
        //cout << "CHILD 2" << endl;
        //break;
        //cout << "CHILD in PARENT: Semaphore value = " << val1 << endl;
        //printf("CHILD BUF = %s",buf);
        //cout << "CHILD BUF = " << memptr << endl;
        //fflush(stdout);
        execl("4_lab_child_2", "4_lab_child_2", NULL);
        //perror("EXECL");
        //exit(EXIT_FAILURE);
      }
      //sem_wait(semptr);
    }

  } else if (pid_0 < 0) {

    perror("FORK");
    exit(EXIT_FAILURE);

  }
  return EXIT_SUCCESS;
}