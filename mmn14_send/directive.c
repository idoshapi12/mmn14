#include "directive.h"


/*
Check whether a given string represents a string:
starts with a double quote (") and ends with another.
There can be blank characters before the 1st and/or after the 2nd double quote
Input: str - the given string
       lineNumber - line number in the source code (used when printing an error)
Output: TRUE if the is a "string" and FALSE if not
*/
int checkIfString(char* str, int lineNumber)
{
    int firstDoubleQuote, i;

    i=0;
    while(isspace(str[i])) /* pass white spaces if any */
    {
        i++;
    }
    
    if(str[i] != '\"') /* now we must see the start of a string (")*/
    {
        printf("line %d: ERROR: expected start of string (\")\n", lineNumber);
        return FALSE;
    }
    firstDoubleQuote = i;
    
     /* go back on the line until we see the end of a string (")*/ 
     i = strlen(str) - 1;
     while(isspace(str[i]) && (firstDoubleQuote < i))
     {
         i--;
     }
    
     if(i == firstDoubleQuote) /*string is illegal - contains only one double quote (")*/
     {
         printf("line %d: ERROR: didn't find end of string (\")\n", lineNumber);
         return FALSE;
     }
        
     if(str[i] != '\"') /* now we must see the end of a string (")*/
     {
         printf("line %d: ERROR: didn't find end of string (\")\n", lineNumber);
         return FALSE;
     }
        
    return TRUE;    
}

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

