#include <stdio.h>
#include "headerFix.h"


void f(void);
int y = 0x3924;

int
main()
{
  f();
  x = 0x2234;
  printf("x = 0x%x, y = 0x%x\n", x, y);
  return 0;
}
