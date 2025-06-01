#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc <= 1){
    fprintf(2, "Usage: sleep ticks\n");
    exit(0);
  }

  int time = atoi(argv[1]);
  sleep(time);
  fprintf(1, "Slept for %d ticks\n", time);
  exit(0);
}
