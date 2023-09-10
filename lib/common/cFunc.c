/**
 ******************************************************************************
 * @file    cFunc.h
 * @author  h.soleimanipour@gmail.com
 * @version V1
 * @date    30-Jan-2023
 * @brief   Consist of some useful function in C language
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "cFunc.h"
#define RING_BUFFER_NUMBER 2

/* Private function prototypes -----------------------------------------------*/
volatile unsigned char ringBufferId = 0;
volatile int *ringPtrAr[RING_BUFFER_NUMBER];
volatile int ringParam[RING_BUFFER_NUMBER][RING_PARAM_SIZE];

/// @brief Parameter for managing ring buffer
enum ringBufferParam
{
    RING_COUNTER = 0,
    RING_WRITE,
    RING_READ,
    RING_SIZE,
    RING_PARAM_SIZE
};


/// @brief Reset buffer of ring buffer
/// @param _id Reset desired buffer
void RingBuffer8Reset(unsigned char _id)
{
    ringParam[_id][RING_WRITE] = 0;
    ringParam[_id][RING_READ] = 0;
    ringParam[_id][RING_COUNTER] = 0;
}

/// @brief Allocate a memory and assign a an ID to a ring buffer
/// @param _size Size of ring buffer
/// @return -1: Error >=0 : ID of created ring buffer
char RingBuffer8Create(unsigned int _size)
{
    if (ringBufferId < RING_BUFFER_NUMBER)
    {
        ringPtrAr[ringBufferId] = (volatile int *)malloc(_size * sizeof(unsigned char));
        ringParam[ringBufferId][RING_SIZE] = _size;
        ringParam[ringBufferId][RING_WRITE] = 0;
        ringParam[ringBufferId][RING_READ] = 0;
        ringParam[ringBufferId][RING_COUNTER] = 0;

        ringBufferId++;
        return (ringBufferId - 1);
    }
    return -1;
}

/// @brief Write in ring buffer
/// @param _id Write in _id ring buffer
/// @param _val Write _val in ring buffer
/// @return 0: successfull
char RingBuffer8Write(unsigned char _id, unsigned char _val)
{
    if (ringParam[_id][RING_COUNTER] < ringParam[_id][RING_SIZE])
    {
        //(unsigned char)ringPtrAr[_id][(ringParam[_id][RING_WRITE])] = _val;
        *(ringPtrAr[_id] + (int)(ringParam[_id][RING_WRITE])) = _val;

        ringParam[_id][RING_COUNTER]++;
        ringParam[_id][RING_WRITE]++;
        if (ringParam[_id][RING_WRITE] >= ringParam[_id][RING_SIZE])
        {
            ringParam[_id][RING_WRITE] = 0;
        }
    }
    return 0;
}

/// @brief If there is any data in ring buffer, you can get it
/// @param _id Check _id ring buffer
/// @param _val Write received data in it
/// @return 0: There is no data, 1: There is data
char RingBuffer8Read(unsigned char _id, unsigned char *_val)
{
    if (ringParam[_id][RING_COUNTER] > 0)
    {
        //*_val = (unsigned char)ringPtrAr[_id][(ringParam[_id][RING_READ])];
        *_val = *(ringPtrAr[_id] + (int)(ringParam[_id][RING_READ]));

        ringParam[_id][RING_COUNTER]--;
        ringParam[_id][RING_READ]++;

        if (ringParam[_id][RING_READ] >= ringParam[_id][RING_SIZE])
        {
            ringParam[_id][RING_READ] = 0;
        }

        return 1;
    }
    return 0;
}

/////////-----------------------------------------

/// @brief Convert HEX format to integer
/// @param _hex HEX format
/// @return Result integer format
unsigned char charhexToInt(unsigned char _hex)
{

    if ((_hex >= '0') && (_hex <= '9'))
    {
        return (_hex - 48);
    }
    else if ((_hex >= 'A') && (_hex <= 'F'))
    {
        return (_hex - 55);
    }
    else if ((_hex >= 'a') && (_hex <= 'f'))
    {
        return (_hex - 87);
    }
}