#include "cFunc.h"
#define RING_BUFFER_NUMBER 2

unsigned char ringBufferId = 0;

enum ringBufferParam
{
    RING_COUNTER = 0,
    RING_WRITE,
    RING_READ,
    RING_SIZE,
    RING_PARAM_SIZE
};

int *ringPtrAr[RING_BUFFER_NUMBER];
int ringParam[RING_BUFFER_NUMBER][RING_PARAM_SIZE];


char RingBuffer8Create(unsigned int _size)
{
    if (ringBufferId < RING_BUFFER_NUMBER)
    {
        ringPtrAr[ringBufferId] = (int *)malloc(_size * sizeof(unsigned char));
        ringParam[ringBufferId][RING_SIZE] = _size;
        ringParam[ringBufferId][RING_WRITE] = 0;
        ringParam[ringBufferId][RING_READ] = 0;
        ringParam[ringBufferId][RING_COUNTER] = 0;

        ringBufferId++;
        return (ringBufferId - 1);
    }
    return -1;
}

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
