#include "interface.h"
#include "hardware.h"

void initialize(void)
{
  HwInit();
}

void SaveMemoryByte(unsigned char _index, unsigned char _data)
{
  if ((_index >= 0) && (_index < 128))
  {
    EEpromWriteByte((0x4000 + _index), _data);
  }
}

unsigned char ReadMemoryByte(unsigned char _index)
{
  if ((_index >= 0) && (_index < 128))
  {
    return FLASH_ReadByte((0x4000 + _index));
  }
  else
    return (0);
}

void Light(char _val)
{
  SetPcbPC3(_val);
}

char Button(void)
{
  return GetPcbPD4();
}

char ReadTag(unsigned char *_ar)
{
  return GetUart1(_ar);
}

void u_printf(unsigned char *_ar, unsigned char _size)
{
  SendUart1(_ar, _size);
}
