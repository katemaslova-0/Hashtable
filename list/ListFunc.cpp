#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "Data.h"

static const char * CANARY = "canary";

int ListCtor (List_t * lst, FILE * fp)
{
    assert(lst);
    assert(fp);

    lst->data = (const char **) calloc (START_SIZE, sizeof(const char *)); //func
    lst->next = (int *) calloc (START_SIZE, sizeof(int));
    lst->prev = (int *) calloc (START_SIZE, sizeof(int));

    lst->log = fp;                  // logfile

    (lst->data)[0] = CANARY;
    (lst->next)[0] = 0;             // head
    (lst->prev)[0] = 0;             // tail

    lst->free = 1;                  // the first free element of array
    lst->capacity = START_SIZE;
    lst->num_of_elems = 0;

    FillDataWithPoison(lst, 1);
    FillNextWithFreeAddresses(lst, 1);
    FillPrevWithStartValue(lst, 1);

    LIST_VERIFY(lst, After, 0, 0); // fix

    return NoError;
}


int ListVerify (List_t * lst)
{
    assert(lst);

    int error = NoError;

    if ((error = CheckNullPointers(lst)) != NoError)
        return error;
    if ((error = CheckCanaryValue(lst)) != NoError)
        return error;
    if ((error = CheckIfIndexesValid(lst)) != NoError)
        return error;
    if ((error = CheckNextAndPrevConnections(lst)) != NoError)
        return error;
    if ((error = CheckIfCycles(lst)) != NoError)
        return error;

    return error;
}


ListErr_t GraphDump (List_t * lst, Place_t place, const char * func_name, const char * file_name, int num_of_line, int addr, const char * elem, int result, ...)
{
    assert(lst);
    assert(func_name);
    assert(file_name);

    va_list args = {};
    va_start(args, result);

    MakeGraphCodeFile(lst);

    char * pic_name = GetPicName();
    char * command  = GetCommand(pic_name);
    const char * place_of_call = GetPlace(place);
    FILE * log = GetLog(lst);

    if (system(command) != 0)
        return SystemError;
    free(command);

    OutputTitle(log, place_of_call, func_name, file_name, num_of_line, addr, elem, result);
    const char * string = va_arg(args, const char *);
    if (string != NULL)
        fprintf(log, "\nПричина вызова: %s\n\n", string);
    va_end(args);
    OutputIndexes(log, lst);
    OutputData(log, lst);
    OutputNext(log, lst);
    OutputPrev(log, lst);
    OutputFree(log, lst);
    OutputImage(log, lst, pic_name);

    free(pic_name);
    return NoError;
}


void MakeGraphCodeFile (List_t * lst)
{
    assert(lst);

    FILE * fp = fopen("graphdump.txt", "w");
    assert(fp); //FIXME: check

    OutputNullCell         (fp, lst);
    OutputArrayCellsNodes  (fp, lst);
    OutputHeadAndTailNodes (fp, lst);
    OutputAllEdges         (fp, lst);
    OutputArrayCellsEdges  (fp, lst);
    OutputHeadAndTailEdges (fp, lst);

    fclose(fp);
}


ListErr_t ListRealloc (List_t * lst)
{
    assert(lst);

    lst->capacity *= 2;
    lst->data = (const char **) realloc (lst->data, ((size_t) lst->capacity * sizeof(const char *)));
    lst->next = (int *) realloc (lst->next, ((size_t) lst->capacity * sizeof(int)));
    lst->prev = (int *) realloc (lst->prev, ((size_t) lst->capacity * sizeof(int)));

    if (lst->data == NULL || lst->next == NULL || lst->prev == NULL)
        return MemAllocError;

    FillDataWithPoison(lst, lst->capacity / 2);
    FillPrevWithStartValue(lst, lst->capacity / 2);
    FillNextWithFreeAddresses(lst, lst->capacity / 2);

    return NoError;
}


void ListDtor (List_t * lst)
{
    assert(lst);

    free(lst->data);
    free(lst->next);
    free(lst->prev);
}

