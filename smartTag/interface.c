#include "interface.h"
#include "hardware.h"

void initialize(void)
{
  HwInit();
}

void light(char _val)
{
  SetPcbPC3(_val);
}

char button(void)
{
  return GetPcbPD4();
}

char readTag(unsigned char *_ar)
{
  return GetUart1(_ar);
}

void u_printf(unsigned char *_ar, unsigned char _size)
{
  SendUart1(_ar, _size);
}