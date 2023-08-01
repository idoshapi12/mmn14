#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "common.h"



/*
Checks if str is exactly the name of one of the allowed registers (one of r1, r2..., r8)
Input: str - register name candidate
Output: True 
*/
int isRegister(char* str);

/*
  Look for the given opcode in the commands table and return its index or -1 otherwise. 
  if command not found and labelCheck is 0 then print an error

Input: line - line of code
       commandArr - table of supported commands
       labelCheck - 
Output: In case command is supported, return the index of the command in commandArr, otherwise returns -1       
*/
int findCommand (char *opcode, const command commandArr[], int lineNumber, int labelCheck);

/*
Feels the command table with all supported commands, how many operands should they be called with and 
bitmask per source or target, each stating which addressing methods are supported:
bit i is set for every legal addressing method for an operand (0,1,2,3)
if no operand for a command - byte is 0 
Input: 
       commandArr - table of supported commands
Output: None
*/
void commandTableInitialize (command commandArr[]);

                                    
/*
Code the first word of a instruction 
Input: 
       line - line of code containing the instruction 
       commandArr - command table used for veryfing commands seen in the expended source code 
       commandIndex - index of the command in the command  line - line of code containing the directive 
       pIC - in and out argument pointer to the IC
       instructions - instruction table to be filled with the operand
       labelFlag - does this line begins with a label
       lineNumber - line number in the source code (used when printing an error)
       pPass - out parameter - (pointer to) whether this line contained an error
Output: None
*/
void codeInstruction (char *line, const command commandArr[], int commandIndex, 
                                    int *pIC, codedWord instructions[], int labelFlag, 
                                    int lineNumber, int *pPass);
                                    
/*
Code the operands that was not coded before as they use symbols
Input:
        line - source line of code
        instructions - instruction table to be possibly filled with adresses of symbols for operands
        pIC - in and out argument pointer to the IC 
        commandArr - command table used for veryfing commands seen in the expended source code 
        pHeadOfLabelList - Labels table 
        sourceFileName - name of the expended source code
        labelFlag - indicates whether this line starts with a label 
        pHeadOfExternLabelInstances - pointer to the external labals table
        lineNumber - line number in the source code (used when printing an error)
        pPass - out parameter - (pointer to) whether this line contained an error
Output: None
*/
void codeOperandInSecondPass(char *line, codedWord instructions[], int *pIC, command commandArr[], 
                                   labelNode **pHeadOfLabelList, int labelFlag, 
                                    externLabelInstances **pHeadOfExternLabelInstances, int lineNumber, int *pPass);
                                    
int verifyNumOfOperands (char *line, const command commandArr[], int commandIndex, int lineNumber, int labelFlag);

#endif /* INSTRUCTIONS_H */