int isStorageDirective(char *line, int labelFlag, int lineNumber, int *pPass)
{
    char *param, *directive, checkLine[MAX_LINE_LENGTH];
    int paramCoumter = 0, commaCounter=0, i=0;
    
    strcpy(checkLine, line);
    
    directive = strtok(checkLine," ,\t\n\r");
    if (labelFlag)
    {
        directive = strtok(NULL," ,\t\n\r");/* if line starts with a label then 2nd token is the directive*/
    }
    if (!strcmp(directive, ".data"))
    {
        param = strtok(NULL," ,\n\r\t");
        /*expecting comma separated list of numbers */
        if (param == NULL)
        {
            printf("line %d: ERROR: .data directive must contain at least one number\n", lineNumber);
            return 2;
        }
        while(param != NULL)
        {
            if (!checkNumbers(param, FALSE, lineNumber))
            {
                *pPass = FALSE;
                printf("line %d: ERROR: .data directive must contain numbers only\n", lineNumber);
                return 2;
            }
            param=strtok(NULL," ,\n\r\t");
            paramCoumter++;
        }
        i=0;
        while (line[i] != '\n')
        {
            if (line[i] == ',')
                commaCounter++;
            i++;
        }
        if (paramCoumter != commaCounter+1)
        {
            printf("line %d: ERROR: wrong number of commas or parameters\n", lineNumber);
            return 2;
        }
    }
    else if (!strcmp(directive, ".string"))
    {
        param=strtok(NULL,""); /*get rest of line following .string*/
        if (param == NULL)
        {
            printf("line %d: ERROR: .string directive must get a string\n", lineNumber);
            return 2;
        }
        if (!checkIfString(param, lineNumber))
        {
            *pPass = FALSE;
            return 2;
        }
    }
    else if (!strcmp(directive, ".struct"))
    {
        param = strtok(NULL," \t\n");
        
        /* reach the address in directive that is after the .string and possible blank characters
        for(i=0; (directive + i) < param; i++);*/
        
        /*we want to make sure there is only one comma (,) and that it is placed correctly
          so first we will count how many commas until end of line
          we can start after the '.struct' section as we know its address*/
        
        for(i = (param-checkLine); (i < strlen(line)); i++)
        {
            if(line[i] == ',')
            {
                /*special failure - the first non blank character folowing .struct cannot be ','*/
                if(i == (param-checkLine))
                {
                    printf("line %d: ERROR: .struct directive must get exactly 2 values with a comma between them\n", lineNumber);
                    return 2;
                }
                commaCounter++;
            }
        }
        
        if(commaCounter != 1)
        {
            printf("line %d: ERROR: .struct directive must get exactly 2 values with a comma between them\n", lineNumber);
            return 2;
        }
        
        /* now that we know there is exactly 1 comma, we can safely use strtok to get the 2 items */
        strcpy(checkLine, line);
        directive = strtok(checkLine," ,\t\n\r");
        if (labelFlag)
        {
            directive = strtok(NULL," ,\t\n\r");/* if line starts with a label then 2nd token is the directive*/
        }
        
        param = strtok(NULL," ,\t\n");

        if (!checkNumbers(param, FALSE, lineNumber))
        {
            *pPass = FALSE;
            return 2;
        }
       
        param=strtok(NULL," ,\t\n"); /*get the second item of the struct */
    
        if (!checkIfString(param, lineNumber))
        {
            *pPass = FALSE;
            return 2;
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}


/*
Check whether this line is an extern directive
Input: line - code line 
Output: TRUE if directive is extern otherwise FALSE
*/
int isExternDirective (char *line, int labelFlag, int lineNumber)
{
  char *externDirective, checkLine[MAX_LINE_LENGTH];
  strcpy(checkLine, line);
  externDirective = strtok(checkLine, " \t\n\r");
  if (labelFlag)
  {
      externDirective = strtok(NULL, " \t\n\r");
  }
  if (!strcmp(externDirective, ".extern"))
  {
      if (labelFlag)
      {
          printf("line %d: ERROR: label in start of extern directive is meaningless\n", lineNumber);
          return 2;
      }
      return 1;
  }
  return 0;
}


/*
Check whether this line is an entry directive

Input: line - code line 
Output: TRUE if directive is entry otherwise FALSE
*/
int isEntryDirective (char *line, int labelFlag, int lineNumber)
{
  char *entryDirective, checkLine[MAX_LINE_LENGTH];
  strcpy(checkLine, line);
  entryDirective = strtok(checkLine, " \t\n\r");
  if (labelFlag)
  {
      entryDirective = strtok(NULL, " \t\n\r");
  }
  if (!strcmp(entryDirective, ".entry"))
  {
      if (labelFlag)
      {
          printf("line %d: ERROR: label in start of entry directive is meaningless\n", lineNumber);
      }
      return TRUE;
  }
  return FALSE;
}


/*
Assumes getting a correct string "<...>" and copies all its content <...> into 
the data table
Input:  
        str - string to be stored 
        pDC - the DC counter for inserting data corectly to the data table (increments in function)
        data - the data table
Output: None     
*/
static void codeString(char *str, int *pDC, dataWord data[])
{
    int i=1;/*skip the initial '"'*/
    while (str[i]!='\"')/*until we reach end of string*/
    {
        data[(*pDC)++].paramWord.param = str[i++];
    }
    data[(*pDC)++].paramWord.param = 0;
}


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
void codeData (char *line, dataWord data[], int *pDC, int labelFlag)
{
    char *dataDirective, *param;
    dataDirective = strtok (line, " ,\t");		/*get rid of the opcode*/
    if (labelFlag)
    {
      dataDirective = strtok (NULL, " ,\t");
    }
    if (strcmp(dataDirective, ".data")==0)
    {
        while ((param = strtok (NULL, " ,\t")) != NULL)
        {
            data[(*pDC)++].paramWord.param = atoi(param);
        }
    }
    else if (strcmp(dataDirective, ".string")==0)
    {
        param = strtok (NULL, " ,\t");
        codeString(param, pDC, data);
    }
    else if (strcmp(dataDirective, ".struct")==0)
    {
        param = strtok (NULL, " ,\t");
        data[(*pDC)++].paramWord.param = atoi(param);
        param = strtok (NULL, " ,\t");
        codeString(param, pDC, data);
    }
    
}



/*
Check if line is a directive 

Input: line - source line of code
Output: 1 if the line is a directive otherwise 0
*/
int isDirective (char *line)
{
    if (strstr (line, ".data") == NULL && strstr (line, ".string") == NULL
    && strstr (line, ".struct") == NULL && strstr (line, ".extern") == NULL)
        return 0;
    return 1;
}

/*
Update that a label is of entry type in the labels table. However, if this label is already marked as EXTERNAL
(at the first pass) or undefined, report an error and fail
Input:
        line - line - source line of code
        pHeadOfLabelList -  pHeadOfLabelList - Labels table 
        lineNumber - line number in the source code (used when printing an error)
        
Output: 1 if label was successfully updated otherwise 0

*/
int setLabelAsEntry(char *line, labelNode **pHeadOfLabelList, int lineNumber)
{
    char *token;
    labelNode *ptr = *pHeadOfLabelList;
    strtok(line," \n\r\t");
    token=strtok(NULL," \n\r\t");
    for(;ptr != NULL; ptr = ptr->nextLabel)
    {
        if (strcmp(ptr->labelName, token)==0)
        {
            if (ptr->labelType == EXTERNAL)
            {
                printf("line %d: ERROR: label \"%s\" can't be entry and external at the same time\n", lineNumber, token);
                return FALSE;
            }
            ptr->labelType=ENTRY;
            return TRUE;
        }
    }
    printf("line %d: ERROR: entry instruction references an unknown label \"%s\"\n", lineNumber, token);
    return FALSE;
}