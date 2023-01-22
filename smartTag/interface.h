void Initialize(void);

void SaveMemoryByte(unsigned char _index, unsigned char _data);
unsigned char ReadMemoryByte(unsigned char _index);

void Light(char _val);
void Buzz(char _val);
void RelayTimer(char _val);
char Button(void);
int ButtonTime(void);
char ReadTag(unsigned char *_ar);
void ResetTagUart(void);

void u_printf(unsigned char *_ar, unsigned char _size);
void u_Sprintf(unsigned char *_ar);
void u_Cprintf(const unsigned char *_ar);
void u_intPrintf(int _val);