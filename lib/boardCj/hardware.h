#include "stm8s.h"
enum LedStatus
{
    LED_OFF = 0,
    LED_ON,
    LED_OK,
    LED_ERROR,
    LED_REG_USER,
    LED_REG_MASTER

};
void HwInit(void);

void EEpromWriteByte(uint32_t _address, uint8_t _data);
uint8_t EEpromReadByte(uint32_t _address);

void SetPcbPC3(char _val);
char GetPcbPD4(void);

void SendUart1(unsigned char *_ar, unsigned char _size);
char GetUart1(unsigned char *_ar);

void Uart1RXIRQcallback(unsigned char _val);
void Uart1TXIRQcallback();

void Delay(__IO uint32_t _val);
void LedStatus(char _val);
void TIM4_callBackIrq(void);