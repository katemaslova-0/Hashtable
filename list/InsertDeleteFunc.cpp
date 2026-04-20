#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

static const char * POISON = "poison";

int InsertAfter (List_t * lst, int addr_of_el_before, const char * elem)
{
    //assert(lst);

    // LIST_VERIFY(lst, Before, addr_of_el_before, elem); // is assert needed?

    // if (CheckIfAddressValid(lst, addr_of_el_before) != NoError)
    //     return InvalidElemAddr;
// 
    // if (CheckIfElemBeforeExist(lst, addr_of_el_before) != NoError)
    //     return ElemDoesNotExist;
// 
    //if (CallReallocIfNeed(lst) != NoError)
//        return MemAllocError;

    CallReallocIfNeed(lst);

    int data_pos = PutElemToData(lst, elem);
    (lst->next)[data_pos] = (lst->next)[addr_of_el_before]; // setting new next's and prev's
    (lst->prev)[(lst->next)[addr_of_el_before]] = data_pos;
    (lst->next)[addr_of_el_before] = data_pos;
    (lst->prev)[data_pos] = addr_of_el_before;

    // LIST_VERIFY(lst, After, addr_of_el_before, elem);

    lst->num_of_elems++;

    return data_pos;
}


int InsertBefore (List_t * lst, int addr_of_el_after, const char * elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before, addr_of_el_after, elem);

    int pos = InsertAfter(lst, (lst->prev)[addr_of_el_after], elem);

    LIST_VERIFY(lst, After, addr_of_el_after, elem);

    return pos;
}


int InsertToHead (List_t * lst, const char * elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before, (lst->next)[0], elem);

    int pos = InsertBefore(lst, (lst->next)[0], elem);

    LIST_VERIFY(lst, After, (lst->next)[0], elem);

    return pos;
}


int InsertToTail (List_t * lst, const char * elem)
{
    // assert(lst);
    // assert(elem);

    // LIST_VERIFY(lst, Before, 0, elem); // check!! changed from elem, elem

    int pos = InsertAfter(lst, (lst->prev)[0], elem);

    // LIST_VERIFY(lst, After, 0, elem); // check!! changed from elem, elem

    return pos;
}


int DeleteEl (List_t * lst, int addr_of_el)
{
    assert(lst);

    const char * deleted_elem = lst->data[addr_of_el];
    LIST_VERIFY(lst, Before, addr_of_el, deleted_elem);

    if (CheckIfElemBeforeExist(lst, addr_of_el) != NoError)
        return ElemDoesNotExist;

    (lst->data)[addr_of_el] = POISON;
    (lst->next)[(lst->prev)[addr_of_el]] = (lst->next)[addr_of_el]; // changing next value for the prev el
    (lst->prev)[(lst->next)[addr_of_el]] = (lst->prev)[addr_of_el]; // changing prev value for the next el

    (lst->next)[addr_of_el] = lst->free; // writing the next free address (= value of free) in
    lst->free = addr_of_el;              // changing free value (to current data pos)

    (lst->prev)[addr_of_el] = FREE_ADDRESS_INDICATOR;

    LIST_VERIFY(lst, After, addr_of_el, deleted_elem);

    lst->num_of_elems--;

    return NoError;
}


/*Some inside Insert/Delete functions(not for user)*/

int PutElemToData (List_t * lst, const char * elem)
{
    // assert(lst);

    int data_pos = lst->free;
    (lst->data)[data_pos] = elem;
    lst->free = (lst->next)[lst->free];

    return data_pos;
}


ListErr_t CheckIfElemBeforeExist (List_t * lst, int address)
{
    assert(lst);

    if ((lst->prev)[address] == FREE_ADDRESS_INDICATOR)
        return ElemDoesNotExist;

    return NoError;
}


ListErr_t CheckIfAddressValid (List_t * lst, int address)
{
    assert(lst);

    if (address < 0 || address >= lst->capacity)
        return InvalidElemAddr;

    return NoError;
}


ListErr_t CallReallocIfNeed (List_t * lst)
{
    // assert(lst);

    if (lst->free == 0)
    {
        //if (ListRealloc(lst) != NoError)
        //    return MemAllocError;
        ListRealloc(lst);
    }

    return NoError;
}
