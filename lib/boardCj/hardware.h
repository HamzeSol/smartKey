#include "stm8s.h"

extern int second;
extern int repeatCounter;
extern int holdCounter;

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
void EEpromWriteArray(uint32_t _address, uint8_t *_data, uint8_t _size);
void EEpromWriteByte(uint32_t _address, uint8_t _data);
void EEpromReadArray(uint32_t _address, uint8_t *_data, uint8_t _size);
uint8_t EEpromReadByte(uint32_t _address);

void harwareExecute(void);
void SetPcbPC3(char _val);
void SetPcbPD3(char _val);
void SetPcbPD2(char _val);
char GetPcbPD4(void);
int GetPcbPD4Time(void);

void SendUart1(unsigned char *_ar, unsigned char _size);
void SendConstStringUart1(const unsigned char *_ar);
void SendStringUart1(unsigned char *_ar);
void SendIntUart1(int _val);
void Uart1BufferReset(void);

char GetUart1(unsigned char *_ar);

void Uart1RXIRQcallback(unsigned char _val);
void Uart1TXIRQcallback();

void Delay(__IO uint32_t _val);
void LedStatus(char _val);
void TIM4_callBackIrq(void);