//
//  RandomInRange.c
//  libalgorithm
//
//  Created by sshe on 12-10-3.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int random_in_range(int start, int end)
{
    unsigned int seed;
    long random_value;
    unsigned int range = end - start + 1;
    FILE *random_fp;
    
    if (range == 0)
        return start;
    
    random_fp = fopen("/dev/random", "r");
    if (!random_fp){
        perror("Cannot open random");
        return -1;
    }
    
    fread(&seed, sizeof(unsigned int), 1, random_fp);
    fclose(random_fp);
    srandom(seed);
    random_value = random();
    random_value = random_value % range + start;
    
    return random_value;
}

long int random_in_range_with_seed(int start, int end, unsigned int seed)
{
    long random_value;
    unsigned int range = end - start + 1;
    
    if (range == 0)
        return start;
    
    srandom(seed);
    random_value = random();
    random_value = random_value % range + start;
    
    return random_value;
}

void randomize_in_place(int *keys, int p, int r)
{
    int len = r - p + 1;
    int key_idx, tmp;
    unsigned int *seed, range;
    long random_value;
    FILE *random_fp;
    
    if (len <= 1)
        return;
    /*
     * Open random device to get seed
     */
    random_fp = fopen("/dev/random", "r");
    if (!random_fp){
        perror("Cannot open random");
        return;
    }
    
    seed = malloc(len * sizeof(unsigned int));
    if (!seed){
        perror("Allocate seed array");
        fclose(random_fp);
        return;
    }
    
    fread(seed, len * sizeof(unsigned int), 1, random_fp);
    for (key_idx = p; key_idx < r; key_idx ++) {
        /*
         * Get a randomized index
         */
        range = r - key_idx + 1;
#if 0
        srandom(seed[key_idx - p]);
        random_value = random();
        random_value = random_value % range + key_idx;
#else
        random_value = seed[key_idx - p] % range + key_idx;
#endif
        /*
         * Swap the key value to a random position
         */
        tmp = keys[key_idx];
        keys[key_idx] = keys[random_value];
        keys[random_value] = tmp;
    }

    free(seed);
    fclose(random_fp);
}

int random_samples(int *keys, int keys_num, int *samples, int samples_num)
{
    unsigned int *seed, range;
    int key_idx, i;
    long random_value;
    FILE *random_fp;
    
    if (samples_num > keys_num || samples == 0)
        return -1;
    
    if (keys_num == samples_num){
        memcpy(samples, keys, keys_num);
        return 0;
    }
    /*
     * Open random device to get seed
     */
    random_fp = fopen("/dev/random", "r");
    if (!random_fp){
        perror("Cannot open random");
        return -1;
    }
    
    seed = malloc(samples_num * sizeof(unsigned int));
    if (!seed){
        perror("Allocate seed array");
        fclose(random_fp);
        return -1;
    }
    
    fread(seed, samples_num * sizeof(unsigned int), 1, random_fp);
    for (key_idx = 0; key_idx < samples_num; key_idx ++) {
        /*
         * Get a randomized index
         */
        range = keys_num - key_idx;
#if 0
        srandom(seed[key_idx]);
        random_value = random();
        random_value = random_value % range;
#else
        random_value = seed[key_idx] % range;
#endif
        /*
         * Get a sample
         */
        // This can be optimized
        for (i = 0; i <= key_idx; i ++) {
            if (samples[i] == keys[random_value])
                break;
        }
        
        if (i <= key_idx)
            samples[key_idx] = keys[keys_num - key_idx - 1];
        else
            samples[key_idx] = keys[random_value];
    }
    
    free(seed);
    fclose(random_fp);
    
    return 0;
}

/*
 * To fill the array with random data
 * buffer: the array to be filled
 * len: length of buffer
 */
void fill_random_int_buf(int *buffer, int len)
{
    FILE *random_fp = fopen("/dev/random", "r");
    
    if (!random_fp){
        perror("Cannot open random");
        return;
    }
    
    for (int i = 0; i < len; i ++){
        for (int j = 0; j < sizeof(int); j ++){
            unsigned char c = fgetc(random_fp);
            
            buffer[i] |= (c << (j << 3));   // value |= c << 8 * j
        }
    }
    
    fclose(random_fp);
}