#ifndef HASHTABLE_H
#define HASHTABLE_H

const int WORD_LEN_32 = 32;

struct Hash_t
{
    List_t * data;
    unsigned int (* hashfunc) (const char *, int);
    int size;
};

struct StringBuffer
{
    const char ** data;
    int size;
};

struct CharBuffer
{
    char * data;
    int size;
};

StringBuffer WordBuffInit (CharBuffer buffer, int num_of_words);
void ReworkBuffer         (CharBuffer buffer, char sym_to_find, char sym_to_put_instead);
int InsertWords           (Hash_t hashtable, StringBuffer word_buff);
int CountWords            (CharBuffer buffer);
void CopyFileToBuff       (CharBuffer buffer, FILE * fp);
CharBuffer CharBuffInit   (char * filename);

Hash_t HashtableInit      (unsigned int (* hashfunc) (const char *, int), int size, FILE * fp, FILE * log);
void HashtableDtor        (Hash_t hashtable);

ListErr_t HashtableDump   (Hash_t hashtable);
int MakeHashTCodeFile     (Hash_t hashtable);
char * GetHashdumpCommand (char * pic_name);

void MakeHistogram        (Hash_t hashtable);
bool FindWord             (Hash_t hashtable, const char * word);

int MyStrcmp              (const char * string1, const char * string2);
int CalcMaxWordSize       (const char * buffer, int num_of_words);
int GetHashtableSize      (Hash_t hashtable);

const char * MakePlotDataFile   (Hash_t hashtable);
char * MovePtrToNextWord        (char * curr_sym);
void CopyWordsTo64SizedBuff     (CharBuffer buffer, char * sized64_word_buff, int num_of_words);
void InsertToHashtable          (Hash_t hashtable, const char * word);

#endif // HASHTABLE_H