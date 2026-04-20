#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

#include <immintrin.h>

#include "list/Data.h"
#include "HashfuncData.h"
#include "HashtableData.h"


Hash_t HashtableInit (unsigned int (* hashfunc) (const char *, int), int size, FILE * fp, FILE * log)
{
    assert(fp);
    assert(log);
    assert(hashfunc);

    Hash_t hashtable = {};

    hashtable.data = (List_t *) calloc (size, sizeof(List_t));
    if (!hashtable.data)
    {
        printf("Error while allocating buffer\n");
        return hashtable;
    }

    for (int i = 0; i < size; i++)
        ListCtor(&hashtable.data[i], log);

    hashtable.hashfunc = hashfunc;
    hashtable.size = size;

    return hashtable;
}


void HashtableDtor (Hash_t hashtable)
{
    int hashtable_size = GetHashtableSize(hashtable);

    for (int count = 0; count < hashtable_size; count++)
        ListDtor(&hashtable.data[count]);

    free(hashtable.data);
}


ListErr_t HashtableDump (Hash_t hashtable)
{
    int hashtable_size = GetHashtableSize(hashtable);

    if (hashtable_size >= 100)
    {
        printf("No graphdump(too many elements)\n");
        return NoError;
    }

    int res = MakeHashTCodeFile(hashtable);
    if (!res) return FileOpeningError;

    char * picture_name = GetPicName();
    if (!picture_name)
        return MemAllocError;
    
    char * hashdump_cmd  = GetHashdumpCommand(picture_name);
    if (!hashdump_cmd)
        return MemAllocError;

    if (system(hashdump_cmd) != 0)
        return SystemError;

    free(hashdump_cmd);
    free(picture_name);

    return NoError;
}


void MakeHistogram (Hash_t hashtable)
{
    int hashtable_size = GetHashtableSize(hashtable);

    const char * plot_data_filename = MakePlotDataFile(hashtable);

    FILE * fp = fopen("plot.gp", "w");

    fprintf(fp, "# plot.gp\nset terminal pngcairo size 800,600 enhanced\nset output 'histogram.png'\n");
    fprintf(fp, "set title \"Распределение количества слов в ячейках хеш-таблицы\" font \",14\"\n");
    fprintf(fp, "set ylabel \"Количество слов\" font \",12\"\nset xlabel \"Номер ячейки\" font \",12\"\n");
    fprintf(fp, "set style data histogram\nset style histogram cluster gap 1\nset border 3\nunset key\n");
    fprintf(fp, "set style fill solid 1.0\nset boxwidth 0.9\nset grid y\nset yrange [0:200]\nset xrange [0:4096]\n");
    fprintf(fp, "unset x2tics\nunset y2tics\nset ytics nomirror\nset xtics nomirror\n");
    fprintf(fp, "plot '%s' using 1:2 with boxes lc rgb \"steelblue\"\nset xtics 0,50, 200\n", plot_data_filename);

    fclose(fp);

    system("gnuplot plot.gp");
}


const char * MakePlotDataFile (Hash_t hashtable)
{
    const char * filename = "histogram.dat";
    
    FILE * plot_data = fopen(filename, "w");
    if (!plot_data)
    {
        printf("File opening error\n");
        return NULL;
    }
    
    int hashtable_size = GetHashtableSize(hashtable);

    for (int count = 0; count < hashtable_size; count++)
        fprintf(plot_data, "%d %d\n", count, hashtable.data[count].num_of_elems);

    fclose(plot_data);

    return filename;
}


char * GetHashdumpCommand (char * pic_name)
{
    assert(pic_name);

    char * command = (char *) calloc (SIZE_OF_COMMAND, sizeof(char));
    if (!command)
    {
        printf("Error while allocating memory\n");
        return NULL;
    }

    sprintf(command, "gvpack -u hashdump.txt | dot -Tpng -o %s", pic_name);

    return command;
}


int MakeHashTCodeFile (Hash_t hashtable)
{
    int hashtable_size = GetHashtableSize(hashtable);

    FILE * fp = fopen("hashdump.txt", "w");
    if (!fp)
    {
        printf("File opening error\n");
        return 0;
    }

    for (int count = 0; count < hashtable_size; count++)
    {
        List_t * cur_lst = &hashtable.data[count];
        OutputNullCell         (fp, cur_lst);
        OutputArrayCellsNodes  (fp, cur_lst);
        OutputHeadAndTailNodes (fp, cur_lst);
        OutputAllEdges         (fp, cur_lst);
        OutputArrayCellsEdges  (fp, cur_lst);
        OutputHeadAndTailEdges (fp, cur_lst);
    }

    fclose(fp);
    return 1;
}


