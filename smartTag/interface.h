void Initialize(void);

void SaveMemoryByte(unsigned char _index, unsigned char _data);
unsigned char ReadMemoryByte(unsigned char _index);

void Light(char _val);
char Button(void);
char ReadTag(unsigned char *_ar);

void u_printf(unsigned char *_ar, unsigned char _size);