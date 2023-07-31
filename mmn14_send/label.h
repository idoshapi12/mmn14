#ifndef LABEL_H
#define LABEL_H

#include "common.h"

#define A 0
#define R 2
#define E 1



/*
Check whether a given label is valid. A Valid label starts with a letter (little or capital) following 
letters and/or digits. The label must end with a colon. The same label cannot be defined more than once and can't be 
a reserved name: register name or command or directive
Input: headOfLabelList - Labels Table
       labelWithColon - the given label
       lineNumber - line number in the source code (used when printing an error)
       commandArr - table of supported commands
Output: TRUE if the label is valid and FALSE if not
*/
int verifyLabel(labelNode * headOfLabelList, char *labelWithColon, int lineNumber, const command commandArr[], int *pPass);

/*
Insert a label into the labels list 
Input: 
       line - line containing the label
       pHeadOfLabelList - pointer to Labels Table
       type - external or entry
       address - The address where the label seats at 
       commandArr - table of supported commands
       lineNumber - line number in the source code (used when printing an error)
Output: None
*/
void addLabel (char *line, labelNode ** pHeadOfLabelList, int type, int address, const command commandArr[], 
                  int lineNumber);
                  
/*
Update all data labels - add The total IC to them as we want the data section 
to follow the instruction section in memory
Input:  
        pHeadOfLabelList - Labels table 
        IC - IC after counting all instructions
Output: None     
*/
void updateDataLabels(labelNode **pHeadOfLabelList, int IC);

/*
Free label node allocated memory
Input:  
        pHeadOfLabelList - Labels table 
Output: None     
*/
void resetLabelTable(labelNode **pHeadOfLabelList);

/*
Free all allocated memory used for external labels table

Input: pHeadOfExternLabelInstances - pointer to the external labals table
Output: None
*/
void resetExternLabelInstances(externLabelInstances **pHeadOfExternLabelInstances);

/*
Code label to the instructions table. If this is external label we need also to add it to the external labels table
Input:
        label - the name of the label
        instructions - instruction table to be possibly filled with adresses of symbols for operands
        pIC - in and out argument pointer to the IC 
        commandArr - command table used for veryfing commands seen in the expended source code 
        pHeadOfLabelList - Labels table 
        pHeadOfExternLabelInstances - pointer to the external labals table
        lineNumber - line number in the source code (used when printing an error)
        pPass - out parameter - (pointer to) whether this line contained an error
Output: None
*/
void codeLabel(char *label, codedWord instructions[], int *pIC, command commandArr[], 
                                labelNode **pHeadOfLabelList, externLabelInstances **pHeadOfExternLabelInstances, 
                                int lineNumber, int *pPass);
                                
/*
Returns whether a string line starts with a checkLabel (contains ':')
Input: string line
Output: 1 if it's a label or 0 otherwise
*/
int isLabel (char *line);

#endif /* LABEL_H */