//
//  StringMod.c
//  libalgorithm
//
//  Created by sshe on 13-5-20.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

int str_mod(unsigned char *dividend_str, int dividend_len, int divisor)
{
    unsigned int i, dividend = 0;
    
    for (i = 0; i < dividend_len; i ++){
        dividend = (dividend << 7) + dividend_str[i];
        if (dividend >= divisor)
            dividend %= divisor;
    }
    
    return dividend;
}