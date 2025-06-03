#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc <= 1){
    fprintf(2, "Usage: trace mask command\n");
    exit(0);
  }
  // int trace(int, const char*, char**)
  int mask = atoi(argv[1]);
  char *command = argv[2];
  char **args = argv + 2;
  printf("mask=%d, command=%s\n", mask, command);
  trace(mask, command, args);
  exit(0);
}
