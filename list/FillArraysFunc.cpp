#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

static const char * POISON = "poison";

void FillDataWithPoison (List_t * lst, int start)
{
    assert(lst);
    assert(lst->data);

    int capacity = GetCapacity(lst);

    for (int count = start; count < capacity; count++) // [0] is for CANARY
    {
        (lst->data)[count] = POISON;
    }
}


void FillNextWithFreeAddresses (List_t * lst, int start)
{
    assert(lst);
    assert(lst->next);

    int capacity = GetCapacity(lst);

    if (start != 1)
        lst->free = start;

    for (int count = start; count < capacity - 1; count++) // [0] is for head, next for [START_SIZE - 1] is 0
    {
        (lst->next)[count] = count + 1; // address of the next free cell
    }
    (lst->next)[capacity - 1] = 0;

}


void FillPrevWithStartValue (List_t * lst, int start)
{
    assert(lst);
    assert(lst->prev);

    int capacity = GetCapacity(lst);

    for (int count = start; count < capacity; count++) // starting from el connected with [1] el of data
    {
        (lst->prev)[count] = FREE_ADDRESS_INDICATOR;
    }
}


