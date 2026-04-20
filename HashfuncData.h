#ifndef HASH_H
#define HASH_H

#include "HashtableData.h"

unsigned int SimpleHashfunc         (const char * word, int hashtable_size);
unsigned int FirstAsciiHashFunc     (const char * word, int hashtable_size);
unsigned int WordLenHashFunc        (const char * word, int hashtable_size);
unsigned int SumAsciiHashFunc       (const char * word, int hashtable_size);
unsigned int LeftShiftNXorHashFunc  (const char * word, int hashtable_size);
unsigned int RightShiftNXorHashFunc (const char * word, int hashtable_size);
unsigned int Crc32                  (const char * word, int hashtable_size);

unsigned int CalculateAsciiSum      (const char * word);

#endif // HASH_H