StringBuffer WordBuffInit (CharBuffer buffer, int num_of_words)
{
    StringBuffer word_buff = {};
    
    char * sized32_word_buff = (char *) calloc ((size_t) num_of_words, WORD_LEN_32);
    if (!sized32_word_buff)
    {
        printf("Error while allocating memory\n");
        return word_buff;
    }

    char * buff_ptr = sized32_word_buff;
    char * curr_sym = buffer.data;

    for (int i = 0; i < num_of_words; i++)
    {
        if (32 < strlen(curr_sym))
        {
            num_of_words--;
            buff_ptr += WORD_LEN_32;
            curr_sym = MovePtrToNextWord(curr_sym);
            continue;
        }
        strcpy(buff_ptr, curr_sym);
        buff_ptr += WORD_LEN_32;
        curr_sym = MovePtrToNextWord(curr_sym);
    }


    word_buff.data = (const char **) calloc ((size_t) num_of_words, sizeof(const char *));
    if (!word_buff.data)
    {
        printf("Error while allocating memory\n");
        return word_buff;
    }

    word_buff.size = num_of_words;
    const char * curr_word = sized32_word_buff;

    for (int count = 0; count < num_of_words; count++)
    {
        word_buff.data[count] = curr_word;
        curr_word += WORD_LEN_32;
    }

    return word_buff;
}


char * MovePtrToNextWord (char * curr_sym)
{
    while (*curr_sym != '\0')
            curr_sym++;
        curr_sym++;

    return curr_sym;
}


void ReworkBuffer (CharBuffer buffer, char sym_to_find, char sym_to_put_instead)
{
    assert(buffer.data);

    int count = 0;

    while (count < buffer.size - 1)
    {
        if (*(buffer.data + count) == sym_to_find)
        {
            *(buffer.data + count) = sym_to_put_instead;
        }
        count++;
    }

    *(buffer.data + count) = '\0';
}


int InsertWords (Hash_t hashtable, StringBuffer word_buff)
{
    int word_buffer_size = word_buff.size;

    for (int count = 0; count < word_buffer_size; count++)
    {
        const char * word = word_buff.data[count];
        InsertToHashtable(hashtable, word);
    }

    return NoError;
}


void InsertToHashtable (Hash_t hashtable, const char * word)
{
    unsigned int hash = hashtable.hashfunc(word, hashtable.size);
    List_t lst = hashtable.data[hash];
    int num_of_elems = lst.num_of_elems;

    int i = 0;
    for (; i < num_of_elems; i++)
    {
        if (strcmp(word, lst.data[i]) == 0)
            break;
    }
    
    if (i != num_of_elems)
        return;

    InsertToTail(&hashtable.data[hash], word);
}


bool FindWord (Hash_t hashtable, const char * word) 
{                                                     
    int hashtable_size = GetHashtableSize(hashtable);

    unsigned int hash = hashtable.hashfunc(word, hashtable_size);
    List_t lst = hashtable.data[hash];
    int num_of_elems = hashtable.data[hash].num_of_elems;

    for (int count = 0; count < num_of_elems; count++)
    {
        if (MyStrcmp(lst.data[count], word) == 0)
            return true;
    }

    return false;
}


int MyStrcmp (const char * string1, const char * string2)
{
    __m256i a = _mm256_loadu_si256((const __m256i *) string1);
    __m256i b = _mm256_loadu_si256((const __m256i *) string2);
    
    __m256i cmp = _mm256_cmpeq_epi8(a, b);
    int symbols_equ_mask = _mm256_movemask_epi8(cmp);

    return ~symbols_equ_mask;
}


int CountWords (CharBuffer buffer)
{
    int word_counter = 0;
    int count = 0;
    
    for (char * current_sym = buffer.data; count < buffer.size; current_sym++, count++)
    {
        if (*current_sym == ' ')
            word_counter++;
    }

    word_counter++;

    return word_counter;
}


void CopyFileToBuff (CharBuffer buffer, FILE * fp)
{
    assert(buffer.data);
    assert(fp);

    int res = fread(buffer.data, sizeof(char), (size_t)(buffer.size), fp);
    if (!res) printf ("Error: no symbols has been read.\n");
}


CharBuffer CharBuffInit (char * filename)
{
    assert(filename);

    struct stat st = {};
    stat(filename, &st);
    int size_of_buffer = (int)st.st_size + 1;

    CharBuffer buffer = {};
    
    buffer.size = size_of_buffer;
    buffer.data = (char *) calloc ((size_t) size_of_buffer, sizeof(char));

    return buffer;
}


int CalcMaxWordSize (const char * buffer, int num_of_words)
{
    int maxlen = 0;
    int index = 0;

    for (int i = 0; i < num_of_words; i++)
    {
        int next = strlen(buffer + i);
        if (next > maxlen)
        {
            maxlen = next;
            index = i;
        }
    }

    return maxlen;
}


int GetHashtableSize (Hash_t hashtable)
{
    return hashtable.size;
}