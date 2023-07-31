#ifndef COMMON_H
#define COMMON_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define COMMAND_LENGTH 3
#define PERMS 0666
#define TRUE 1
#define FALSE 0 
#define FIRST_ADDRESS_METHOD 1
#define SECOND_ADDRESS_METHOD (1<<1) /*2*/
#define THIRD_ADDRESS_METHOD (1<<2) /*4*/
#define FOURTH_ADDRESS_METHOD (1<<3) /*8*/
#define NO_OPERAND (1<<4) /*16*/
#define DATA 0
#define CODE 1
#define EXTERNAL 2
#define ENTRY 3
#define INITIAL_INSTRUCTION_ADDRESS 100


/*Labels table implementation as a linked list*/
typedef struct label
{
    char labelName[MAX_LINE_LENGTH]; /*name of label as appears in the source code*/
    int labelAddress; /*address that was allocated to this label*/
    int labelType; /*One of DATA/CODE/EXTERNAL/ENTRY*/
    struct label *nextLabel;
}labelNode;

/*This struct is used to describe each command's unique parameters*/
/*it is used to comapre and verify that commands in the source code are writtern*/
/*correctly*/
typedef struct command
{
    char commandName[COMMAND_LENGTH];
    int numOfOperands; /*num of operand for this command*/
    
    /*byte of bitmap (mask) for legal adressing methods for the source and target operands:*/
    /*bit i is set for every legal addressing method for an operand (0,1,2,3)*/
    /*if no operand for a command - byte is 0*/
    unsigned char confirmAddressesSource;
    unsigned char confirmAddressesTarget;
}command;

/*we use a union for multiple views of the same data for a code word*/
typedef union 
{
  struct
  {
    unsigned int ARE:2; /*bits 0,1 for A/R/E (values: 0-2)*/
    unsigned int targetOperandAdressMethod : 2; /*bits 2,3 for target operand addressing (values: 0-3)*/
    unsigned int sourceOperandAdressMethod : 2; /*bits 4,5 for source operand addressing (values: 0-3)*/
    unsigned int opcode : 4; /*bits 6-9 for opcode as apperas in the comnmand table (values: 0-15)*/
    unsigned int notUsed : 22; /*as we need only 10 bits, we don't use the 22 MSBs of the unsigned int*/
  }fields;
  
  struct
  {
    unsigned int ARE : 2; /*bits 0,1 for A/R/E (values: 0-2)*/
    unsigned int operand : 8; /*bits 2-9 for operand codding*/
    unsigned int notUsed : 22; /*as we need only 10 bits, we don't use the 22 MSBs of the unsigned int*/
  }operandWord;
  
  struct
  {
    unsigned int ARE : 2; /*bits 0,1 for A/R/E (values: 0-2)*/
    unsigned int targetRegister : 4; /*bits 2-5 for the target register (values: 0-7)*/
    unsigned int sourceRegister : 4; /*bits 6-9 for the source register (values: 0-7)*/
    unsigned int notUsed : 22; /*as we need only 10 bits, we don't use the 22 MSBs of the unsigned int*/
  }registerWord;
  
  unsigned int cWord; 
  struct
  {
    unsigned int first : 5; /*bits 0-4 for the LSB digit of the used machine language coding*/
    unsigned int second : 5; /*bits 5-9 for the MSB digit of the used machine language coding*/
    unsigned int notUsed : 22; /*as we need only 10 bits, we don't use the 22 MSBs of the unsigned int*/
  }code32Fields;
}codedWord;

/*we use a union for multiple views of the same data for a data word*/
typedef union 
{
  struct
  {
    unsigned int param : 10; /*bits 0-9 for holding the data*/
    unsigned int notUsed : 22; /*as we need only 10 bits, we don't use the 22 MSBs of the unsigned int*/
  }paramWord;
  unsigned int dWord;
  struct
  {
    unsigned int first : 5; /*bits 0-4 for the LSB digit of the used machine language coding*/
    unsigned int second : 5; /*bits 5-9 for the MSB digit of the used machine language coding*/
    unsigned int notUsed : 22; /*as we need only 10 bits, we don't use the 22 MSBs of the unsigned int*/
  }code32Fields;
}dataWord;

typedef struct externLabel
{
    char labelName[MAX_LABEL_LENGTH];/* Name of label */
    int lineAddressInstance; /* The address in expanded source code (.am) where the label is used */
    struct externLabel *nextExtern;
}externLabelInstances;


/*
Checks if this is a blank line (composed only from \t, \r, \n or is empty) or 
that this is a comment (begins with ;)
Input: string line
Output: 1 if line should be igonred or 0 otherwise
*/
int shouldIgnoreLine(char *line);

/*
This function checks the following:
1. If startsWithLadder is TRUE, temp must start with '#'.
2. (After # if required),  a + or - signes can appear (only one of them and only once) following a sequence of numbers

Input: temp - line to be checked
       startsWithLadder - 1 for yes and 0 for no 
       lineNumber - line number in the source code (used when printing an error)
*/
int checkNumbers(char *operand, int startsWithLadder, int lineNumber);

#endif /*COMMON_H*/
