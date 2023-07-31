#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include "common.h"


/*
Check whether a given string represents a string:
starts with a double quote (") and ends with another.
There can be blank characters before the 1st and/or after the 2nd double quote
Input: str - the given string
       lineNumber - line number in the source code (used when printing an error)
Output: TRUE if the is a "string" and FALSE if not
*/
int checkIfString(char* str, int lineNumber);

/*
Checks whether this code line is a storage directive 
Input: line - line of code
       labelFlag - does this line begins with a label
       lineNumber - line number in the source code (used when printing an error)
       pPass - out parameter - (pointer to) whether this line contained an error
Output: TRUE when this is a storage directive and it's correct.
        2 when this is a storage directive and it's NOT correct.
        FALSE when this is NOT a storage directive
*/
int isStorageDirective (char *line, int labelFlag, int lineNumber, int *pPass);

/*
Check whether this line is an extern directive
Input: line - code line 
Output: TRUE if directive is extern otherwise FALSE
*/
int isExternDirective (char *line, int labelFlag, int lineNumber);

/*
Check whether this line is an entry directive

Input: line - code line 
Output: TRUE if directive is entry otherwise FALSE
*/
int isEntryDirective (char *line, int labelFlag, int lineNumber);

/*
Code the different data elements to the correct binary format:
1. For .data directive - code numbers instead of their ASCII representation
2. For .string - copy it as is to data table
3. For .struct - copy the number and the string 
Input:  
        line - line of code containing the directive 
        data - the data table
        pDC - the DC counter for inserting data corectly to the data table (increments in function)
        labelFlag - Whether directive contains a label
Output: None     
*/
void codeData (char *line, dataWord data[], int *pDC, int labelFlag);

/*
Check if line is a directive 

Input: line - source line of code
Output: 1 if the line is a directive otherwise 0
*/
int isDirective (char *line);

/*
Update that a label is of entry type in the labels table. However, if this label already marked as EXTERNAL
(at the first pass), report an error and fail
Input:
        line - line - source line of code
        pHeadOfLabelList -  pHeadOfLabelList - Labels table 
        lineNumber - line number in the source code (used when printing an error)
        
Output: 1 if label was successfully updated otherwise 0

*/
int setLabelAsEntry(char *line, labelNode **pHeadOfLabelList, int lineNumber);

#endif /* DIRECTIVE_H */