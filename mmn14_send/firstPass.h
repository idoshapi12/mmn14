#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "common.h"

#define MAX_NUM_OF_INSTRUCTIONS 100
#define MAX_NUM_OF_DATA 100




/*
Build a symbol tables storing the address for every found symbol. 
Partial coding of words is also done at this stage.
   
Input:  instructions - instruction table to be filled with all instruction in the expended source code
        data - data table to be filled with all data directives in the expended source code
        commandArr - command table used for veryfing commands seen in the expended source code 
        pHeadOfLabelList - Labels table 
        sourceFileName - name of the expended source code
        pDC - pointer to the DC. Serves also as an out argument and holds the current DC counting after all instructions
              took place
Output: Returns TRUE when the first transtion passed (no error in any code line) or FALSE otherwise        
*/
int runFirstPass (codedWord instructions[], dataWord data[], const command commandArr[], labelNode **pHeadOfLabelList, char *fileName, int *pDC);


#endif /*FIRST_PASS_H*/
