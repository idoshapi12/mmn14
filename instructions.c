#include "instructions.h"
#include "label.h"

#define SOURCE 0
#define TARGET 1

typedef struct 
{
    char source[MAX_LINE_LENGTH];/* the source operand */
    char target[MAX_LINE_LENGTH];/* the target operand */
}operands;

/*
Calculates log in base 2 of n
Input: n - integer Number
Output: Log2(n)
*/
static int Log2( int n )  
{  
    int answer = 0;
    while(n != 0)
    {
        n=n>>1;
        answer++;
    }
    return answer-1;
}  


/*
Checks if str is exactly the name of one of the allowed registers (one of r1, r2..., r8)
Input: str - register name candidate
Output: True 
*/
int isRegister(char* str)
{
    return (str && (strlen(str) == 2) && (str[0] == 'r') && ('0' <= str[1] && '7' >= str[1]));
}


/*
  Look for the given opcode in the commands table and return its index or -1 otherwise. 
  if command not found and labelCheck is 0 then print an error

Input: line - line of code
       commandArr - table of supported commands
       labelCheck - 
Output: In case command is supported, return the index of the command in commandArr, otherwise returns -1       
*/
int findCommand (char *opcode, const command commandArr[], int lineNumber, int labelCheck)
{
  int i;
  for (i = 0; i < 16; i++)
  {
      if (!strcmp(opcode, commandArr[i].commandName))
	  {
	    return i;
	  }
  }
  if (!labelCheck)
  {
    printf("line %d: ERROR: unsupported command \"%s\"\n", lineNumber, opcode);
  }
  return -1;
}


/*
Feels the command table with all supported commands, how many operands should they be called with and 
bitmask per source or target, each stating which addressing methods are supported:
bit i is set for every legal addressing method for an operand (0,1,2,3)
if no operand for a command - byte is 0 
Input: 
       commandArr - table of supported commands
Output: None
*/
void commandTableInitialize (command commandArr[])
{
  strcpy (commandArr[0].commandName, "mov");
  commandArr[0].numOfOperands = 2;
  commandArr[0].confirmAddressesSource = 15;
  commandArr[0].confirmAddressesTarget = 14;

  strcpy (commandArr[1].commandName, "cmp");
  commandArr[1].numOfOperands = 2;
  commandArr[1].confirmAddressesSource = 15;
  commandArr[1].confirmAddressesTarget = 15;

  strcpy (commandArr[2].commandName, "add");
  commandArr[2].numOfOperands = 2;
  commandArr[2].confirmAddressesSource = 15;
  commandArr[2].confirmAddressesTarget = 14;

  strcpy (commandArr[3].commandName, "sub");
  commandArr[3].numOfOperands = 2;
  commandArr[3].confirmAddressesSource = 15;
  commandArr[3].confirmAddressesTarget = 14;

  strcpy (commandArr[4].commandName, "not");
  commandArr[4].numOfOperands = 1;
  commandArr[4].confirmAddressesSource = 16;
  commandArr[4].confirmAddressesTarget = 14;

  strcpy (commandArr[5].commandName, "clr");
  commandArr[5].numOfOperands = 1;
  commandArr[5].confirmAddressesSource = 16;
  commandArr[5].confirmAddressesTarget = 14;

  strcpy (commandArr[6].commandName, "lea");
  commandArr[6].numOfOperands = 2;
  commandArr[6].confirmAddressesSource = 6;
  commandArr[6].confirmAddressesTarget = 14;

  strcpy (commandArr[7].commandName, "inc");
  commandArr[7].numOfOperands = 1;
  commandArr[7].confirmAddressesSource = 16;
  commandArr[7].confirmAddressesTarget = 14;

  strcpy (commandArr[8].commandName, "dec");
  commandArr[8].numOfOperands = 1;
  commandArr[8].confirmAddressesSource = 16;
  commandArr[8].confirmAddressesTarget = 14;

  strcpy (commandArr[9].commandName, "jmp");
  commandArr[9].numOfOperands = 1;
  commandArr[9].confirmAddressesSource = 16;
  commandArr[9].confirmAddressesTarget = 14;

  strcpy (commandArr[10].commandName, "bne");
  commandArr[10].numOfOperands = 1;
  commandArr[10].confirmAddressesSource = 16;
  commandArr[10].confirmAddressesTarget = 14;

  strcpy (commandArr[11].commandName, "get");
  commandArr[11].numOfOperands = 1;
  commandArr[11].confirmAddressesSource = 16;
  commandArr[11].confirmAddressesTarget = 14;

  strcpy (commandArr[12].commandName, "prn");
  commandArr[12].numOfOperands = 1;
  commandArr[12].confirmAddressesSource = 16;
  commandArr[12].confirmAddressesTarget = 15;

  strcpy (commandArr[13].commandName, "jsr");
  commandArr[13].numOfOperands = 1;
  commandArr[13].confirmAddressesSource = 16;
  commandArr[13].confirmAddressesTarget = 14;

  strcpy (commandArr[14].commandName, "rts");
  commandArr[14].numOfOperands = 0;
  commandArr[14].confirmAddressesSource = 16;
  commandArr[14].confirmAddressesTarget = 16;

  strcpy (commandArr[15].commandName, "hlt");
  commandArr[15].numOfOperands = 0;
  commandArr[15].confirmAddressesSource = 16;
  commandArr[15].confirmAddressesTarget = 16;

}


