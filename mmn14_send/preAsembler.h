#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "common.h"

#define MAX_MACRO_CONTENT_LENGTH 810 /*assuming no more than 10 rows of macro*/
#define MACRO_TABLE_SIZE 3
#define PERMS 0666

typedef struct macro
{
    char macroName[MAX_LINE_LENGTH];/*Name of the macro */
    char macroContent[MAX_MACRO_CONTENT_LENGTH];/*the content that should be expanded */
    struct macro *nextMacro; 
}macroNode;

/*
Expands all macro and generates a new surce code file with no macro 
defintions and the relevant macro content instead of any instance (usage) of it in the source code

Input: fileName - source code file name 
       pExpandedSourceFileNameWithExt - a pointer to the string that should be allocated inside 
       the function and be filled with the expanded source file name 
       Note: Caller should free this memory!
Output: None
*/
void runPreAsembler(char *fileName, char** pExpandedSourceFileNameWithExt);

#endif /*PRE_ASSEMBLER_H*/

