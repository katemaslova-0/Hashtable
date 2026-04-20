#include <stdio.h>

#include "Data.h"


void OutputErrorCode (int error)
{
    switch (error)
    {
        case NoError:                                                            break;
        case InvalidElemAddr:    printf("Invalid element adress\n");             break;
        case NullListPtr:        printf("Null list pointer\n");                  break;
        case NullDataPtr:        printf("Null data pointer\n");                  break;
        case NullNextPtr:        printf("Null next pointer\n");                  break;
        case NullPrevPtr:        printf("Null prev pointer\n");                  break;
        case ChangedCanaryVal:   printf("Changed canary value\n");               break;
        case PrevNextConnectErr: printf("Error at next and prev connection\n");  break;
        case MemAllocError:      printf("Memory allocation error\n");            break;
        case FileOpeningError:   printf("File openinf error\n");                 break;
        case InvalidNextValue:   printf("Invalid next value\n");                 break;
        case InvalidPrevValue:   printf("Invalid prev value\n");                 break;
        case CapacityOverflow:   printf("Too many connections(maybe cycles)\n"); break;
        case ElemDoesNotExist:   printf("Element does not exist\n");             break;
        case SystemError:        printf("System function error\n");              break;
        default: ;
    }
}