/*
Check what type of address method the param has or if no operand exists
Input: 
       param - pointer to the operand or NULL if no operand exists
Output: one of the following: NO_OPERAND/FIRST_ADDRESS_METHOD/SECOND_ADDRESS_METHOD/
                              THIRD_ADDRESS_METHOD/FOURTH_ADDRESS_METHOD
*/
static int getAddressMethod (char *param)
{
  if (param[0] == 0)
    {
      return NO_OPERAND;
    }
  else if (param[0] == '#')
    {
      return FIRST_ADDRESS_METHOD;
    }
  else if (isRegister(param))
    {
      return FOURTH_ADDRESS_METHOD;
    }
  else if (strstr (param, ".") != NULL)
    {
      return THIRD_ADDRESS_METHOD;
    }
  else
    {
      return SECOND_ADDRESS_METHOD;
    }
}


/*
Code operands based on their addressing method
Input: 
       addressMethod - one of the 4 addressing methods (0,1,2,3)
       operand - the given operand to code
       instructions - instruction table to be filled with the operand
       pIC - in and out argument pointer to the IC 
       SOT - SOURCE/TARGET
       lineNumber - line number in the source code (used when printing an error)
       pPass - out parameter - (pointer to) whether this line contained an error
Output: None
*/
static void codeOperandInFirstPass (int addressMethod, char *operand, codedWord instructions[], 
                                    int *pIC, int SOT,int lineNumber, int* pPass)
{
  switch (addressMethod)
  {
    case FIRST_ADDRESS_METHOD:
      if(!checkNumbers(operand, TRUE, lineNumber))
      {
          *pPass = FALSE;
          break;
      }
      instructions[*pIC].operandWord.operand = atoi (operand + 1);
      (*pIC)++;
      break;

    case SECOND_ADDRESS_METHOD:
      (*pIC)++;
      break;
      
    case THIRD_ADDRESS_METHOD:
        (*pIC)++;
        strtok (operand, ".");
        instructions[*pIC].operandWord.operand = atoi (strtok (NULL, "."));
        (*pIC)++;
        break;
      

    case FOURTH_ADDRESS_METHOD:
        if (SOT == TARGET)
	    {
	        instructions[*pIC].registerWord.targetRegister = atoi (operand + 1);
	    }
        else
	    {
	        instructions[*pIC].registerWord.sourceRegister = atoi (operand + 1);
	    }
	    (*pIC)++;
        break;
    default:
        break;
  }
}


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
                                    int lineNumber, int *pPass)
{
    int sourceAddressMethodCoding, targetAddressMethodCoding;
    char *operand, firstOperand[MAX_LINE_LENGTH] = { 0 };
    operands commandOperands = {0};
    strtok (line, " ,\n\r\t");		/*get rid of the opcode/the label (if exists)*/
    if (labelFlag)
    {
        strtok (NULL, " ,\n\r\t");/*if there was a label, now get rid of the opcode*/
    }
    operand = strtok (NULL, " ,\n\r\t");	/*try to get the first operand*/

    if (operand != NULL)
    {
        strcpy (firstOperand, operand);
        operand = strtok (NULL, " ,\n\r");	/*try to get the second operand*/
        if (operand == NULL)
        {
          strcpy (commandOperands.target, firstOperand);
        }
        else
        {
    	  strcpy (commandOperands.source, firstOperand);
    	  strcpy (commandOperands.target, operand);
        }
    }
    instructions[*pIC].fields.ARE = 0;
    sourceAddressMethodCoding = getAddressMethod (commandOperands.source);
    if ((sourceAddressMethodCoding & commandArr[commandIndex].confirmAddressesSource) == 0)
    {
      printf("line %d: ERROR: unsupported source address method\n", lineNumber);
      *pPass = FALSE;
      return;
    }
    
/* At sourceAddressMethodCoding we used 0,1,2 or 3 (addressing methods)
     We now want to set the ith bit (i=0,1,2 or 3) in the sourceAddressMethodCoding bitmap*/    
  instructions[*pIC].fields.sourceOperandAdressMethod = Log2(sourceAddressMethodCoding);
  targetAddressMethodCoding = getAddressMethod (commandOperands.target);
  if ((targetAddressMethodCoding & commandArr[commandIndex].confirmAddressesTarget) == 0)
  {
      printf("line %d: ERROR: unsupported target address method\n", lineNumber);
      *pPass = FALSE;
      return;
  }
    
  /* At targetAddressMethodCoding we used 0,1,2 or 3 (addressing methods)
     We now want to set the ith bit (i=0,1,2 or 3) in the targetOperandAdressMethod bitmap*/  
  instructions[*pIC].fields.targetOperandAdressMethod = Log2(targetAddressMethodCoding);
  instructions[*pIC].fields.opcode = commandIndex;
  (*pIC)++;
  codeOperandInFirstPass (sourceAddressMethodCoding, commandOperands.source,
			  instructions, pIC, SOURCE, lineNumber, pPass);
  if ((sourceAddressMethodCoding == FOURTH_ADDRESS_METHOD)
      && (targetAddressMethodCoding == FOURTH_ADDRESS_METHOD))
    {
      (*pIC)--;/*we have an instruct with 2 registers so we want to go back and update the 
               previous binary word*/
    }
  codeOperandInFirstPass (targetAddressMethodCoding, commandOperands.target, instructions, pIC, TARGET, lineNumber, pPass);
}


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
                                    externLabelInstances **pHeadOfExternLabelInstances, int lineNumber, int *pPass)
{
    int i=0, AddressMethodCoding, isRegisterOperand = FALSE, firstWordIndex;
    char *label, structOperand[MAX_LABEL_LENGTH]={0};
    
    /*There are 3 options for the number of operands of this command:
    1. No operands - in this case we coded NO_OPERAND into both sourceOperandAdressMethod targetOperandAdressMethod so we
       will update AddressMethodCoding with NO_OPERAND as required.
    2. One operands - in this case we know it must be the target operand where we coded the correct addressing method so 
       will update AddressMethodCoding with it as required.
    3. Two operands - in this case we know the first operand must be the source operand where we coded the correct addressing method so 
       will update AddressMethodCoding with it as required. We will later handle the second (source) operand
    */
    if (commandArr[instructions[*pIC].fields.opcode].numOfOperands == 1)
    {
        AddressMethodCoding = instructions[*pIC].fields.targetOperandAdressMethod;
    }
    else
    {
        AddressMethodCoding = instructions[*pIC].fields.sourceOperandAdressMethod;
    }
    strtok(line, " ,\n\r\t");
    if (labelFlag)
    {
        strtok(NULL, " ,\n\r\t");
    }
    firstWordIndex = *pIC;
    (*pIC)++;
    
    /* 
    1. If we had no operands - we will not enter the following if
    2. If we had one or two operands - we will code it depending on its addressing method
    */
    for (i=0;i<commandArr[instructions[firstWordIndex].fields.opcode].numOfOperands;i++)
    {
        label = strtok(NULL, " ,\n\r\t");
        switch (1<<AddressMethodCoding)
        {
            case FIRST_ADDRESS_METHOD:
                break;
            case SECOND_ADDRESS_METHOD:
                codeLabel(label, instructions, pIC, commandArr, pHeadOfLabelList, pHeadOfExternLabelInstances, lineNumber, pPass);
                break;
            case THIRD_ADDRESS_METHOD:
                strncpy(structOperand, label, strlen(label)-2 );
                codeLabel(structOperand, instructions, pIC, commandArr, pHeadOfLabelList, pHeadOfExternLabelInstances, 
                                    lineNumber, pPass);
                (*pIC)++;
                break;
            case FOURTH_ADDRESS_METHOD:
                if (isRegisterOperand)/*both two operands are register*/
                {
                    (*pIC)--;
                }
                isRegisterOperand = TRUE;
                break;    
            default:
                break;
                
        }
        (*pIC)++;
        AddressMethodCoding = instructions[firstWordIndex].fields.targetOperandAdressMethod;
    }
}



