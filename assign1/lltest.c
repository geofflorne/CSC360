#include <stdio.h>              // printf(), scanf(), setbuf(), perror()
#include <stdlib.h>             // malloc()
#include <string.h>
#include "proc.h"

int main(){
  proc *ptr = NULL;
  print();
  add(123, "hello",0);
  print();
  delete(123);
  print();
  return 0;
}
