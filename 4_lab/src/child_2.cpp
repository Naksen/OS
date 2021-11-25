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
  int val2;
  int val3;
  int LINE_MAX_SIZE = 256;

  sem_t *sem2 = sem_open("Sem2", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
  sem_getvalue(sem2,&val2);

  sem_t *sem3 = sem_open("Sem3", O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
  sem_getvalue(sem3,&val3);
  while (val3++ < 0) {
    sem_post(sem3);
  }
  while (val3-- > 1){
    sem_wait(sem3);
  }
  sem_getvalue(sem3,&val3);

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

  while(true) {
    if (sem_getvalue(sem2, &val2) != 0) {
      perror("SEM_GETVALUE");
      exit(EXIT_FAILURE);
    }
    if (val2 == 2) {
      munmap(memptr, LINE_MAX_SIZE);
      close(map_fd);
      break;
    }
    if (val2 == 1) {

      sem_getvalue(sem3,&val3);
      char *out = (char *)calloc(1, sizeof(char));
      size_t m_size = 0;
      for (int i = 0; i + 1 < LINE_MAX_SIZE; ++i) {// преобразование
        if (memptr[i] == ' ' && memptr[i + 1] == ' ') {
          ++i;
          continue;//
        }
        out[m_size] = memptr[i];
        out = (char *)realloc(out, (++m_size + 1) * sizeof(char));
      }
      out[m_size++] = '\0';

      memset(memptr, '\0', LINE_MAX_SIZE);
      sprintf(memptr, "%s", out);
      free(out);

      sem_post(sem3);

      sem_wait(sem3);
      sem_wait(sem2);
    }
  }

  return 0;
}