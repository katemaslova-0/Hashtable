#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>

#include "list/Data.h"
#include "HashfuncData.h"
#include "HashtableData.h"

unsigned int SimpleHashfunc (const char * word, int hashtable_size)
{
    assert(word);

    return 0;
}


unsigned int FirstAsciiHashFunc (const char * word, int hashtable_size)
{
    assert(word);

    return (unsigned int) word[0];
}


unsigned int WordLenHashFunc (const char * word, int hashtable_size)
{
    assert(word);

    return (unsigned int) strlen(word);
}


unsigned int SumAsciiHashFunc (const char * word, int hashtable_size)
{
    assert(word);

    unsigned int sum = CalculateAsciiSum(word);

    return sum % hashtable_size;
}


unsigned int LeftShiftNXorHashFunc (const char * word, int hashtable_size)
{
    assert(word);

    unsigned int current_value = (unsigned int) word[0];
    int len = (int) strlen(word);

    for (int i = 0; i < len - 1; i++)
    {
        current_value = (current_value << 1) | (current_value >> 31);
        current_value = current_value ^ (unsigned int) word[i + 1];
    }
    
    return current_value % hashtable_size;
}


unsigned int RightShiftNXorHashFunc (const char * word, int hashtable_size)
{
    unsigned int current_value = (unsigned int) word[0];
    int len = (int) strlen(word);

    for (int i = 0; i < len - 1; i++)
    {
        current_value = (current_value >> 1) | (current_value << 31);
        current_value = current_value ^ (unsigned int) word[i + 1];
    }
    
    return current_value % hashtable_size;
}


unsigned int Crc32 (const char * word, int hashtable_size)
{
    // unsigned int res = 0;
    // unsigned int len = (unsigned int) strlen (word);
// 
    // while (len--)
    // {
    //     res ^= (unsigned int) *word;
    //     word++;
    //     for (int i = 0; i < 8; i++)
    //         res = (res >> 1) ^ ((res & 1) ? 0xedb88320 : 0);
    // }
// 
    // return res % hashtable_size;

    unsigned int res = 0xFFFFFFFF;
    const unsigned int * ptr = (const unsigned int *) word;

    // for (int i = 0; i < 8; i++)
    //     res = _mm_crc32_u32(res, ptr[i]);

    asm volatile (".intel_syntax noprefix;\n"
        "crc32 %0, DWORD PTR [%1];\n"
        "crc32 %0, DWORD PTR [%1+4];\n"
        "crc32 %0, DWORD PTR [%1+8];\n"
        "crc32 %0, DWORD PTR [%1+12];\n"
        "crc32 %0, DWORD PTR [%1+16];\n"
        "crc32 %0, DWORD PTR [%1+20];\n"
        "crc32 %0, DWORD PTR [%1+24];\n"
        "crc32 %0, DWORD PTR [%1+28];\n"
        ".att_syntax prefix;\n"
        : "+r" (res) 
        : "r"  (ptr)  
    );
 
    return res % hashtable_size;
}


unsigned int CalculateAsciiSum (const char * word)
{
    assert(word);

    unsigned int res = 0;

    for (const char * current = word; *current != '\0'; current++)
        res += (unsigned int) *current;

    return res;
}