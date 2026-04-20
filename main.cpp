#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include "list/Data.h"
#include "HashfuncData.h"
#include "HashtableData.h"

// extern "C" unsigned int SumAsciiHashFunc (const char * word, int hashtable_size);

const long long unsigned int NUM_OF_SEARCHES = 100000000;

int main (int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("No filename!\n");
        return 0;
    }
    
    char * filename = argv[1];
    FILE * fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Invalid filename\n");
        return -1;
    }

    FILE * log = fopen("logfile", "w");
    if (!log)
    {
        printf("Invalid filename\n");
        return -1;
    }
    
    Hash_t hashtable = HashtableInit(FirstAsciiHashFunc, 4000, fp, log);
    if (!hashtable.data) return -1;
    
    CharBuffer char_buffer = CharBuffInit(filename);
    CopyFileToBuff(char_buffer, fp);

    int num_of_words = CountWords(char_buffer);
    printf("Num of words: %d\n", num_of_words);
    ReworkBuffer(char_buffer, ' ', '\0');
    
    StringBuffer word_buffer = WordBuffInit(char_buffer, num_of_words);
    InsertWords(hashtable, word_buffer);

    double sum = 0;

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long long unsigned int count = 0; count < NUM_OF_SEARCHES; count++)
        bool res = FindWord(hashtable, "dsghsrrhf");

    clock_gettime(CLOCK_MONOTONIC, &end);

    double seconds = (end.tv_sec - start.tv_sec) + 
                     (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Total: %lf seconds\n", seconds);

    // HashtableDump(hashtable);

    MakeHistogram(hashtable);

    fclose(fp);
    fclose(log);
    free(char_buffer.data);
    free(word_buffer.data);
    HashtableDtor(hashtable);

    return 0;
}
