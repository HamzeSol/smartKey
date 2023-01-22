#include "interface.h"
#include "hardware.h"

void Initialize(void)
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
void Buzz(char _val)
{
  SetPcbPD3(_val);
}
void RelayTimer(char _val)
{
  //SetPcbPD2(_val);
  SetPcbPD2Time(_val);
}

char Button(void)
{
  return GetPcbPD4();
}

int ButtonTime(void)
{
  return GetPcbPD4Time();
}

char ReadTag(unsigned char *_ar)
{
  return GetUart1(_ar);
}

void ResetTagUart(void)
{
  Uart1BufferReset();
}

void u_printf(unsigned char *_ar, unsigned char _size)
{
  SendUart1(_ar, _size);
}

void u_Sprintf(unsigned char *_ar)
{
  SendStringUart1(_ar);
}
void u_Cprintf(const unsigned char *_ar)
{
  SendConstStringUart1(_ar);
}
void u_intPrintf(int _val)
{
  SendIntUart1(_val);
}
