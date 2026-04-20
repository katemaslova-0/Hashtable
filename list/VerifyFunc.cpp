#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

static const char * CANARY = "canary";
static const char * POISON = "poison";

/* These functions are only called inside ListVerify so there's no assert's or other null pointers check's
inside */

ListErr_t CheckNullPointers (List_t * lst)
{
    if (lst == NULL)
        return NullListPtr;
    if (lst->next == NULL)
        return NullNextPtr;
    if (lst->prev == NULL)
        return NullPrevPtr;
    if (lst->data == NULL)
        return NullDataPtr;

    return NoError;
}


ListErr_t CheckCanaryValue (List_t * lst)
{
    if (strcmp((lst->data)[0], CANARY) != 0)
        return ChangedCanaryVal;

    return NoError;
}


ListErr_t CheckNextAndPrevConnections (List_t * lst)
{
    for (int count = 0; count < lst->capacity; count++)
    {
        if (strcmp((lst->data)[count], POISON) != 0)
        {
            if ((lst->prev)[(lst->next)[count]] != count)
                return PrevNextConnectErr;
        }
    }

    return NoError;
}


ListErr_t CheckIfIndexesValid (List_t * lst)
{
    for (int count = 0; count < lst->capacity; count++)
    {
        if ((lst->next)[count] >= lst->capacity)
            return InvalidNextValue;
        if ((lst->prev)[count] >= lst->capacity)
            return InvalidPrevValue;
    }

    return NoError;
}


ListErr_t CheckIfCycles (List_t * lst)
{
    int count = 0;
    int capacity = GetCapacity(lst);

    for (int pos = (lst->next)[0]; count < capacity && pos != 0; pos = (lst->next)[pos])
        count++;
    if (count > capacity)
        return CapacityOverflow;

    return NoError;
}
