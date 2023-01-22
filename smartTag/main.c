#include "stm8s.h"
#include <stdio.h>
#include "interface.h"
#include "cFunc.h"
#include "hardware.h"
#include "mcuPinOut.h"

#define NORMAL_USER_CNT 3
#define TAG_CODE_SIZE 5
#define TAG_LIST_SIZE (NORMAL_USER_CNT + 1)

char CrcCheck(unsigned char *_ar, unsigned char _crc);
char Calidation(unsigned char *_ar);
char TagListExist();
char TagCheck(unsigned char *_ar);
void TagSave(unsigned char *_ar, char _level);
char TagDelete(unsigned char *_ar);
void TagListReset(void);
void TagEEpromToRam();
void TagRamToEEprom();

unsigned char tagList[TAG_LIST_SIZE][TAG_CODE_SIZE];
unsigned char tagBuffer[TAG_CODE_SIZE];
char machinState = 0;
char machinStateOld = 0;
unsigned char tagSaveCounter = 0;

enum mState
{
  STS_UNKNOWN = 0,
  STS_NO_REGISTER,
  STS_WAIT_FOR_TAG,
  STS_WAIT_FOR_REGISTER_USER,
  STS_WAIT_FOR_REGISTER_MASTER
};

int main(void)
{

  // char str[30];
  // signed char isKey = 0;
  unsigned char tagId = 0;
  // unsigned char tagIdOld = 0;
  // unsigned int tagIdCnt = 0;
  // unsigned int tagIdHoldCnt = 0;
  // int printCnt = 0;
  char firstTime = 0;
  Initialize();
  Buzz(0);
  Delay(100);
  Buzz(1);

  TagEEpromToRam();
  char i = 0;
  u_Cprintf("(");
  u_intPrintf(tagSaveCounter);
  u_Cprintf(")\n");
  for (i = 0; i < TAG_LIST_SIZE; i++)
  {
    u_intPrintf(i);
    u_Cprintf(":");
    u_intPrintf(tagList[i][0]);
    u_Cprintf(",");
    u_intPrintf(tagList[i][1]);
    u_Cprintf(",");
    u_intPrintf(tagList[i][2]);
    u_Cprintf(",");
    u_intPrintf(tagList[i][3]);
    u_Cprintf(",");
    u_intPrintf(tagList[i][4]);
    u_Cprintf(",\n");
    Delay(200);
  }

  Delay(2000);
  while (1)
  {
    harwareExecute();

    if (machinState != machinStateOld)
    {
      machinStateOld = machinState;
      switch (machinState)
      {
      case STS_UNKNOWN:
        u_Cprintf("STS_UNKNOWN\n");
        break;
      case STS_NO_REGISTER:
        u_Cprintf("STS_NO_REGISTER\n");
        break;
      case STS_WAIT_FOR_TAG:
        u_Cprintf("STS_WAIT_FOR_TAG\n");
        break;
      case STS_WAIT_FOR_REGISTER_USER:
        u_Cprintf("STS_WAIT_FOR_REGISTER_USER\n");
        break;
      case STS_WAIT_FOR_REGISTER_MASTER:
        u_Cprintf("STS_WAIT_FOR_REGISTER_MASTER\n");
        break;
      default:
        break;
      }
    }

    switch (machinState)
    {
    case STS_UNKNOWN:
      if (TagListExist())
      {
        machinState = STS_WAIT_FOR_TAG;
        LedStatus(LED_OFF);
      }
      else
      {
        machinState = STS_NO_REGISTER;
      }
      holdCounter = 0;
      break;
    case STS_NO_REGISTER:
      LedStatus(LED_ON);
      if (ButtonTime() > 1000)
      {
        Buzz(0);
        Delay(500);
        Buzz(1);
        machinState = STS_WAIT_FOR_REGISTER_MASTER;
        second = 0;
      }
      if (ReadTag(tagBuffer))
      {
        LedStatus(LED_OFF);
        Delay(30);
      }
      break;
    case STS_WAIT_FOR_TAG:
      if (ButtonTime() > 1000)
      {
        Buzz(0);
        Delay(500);
        Buzz(1);
        machinState = STS_WAIT_FOR_REGISTER_MASTER;
        second = 0;
        break;
      }

      tagId = TagCheck(tagBuffer);
      if (tagId > 0)
      {
        // tagIdOld = tagId;
        // tagIdCnt = 0;
        u_Cprintf("(");
        u_intPrintf(tagId);
        u_Cprintf(")");
        u_intPrintf(tagBuffer[0]);
        u_Cprintf(",");
        u_intPrintf(tagBuffer[1]);
        u_Cprintf(",");
        u_intPrintf(tagBuffer[2]);
        u_Cprintf(",");
        u_intPrintf(tagBuffer[3]);
        u_Cprintf(",");
        u_intPrintf(tagBuffer[4]);
        u_Cprintf(",\n");

        if ((tagId > 0) && (tagId < 253))
        {
          RelayTimer(2);
          LedStatus(LED_OK);
          Buzz(0);
          Delay(100);
          Buzz(1);
        }
        else if (tagId == 254)
        {
          //-----------------
          Buzz(0);
          Delay(100);
          Buzz(1);
          Delay(100);
          Buzz(0);
          Delay(100);
          Buzz(1);
          Delay(100);
          Buzz(0);
          Delay(100);
          Buzz(1);
          Delay(1000);
          machinState = STS_WAIT_FOR_REGISTER_USER;
          second = 0;
          firstTime = 1;
        }
        else if (tagId == 255)
        {
          LedStatus(LED_ERROR);
        }
      }
      break;
    case STS_WAIT_FOR_REGISTER_USER:
      LedStatus(LED_REG_USER);
      if (firstTime == 1)
      {
        firstTime = 0;
        ResetTagUart();
        // RingBuffer8Reset();
      }
      else
      {
        // TagListReset
        if (ReadTag(tagBuffer))
        {

          // u_printf(tagBuffer, 5);
          // u_intPrintf(tagBuffer[0]);
          // u_Cprintf(",");
          // u_intPrintf(tagBuffer[1]);
          // u_Cprintf(",");
          // u_intPrintf(tagBuffer[2]);
          // u_Cprintf(",");
          // u_intPrintf(tagBuffer[3]);
          u_Cprintf(",");
          u_intPrintf(tagBuffer[4]);

          u_Cprintf(",s\n");

          TagSave(tagBuffer, 2);
          Buzz(0);
          Delay(100);
          Buzz(1);

          repeatCounter = 0;
          machinState = STS_UNKNOWN;
        }

        if (second > 10)
        {
          Buzz(0);
          Delay(100);
          Buzz(1);
          machinState = STS_UNKNOWN;
        }
      }
      break;
    case STS_WAIT_FOR_REGISTER_MASTER:
      if (ButtonTime() > 5000)
      {
        Buzz(0);
        Delay(500);
        Buzz(1);
        TagListReset();
        machinState = STS_UNKNOWN;
        break;
      }

      LedStatus(LED_REG_MASTER);
      if (ReadTag(tagBuffer))
      {
        Buzz(0);
        Delay(100);
        Buzz(1);
        Delay(100);
        TagSave(tagBuffer, 1);
        // u_printf(tagBuffer, 5); // {36}{00}{F8}{85}{F0}
        Buzz(0);
        Delay(100);
        Buzz(1);
        repeatCounter = 0;
        machinState = STS_UNKNOWN;
      }

      if (second > 10)
      {
        Buzz(0);
        Delay(100);
        Buzz(1);
        machinState = STS_UNKNOWN;
      }

      break;

    default:
      break;
    }
  }

  return 0;
}
char i = 0, j = 0;
char CrcCheck(unsigned char *_ar, unsigned char _crc)
{
  return 1;
}

