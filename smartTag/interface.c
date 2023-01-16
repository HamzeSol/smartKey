#include "interface.h"
#include "board.h"




void initialize(void)
{
  hwInit();
}

void light(char _val)
{
  set_PCB_PC3(_val);
}

char button(void)
{
  return get_PCB_PD4();
}