//
//  BasicBitOps.c
//  libalgorithm
//
//  Created by sshe on 15/12/28.
//  Copyright © 2015年 sshe. All rights reserved.
//

#define ROUND_TO_ZERO
//#define NON_NEGATIVE_NUMBERS

unsigned int pop_count(unsigned int v)
{
    v = v - ((v >> 1) & 0x55555555); // count each 2 bits
    v = (v & 0x33333333) + ((v >> 2)& 0x33333333);  // count each 4 bits
    v = (v & 0x0f0f0f0f) + ((v >> 4) & 0x0f0f0f0f); // count each 8 bits
    
    v = (v * 0x01010101) >> 24; // sum up all 8-bit counters
    return v;
}

int average_signed_int(int a, int b)
{
#ifdef ROUND_TO_ZERO
#ifdef NON_NEGATIVE_NUMBERS
    return (int)(((unsigned int)(a + b)) >> 1);
#else
    int t = ((a & b) + ((a ^ b) >> 1));
    return (t + ((unsigned int)t >> 31 & (a ^ b)));
#endif
#else
    return ((a & b) + ((a ^ b) >> 1));
#endif
}

unsigned int average_unsigned_int(unsigned int a, unsigned int b)
{
    return ((a & b) + ((a ^ b) >> 1));
}