void HwInit(void);
void Delay(unsigned short nCount);

void SetPcbPC3(char _val);
char GetPcbPD4(void);

void SendUart1(unsigned char *_ar, unsigned char _size);
char GetUart1(unsigned char *_ar);

void Uart1RXIRQcallback(unsigned char _val);
void Uart1TXIRQcallback();