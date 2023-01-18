#include "interface.h"
#include "cFunc.h"

#include "stm8s.h"

#define KEY_CODE_SIZE 16
#define REG_TAG_LIST_SIZE 10


char crcCheck(unsigned char *_ar, unsigned char _crc);
char validation(unsigned char *_ar);

int main(void)
{
  initialize();
  unsigned char keyCode[KEY_CODE_SIZE];
  signed char isKey = 0;


  while (1)
  {
    // light(button());
 
    isKey = ReadTag(keyCode);
    if (isKey > 0)
    {
      u_printf(keyCode, 14);
    }

    // if(crcCheck(keyCode,keyCode[10]))//crc check
  }
  return 0;
}

// unsigned char registeredList[][]


char crcCheck(unsigned char *_ar, unsigned char _crc)
{
  return 1;
}

char validation(unsigned char *_ar)
{
  return 0;
}
