#include "common.h"


/*
Checks if this is a blank line (composed only from \t, \r, \n or is empty) or 
that this is a comment (begins with ;)
Input: string line
Output: 1 if line should be igonred or 0 otherwise
*/
int shouldIgnoreLine(char *line)
{
    while (*line)
    {
        if (!isspace(*line))
        {
            if (*line == ';')
            {
                return TRUE;
            }
            else 
            {
                return FALSE;
            }
        }
        line++;
    }
    return TRUE; 
}


/*
This function checks the following:
1. If startsWithLadder is TRUE, temp must start with '#'.
2. (After # if required),  a + or - signes can appear (only one of them and only once) following a sequence of numbers

Input: temp - line to be checked
       startsWithLadder - 1 for yes and 0 for no 
       lineNumber - line number in the source code (used when printing an error)
*/
int checkNumbers(char *operand, int startsWithLadder, int lineNumber)
{
    int i=0;
    
    if (startsWithLadder)
    {
        if (operand[i]!='#')
        {
            printf("line %d: ERROR: operand with immediate addressing must start with '#'\n", lineNumber);
            return FALSE;
        }
        i++;
    }
    
    if ((!isdigit(operand[i]))&&operand[i]!='-'&&operand[i]!='+')
    {
        printf("line %d: ERROR: Number should be start with +/- or a digit\n", lineNumber);
        return FALSE;
    }
    
    i++;
    
    while (operand[i]!='\0')
    {
        if (!isdigit(operand[i]))
        {
            printf("line %d: ERROR: Number should be composed from digits only\n", lineNumber);
            return 0;
        }
        i++;
    }
    return TRUE;
}