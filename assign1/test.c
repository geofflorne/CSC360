#include <stdio.h>              // printf(), scanf(), setbuf(), perror()
#include <stdlib.h>             // malloc()
#include "proc.h"

int main(){
  proc *ptr = NULL;
  print();
  add(123, "hello",0);
  add(1234, "helloworld",1);
  print();
  return 0;
}