/*
Verify the correct number of operands and commas depending on the command
Input:
        line - source line of code
        commandArr - command table used for veryfing commands seen in the expended source code 
        commandIndex - index of the command in the command  line - line of code containing the directive 
        lineNumber - line number in the source code (used when printing an error)
        labelFlag - indicates whether this line starts with a label 
Output: None
*/
int verifyNumOfOperands (char *line, const command commandArr[], int commandIndex, int lineNumber, int labelFlag)
{
    int commaCounter = 0, i = 0, operandCounter=0;
    char checkLine[MAX_LINE_LENGTH];
    strcpy(checkLine, line);
    strtok (checkLine, " \t\n\r");
    if (labelFlag)
    {
        strtok (NULL, " \t\n\r");
    }
    if (commandArr[commandIndex].numOfOperands == 0)
    {
        if (strtok (NULL, " \t\n\r") != NULL)
        {
            printf("line %d: ERROR: command shouldn't get operands\n", lineNumber);
            return FALSE;
        }
    }
    else if (commandArr[commandIndex].numOfOperands == 1)
    {
        if (strtok (NULL, " \t\n\r") == NULL)
        {
            printf("line %d: ERROR: missing operands\n", lineNumber);
            return FALSE;
        }
        if (strtok (NULL, " \t\n\r") != NULL)
        {
            printf("line %d: ERROR: too much operands\n", lineNumber);
            return FALSE;
        }
    }
    else /*commandArr[commandIndex].numOfOperands == 2*/
    {
        while (strtok (NULL, " ,\t\n\r")!=NULL)
        {
            operandCounter++;
        }
        while (line[i] != '\n')
        {
            if (line[i] == ',')
                commaCounter++;
            i++;
        }
        if (commaCounter != 1)
        {
            if (operandCounter != 2)
            {
                printf("line %d: ERROR: wrong number of operands\n", lineNumber);
            }
            else
            {
                printf("line %d: ERROR: wrong number of commas\n", lineNumber);
            }
            return FALSE;
        }
        else if (operandCounter != 2)
        {
            printf("line %d: ERROR: wrong number of operands\n", lineNumber);
            return FALSE;
        }
    }
    return TRUE;
}