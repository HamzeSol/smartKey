/**
 ******************************************************************************
 * @file    interface.c
 * @author  h.soleimanipour@gmail.com
 * @version V1
 * @date    30-Jan-2023
 * @brief   This file is an adapter or interface for connect user layer to the hardware layer
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "interface.h"
#include "hardware.h"

/// @brief Initialize hardware
void Initialize(void)
{
  HwInit();
}

// void SaveMemoryByte(unsigned char _index, unsigned char _data)
// {
//   if ((_index >= 0) && (_index < 128))
//   {
//     EEpromWriteByte((0x4000 + _index), _data);
//   }
// }

// unsigned char ReadMemoryByte(unsigned char _index)
// {
//   if ((_index >= 0) && (_index < 128))
//   {
//     return FLASH_ReadByte((0x4000 + _index));
//   }
//   else
//     return (0);
// }

/// @brief Change LED state
/// @param _val 0: OFF 1:ON
void Light(char _val)
{
  SetPcbPC3(_val);
}

/// @brief Enable buzzer
/// @param _val 0: OFF 1:ON
void Buzz(char _val)
{
  SetPcbPD3(_val);
}

/// @brief Enable a Relay and disable it after _val time
/// @param _val Time for Relay ON
void RelayTimer(char _val)
{
  //  SetPcbPD2(_val);
  SetPcbPD2Time(_val);
}

/// @brief Check state of button
/// @return
char Button(void)
{
  return GetPcbPD4();
}

/// @brief Get time of pushed button
/// @return time of pushed button in integer
int ButtonTime(void)
{
  return GetPcbPD4Time();
}

/// @brief RFID tag module sends an array of data when it detect a tag, By this function we can read this data
/// @param _ar Pointer to register, fills with received data from RFID module
/// @return   0: No data , 1: there is new data
char ReadTag(unsigned char *_ar)
{
  return GetUart1(_ar);
}

/// @brief Clear buffer of UART RX
void ResetTagUart(void)
{
  Uart1BufferReset();
}

/// @brief Print specified size array
/// @param _ar Pointer to array for print
/// @param _size Size of array for print
void u_printf(unsigned char *_ar, unsigned char _size)
{
  SendUart1(_ar, _size);
}

/// @brief Print string on UART
/// @param _ar Pointer of array to print
void u_Sprintf(unsigned char *_ar)
{
  SendStringUart1(_ar);
}
void u_Cprintf(const unsigned char *_ar)
{
  SendConstStringUart1(_ar);
}

/// @brief Print an integer on UART
/// @param _val int value to print
void u_intPrintf(int _val)
{
  SendIntUart1(_val);
}
