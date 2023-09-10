/**
 ******************************************************************************
 * @file    cFunc.h
 * @author  h.soleimanipour@gmail.com
 * @version V1
 * @date    30-Jan-2023
 * @brief   Consist of some useful function in C language
 ******************************************************************************
 */


/* Exported functions ------------------------------------------------------- */
char RingBuffer8Create(unsigned int _size);
void RingBuffer8Reset(unsigned char _id);
char RingBuffer8Write(unsigned char _id, unsigned char _val);
char RingBuffer8Read(unsigned char _id, unsigned char *_val);

unsigned char charhexToInt(unsigned char _hex);