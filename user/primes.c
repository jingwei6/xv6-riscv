#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_PRIMES 42

void debug(int* p, int count) {
  printf("pid=%d, count=%d, ", getpid(), count);
  for (int i = 0; i < count; i++) {
    printf("%d ", p[i]);
  }
  printf("\n");
}

void sieve(const int pr) {
  int primes[MAX_PRIMES];
  int count = 0;
  int p = 0;

  while(read(pr, &p, sizeof(int)) != 0) {
    primes[count++] = p;
  }
  close(pr);

  debug(primes, count);

  if (count == 0) {
    printf("No primes found\n");
    exit(0);
  }
  if (count > 279) {
    printf("count %d overflow\n", count);
    exit(1);
  }
  if (count == 1) {
    const int prime = primes[0];
    printf("pid=%d, prime %d\n", getpid(), prime);
    exit(0);
  }

  int fd[2];
  if(pipe(fd) < 0){
    printf("pipe failed\n");
    exit(1);
  }
 
  const int npr = fd[0];
  const int npw = fd[1];

  const int pid = fork();
  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if (pid > 0) {
    // Parent process
    close(npr);
    const int prime = primes[0];
    printf("pid=%d, prime %d\n", getpid(), prime);
    for (int i = 1; i < count; i++) {
      int current = primes[i];
      if (current % prime != 0) {
        write(npw, &current, sizeof(int));
      }
    }
    close(npw);
    wait(0);
  }
  else {
    // Child process
    close(npw);
    sieve(npr);
  }
}

int main(int argc, char *argv[])
{
  int fd[2];
  if(pipe(fd) < 0){
    printf("pipe failed\n");
    exit(1);
  }
  const int pr = fd[0];
  const int pw = fd[1];

  const int pid = fork();
  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if (pid > 0) {
    // Parent process
    close(pr);
    for (int i = 2; i < MAX_PRIMES; i++) {
      write(pw, &i, sizeof(int));
    }
    close(pw);
    wait(0);
  } else {
    // Child process
    close(pw);
    sieve(pr);
  }

  exit(0);
}
