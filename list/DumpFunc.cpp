#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

void OutputTitle (FILE * log, const char * place, const char * func_name, const char * file_name, int num_of_line, int addr, const char * elem, int result)
{
    assert(log);
    assert(place);
    assert(func_name);
    assert(file_name);

    if (strcmp(func_name, "ListCtor") == 0 || strcmp(func_name, "main") == 0)
        fprintf(log, "<pre>\n<h3> DUMP <font color=red>%s</font> %s", place, func_name);
    else
    {
        fprintf(log, "<pre>\n<h3> DUMP <font color=red>%s</font> %s %d", place, func_name, addr);
        fprintf(log, " (element = \"%s\") ", elem);
    }

    if (result == SUCCESS)
        fprintf(log, " <font color=green>(SUCCESS)<font color=black></h3>\n");
    else
        fprintf(log, " <font color=red>(ERROR)<font color=black></h3>\n");
    fprintf(log, "Verified at: %s:%d\n\n\n     ", file_name, num_of_line);
}


void OutputIndexes (FILE * log, List_t * lst)
{
    assert(lst);
    assert(log);

    int capacity = GetCapacity(lst);
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", count);
}


void OutputData (FILE * log, List_t * lst)
{
    assert(lst);
    assert(lst->data);
    assert(log);

    int capacity = GetCapacity(lst);
    fprintf(log, "\n\ndata:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5s", (lst->data)[count]);
}


void OutputNext (FILE * log, List_t * lst)
{
    assert(lst);
    assert(lst->next);
    assert(log);

    int capacity = GetCapacity(lst);
    fprintf(log, "\n\nnext:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->next)[count]);
}


void OutputPrev (FILE * log, List_t * lst)
{
    assert(lst);
    assert(lst->prev);
    assert(log);

    int capacity = GetCapacity(lst);
    fprintf(log, "\n\nprev:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->prev)[count]);
}


void OutputFree (FILE * log, List_t * lst)
{
    assert(lst);
    assert(log);

    fprintf(log, "\n\nfree: %d\n", lst->free);
}


void OutputImage (FILE * log, List_t * lst, char * pic_name)
{
    assert(lst);
    assert(log);
    assert(pic_name);

    fprintf(log, "\nIMAGE:\n\n <img src=%s width=1600px>\n\n\n", pic_name);
}
