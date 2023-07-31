#include "label.h"
#include "instructions.h"

/*
Returns whether a given label exists in the label table

Input: headOfLabelList - Labels Table
       labelName - Given label
      
Output: TRUE if label exists otherwise FALSE
*/
static int findLabel(labelNode * headOfLabelList, char* labelName)
{
    labelNode * ptr = headOfLabelList;
    for (;ptr!=NULL;ptr=ptr->nextLabel)
    {
        if (!strcmp(ptr->labelName, labelName))
        {
            return TRUE;
        }
    }
    return FALSE;
}


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
int verifyLabel(labelNode * headOfLabelList, char *labelWithColon, int lineNumber, const command commandArr[], int *pPass)
{
    int i, labelLength;
    char label[MAX_LABEL_LENGTH];
    labelLength = strlen(labelWithColon)-1;
    if (labelWithColon[labelLength] == ':')
    {
        labelWithColon[labelLength] = 0;
    }
    if (labelLength > MAX_LABEL_LENGTH)
    {
        printf("line %d: ERROR: label \"%s\" is longer than allowed\n", lineNumber, labelWithColon);
        *pPass = FALSE;
        return FALSE;
    }
    strcpy(label, labelWithColon);
    if (!isalpha(label[0]))
    {
        printf("line %d: ERROR: label \"%s\" can't start with '%c' char\n", lineNumber, label, label[0]);
        *pPass = FALSE;
        return FALSE;
    }
    for (i=1;i<labelLength-1;i++)
    {
        if (!isalnum(label[i]))
        {
            printf("line %d: ERROR: label \"%s\" can't contain the char '%c'\n", lineNumber, label, label[i]);
            *pPass = FALSE;
            return FALSE;
        }
    }
    if (findCommand(label, commandArr, lineNumber, TRUE) != -1)
    {
        printf("line %d: ERROR: label \"%s\" can't be defined as instruction name\n", lineNumber, label);
        *pPass = FALSE;
        return FALSE;
    }
    
    if(findLabel(headOfLabelList, label))
    {
        printf("line %d: ERROR: label \"%s\" was defined more than once\n", lineNumber, label);
        *pPass = FALSE;
        return FALSE;
    }
    if (isRegister(label))
    {
        printf("line %d: ERROR: label \"%s\" can't be defined as register name\n", lineNumber, label);
        *pPass = FALSE;
        return FALSE;
    }
    return TRUE;
}


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
                  int lineNumber)
{
  char *label, checkLine[MAX_LINE_LENGTH];
  labelNode *ptr = (labelNode *) malloc (sizeof (labelNode));
  strcpy(checkLine, line);
  label = strtok (checkLine, " :\n\r\t");
  strcpy (ptr->labelName, label);
  if (type != EXTERNAL)
  {
      ptr->labelAddress = address + INITIAL_INSTRUCTION_ADDRESS;
  }
  ptr->labelType = type;
  ptr->nextLabel = *pHeadOfLabelList;
  *pHeadOfLabelList = ptr;
}


/*
Update all data labels - add The total IC to them as we want the data section 
to follow the instruction section in memory
Input:  
        pHeadOfLabelList - Labels table 
        IC - IC after counting all instructions
Output: None     
*/
void updateDataLabels(labelNode **pHeadOfLabelList, int IC)
{
    labelNode *ptr = *pHeadOfLabelList;
    for (;ptr!=NULL;ptr=ptr->nextLabel)
    {
        if (ptr->labelType == DATA)
        {
            ptr->labelAddress += IC;
        }
    }
}


/*
Free label node allocated memory
Input:  
        pHeadOfLabelList - Labels table 
Output: None     
*/
void resetLabelTable(labelNode **pHeadOfLabelList)
{
    /*free all allocated nodes*/
    labelNode* tmp;

    while (*pHeadOfLabelList != NULL)
    {
       tmp = *pHeadOfLabelList;
       *pHeadOfLabelList = (*pHeadOfLabelList)->nextLabel;
       free(tmp);
    }
}


/*
Free all allocated memory used for external labels table

Input: pHeadOfExternLabelInstances - pointer to the external labals table
Output: None
*/
void resetExternLabelInstances(externLabelInstances **pHeadOfExternLabelInstances)
{
    /*free all allocated nodes*/
    externLabelInstances* tmp;

    while (*pHeadOfExternLabelInstances != NULL)
    {
       tmp = *pHeadOfExternLabelInstances;
       *pHeadOfExternLabelInstances = (*pHeadOfExternLabelInstances)->nextExtern;
       free(tmp);
    }
}


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
                                int lineNumber, int *pPass)
{ 
    labelNode *ptr = *pHeadOfLabelList;
    externLabelInstances *ptr2;
    for (;ptr!=NULL;ptr=ptr->nextLabel)
    {
        if (strcmp(ptr->labelName, label)==0)
        {
            instructions[*pIC].operandWord.operand = ptr->labelAddress;
            if (ptr->labelType == EXTERNAL)
            {
                instructions[*pIC].operandWord.ARE = E;
                ptr2 = (externLabelInstances *) malloc (sizeof (externLabelInstances));
                strcpy(ptr2->labelName, label);
                ptr2->lineAddressInstance = *pIC + INITIAL_INSTRUCTION_ADDRESS;
                ptr2->nextExtern = *pHeadOfExternLabelInstances;
                *pHeadOfExternLabelInstances = ptr2;
            }
            else 
            {
                instructions[*pIC].operandWord.ARE = R;
            }
            return;
        }
    }
    printf("line %d: ERROR: label \"%s\" used without being defined\n", lineNumber, label);
    *pPass = FALSE;
}


/*
Returns whether a string line starts with a label (contains ':')
Input: string line
Output: 1 if it's a label or 0 otherwise
*/
int isLabel (char *line)
{
  int i = 0;
  while (line[i] != ':' && line[i] != '\n')
    {
      i++;
    }
  if (line[i] == ':')
    return TRUE;
  return FALSE;
}