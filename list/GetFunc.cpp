#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"


FILE * GetLog (List_t * lst)
{
    assert(lst);

    return lst->log;
}


int GetCapacity (List_t * lst)
{
    assert(lst);

    return lst->capacity;
}


const char * GetPlace (Place_t place)
{
    if (place == Before)
        return "BEFORE";
    else if (place == After)
        return "AFTER"; // FIXME: fix return
    else
        return "AT";
}


char * GetCommand (char * pic_name)
{
    assert(pic_name);

    char * command = (char *) calloc (SIZE_OF_COMMAND, sizeof(char));
    assert(command); // check

    sprintf(command, "dot graphdump.txt -T png -o %s", pic_name);

    return command;
}


char * GetPicName (void)
{
    static int pic_count = 0;

    char * pic_name = (char *) calloc (SIZE_OF_PIC_NAME, sizeof(char));
    assert(pic_name); // FIXME: return error

    sprintf(pic_name, "graphdump%d.png", pic_count);
    pic_count++;

    return pic_name;
}