char Validation(unsigned char *_ar)
{
  return 0;
}

char TagListExist()
{
  char i = 0, j = 0;
  for (j = 0; j < TAG_CODE_SIZE; j++)
  {
    if (tagList[i][j] > 0)
      return 1;
  }
  return 0;
}
unsigned int idCounter = 0;
unsigned char tagIdHold = 0;

char TagCheck(unsigned char *_ar)
{
  char _correct = 0;
  char _return = 0;

  if (ReadTag(_ar))
  {
    for (i = 0; i < TAG_LIST_SIZE; i++)
    {
      _correct = 1;
      for (j = 0; j < TAG_CODE_SIZE; j++)
      {
        if (tagList[i][j] == _ar[j])
        {
        }
        else
        {
          _correct = 0;
          break;
        }
      }
      // if ((_ar[0] == 0) && (_ar[1] == 0) && (_ar[2] == 0) && (_ar[3] == 0) && (_ar[4] == 0))
      // {
      //   _correct = 0;
      // }

      if (_correct == 1)
      {
        _return = (i + 1);
        break;
      }
    }
    if (_correct != 1)
    {
      _return = 255;
    }
  }

  if (repeatCounter >= 300)
  {
    holdCounter = 0;
    tagIdHold = 0;
  }
  if ((_return > 0) && (_return < 253))
  {
    tagIdHold = _return;
  }

  if (holdCounter > 4)
  {
    holdCounter = 0;
    if (tagIdHold == 1)
    {
      // u_intPrintf(tagIdHold);
      // u_Cprintf("]");

      return 254;
    }
    else if ((tagIdHold > 1) && (tagIdHold < 253))
    {
      return 253;
    }
  }

  if (_return == 0)
  {
  }
  else
  {
    if (repeatCounter >= 900)
    {
      repeatCounter = 0;
      return _return;
    }
    repeatCounter = 0;
  }
  return 0;
}

void TagSave(unsigned char *_ar, char _level)
{

  if (_level == 1)
  {
  }
  else
  {
    _level = (tagSaveCounter + 2);
    tagSaveCounter++;
    if (tagSaveCounter >= NORMAL_USER_CNT)
    {
      tagSaveCounter = 0;
    }
  }

  j = 0;
  if (_level > 0)
  {
    _level--;
    for (j = 0; j < TAG_CODE_SIZE; j++)
    {
      tagList[_level][j] = _ar[j];
    }
  }

  TagRamToEEprom();
}

char TagDelete(unsigned char *_ar)
{
}

void TagListReset(void)
{
  i = 0;
  j = 0;
  for (i = 0; i < TAG_LIST_SIZE; i++)
  {
    for (j = 0; j < TAG_CODE_SIZE; j++)
    {
      tagList[i][j] = 0;
    }
  }
  tagSaveCounter = 0;
  TagRamToEEprom();
}

void TagEEpromToRam()
{
  char i = 0;
  EEpromReadArray(0, &tagSaveCounter, 1);
  for (i = 0; i < TAG_LIST_SIZE; i++)
  {
    EEpromReadArray(((i * TAG_CODE_SIZE) + 1), tagList[i], TAG_CODE_SIZE);
  }
}
void TagRamToEEprom()
{
  // char i = 0;
  disableInterrupts();
  UART1_ITConfig(UART1_IT_RXNE, DISABLE);
  EEpromWriteArray(0, &tagSaveCounter, 1);
  EEpromWriteArray((1), tagList[0], (TAG_CODE_SIZE * TAG_LIST_SIZE));
  (void)UART1->SR;
  (void)UART1->DR;
  UART1_ITConfig(UART1_IT_RXNE, ENABLE);
  enableInterrupts();
}