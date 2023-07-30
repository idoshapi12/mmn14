#ifndef SECOND_PASS_H
#define SECOND_PASS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "common.h"




/*
Performs the second transition of the assembler's aalgorithm where the binary code
is updated with the values of the symbols taken from the symbol table

Input: instructions - the array containing the instructions
       data - the array containing the data
       commandArr - the array containing the commands
       pHeadOfLabelList - linked list of all labels
       fileName - the source code   
       DC - the data counter
Output: None
*/
void runSecondPass (codedWord instructions[], dataWord data[], command commandArr[], labelNode **pHeadOfLabelList, char *fileName, int DC);


#endif /*SECOND_PASS_H*/