#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int fd[2];
  if(pipe(fd) < 0){
    fprintf(2, "pingpong: pipe failed\n");
    exit(1);
  }
  int pipe_one_read = fd[0];
  int pipe_one_write = fd[1];
  if(pipe(fd) < 0){
    fprintf(2, "pingpong: pipe failed\n");
    exit(1);
  }
  int pipe_two_read = fd[0];
  int pipe_two_write = fd[1];

  int pid = fork();
  if(pid < 0){
    fprintf(2, "pingpong: fork failed\n");
    exit(1);
  }
  if (pid > 0) {
    // Parent process
    int fdr = pipe_one_read; // Read end of first pipe
    int fdw = pipe_two_write; // Write end of second pipe
    char ch = 'p';
    write(fdw, &ch, 1); // Send initial ping
    if(read(fdr, &ch, 1) == 1) {
      fprintf(2, "%d: received pong\n", getpid());
    }
  } else {
    // Child process
    int fdr = pipe_two_read; // Read end of second pipe
    int fdw = pipe_one_write; // Write end of first pipe
    char ch;
    if(read(fdr, &ch, 1) == 1) {
      fprintf(2, "%d: received ping\n", getpid());
      write(fdw, &ch, 1); 
    }
  }

  exit(0);
}
