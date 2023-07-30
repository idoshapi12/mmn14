#include "secondPass.h"
#include "directive.h"
#include "instructions.h"
#include "label.h"





/*
Creates the object file (.ob) in the machine's language coding from the binary representation of commands and data 

Input:
        sourceFileName- name of the source file w/p extenstion - used to form the name of the new .ob file
        IC - IC
        instructions - instruction table 
        DC - DC
        data - data table for creating the final object (.ob) file 
        digitTable - maps from binary coding to the machine's language coding
Output: None        
*/
static void creatObjectFile(char *sourceFileName, int IC, codedWord instructions[], int DC, dataWord data[], 
                            char digitTable[])
{
    char *fileName;
    FILE *n = NULL;
    int fd, i;
    dataWord base32Address;
    base32Address.paramWord.param = INITIAL_INSTRUCTION_ADDRESS;
    fileName = strtok(sourceFileName, ".");
    strcat(fileName, ".ob");
    if((fd = creat(fileName, PERMS))==-1)
    {
        printf("error in creating file");
    }
    n=fopen(fileName,"w");
    if (n==NULL)
    {
        printf("error in opening file");
    }
    /* write all instructions to file */
    for (i=0;i<IC;i++)
    {
        fprintf(n, "%c%c\t%c%c\n", digitTable[base32Address.code32Fields.second], digitTable[base32Address.code32Fields.first]
        , digitTable[instructions[i].code32Fields.second], digitTable[instructions[i].code32Fields.first]);
        (base32Address.paramWord.param)++;
    }
    
    /* write all storage items to file */
    for (i=0;i<DC;i++)
    {
        fprintf(n, "%c%c\t%c%c\n", digitTable[base32Address.code32Fields.second], digitTable[base32Address.code32Fields.first]
        , digitTable[data[i].code32Fields.second], digitTable[data[i].code32Fields.first]);
        (base32Address.paramWord.param)++;
    }
    fclose(n);
}

/*
Creates the object file (.ent) if needed. The entry file lists all entries with their adresses 

Input:
        pHeadOfLabelList - Labels table 
        sourceFileName - name of the expended source code
        digitTable - maps from binary coding to the machine's language coding
Output: None        
*/
static void creatEntryFile(labelNode **pHeadOfLabelList, char *sourceFileName, char digitTable[])
{
    int fd=0;
    char *fileName;
    FILE *entryFile = NULL;
    dataWord base32Address;
    labelNode *ptr = *pHeadOfLabelList;
    for (;ptr!=NULL;ptr=ptr->nextLabel)
    {
        if (ptr->labelType == ENTRY)
        {
            if (!fd)
            {
                fileName = strtok(sourceFileName, ".");
                strcat(fileName, ".ent");
                if((fd = creat(fileName, PERMS))==-1)
                {
                    printf("error in creating file");
                }
                entryFile=fopen(fileName,"w");
                if (entryFile==NULL)
                {
                    printf("error in opening file");
                }
            }
            base32Address.paramWord.param = ptr->labelAddress;
            fprintf(entryFile, "%-10s\t%c%c\n", ptr->labelName, digitTable[base32Address.code32Fields.second], digitTable[base32Address.code32Fields.first]);
        }
    }
    if (entryFile)
    {
        fclose(entryFile);
    }
}

/*
Creates the object file (.ext) if needed. The externals file lists all externals with their adresses 

Input:
        pHeadOfLabelList - Labels table 
        sourceFileName - name of the expended source code
        digitTable - maps from binary coding to the machine's language coding
Output: None        
*/
static void creatExternFile(externLabelInstances *pHeadOfExternLabelInstances, char *sourceFileName, char digitTable[])
{
    int fd=0;
    char *fileName;
    FILE *externFile = NULL;
    dataWord base32Address;
    externLabelInstances *ptr = pHeadOfExternLabelInstances;
    for (;ptr!=NULL;ptr=ptr->nextExtern)
    {
        if (!fd)
        {
            fileName = strtok(sourceFileName, ".");
            strcat(fileName, ".ext");
            if((fd = creat(fileName, PERMS))==-1)
            {
                printf("error in creating file");
            }
            externFile=fopen(fileName,"w");
            if (externFile==NULL)
            {
                printf("error in opening file");
            }
        }
        base32Address.paramWord.param = ptr->lineAddressInstance;
        fprintf(externFile, "%-10s\t%c%c\n", ptr->labelName, digitTable[base32Address.code32Fields.second], digitTable[base32Address.code32Fields.first]);
    }
    if (externFile)
    {
        fclose(externFile);
    }
}


/*
Complete the coding of all words. In this stage we complete the missing coding of the symbols as we now have all their 
adresses stroed in the labels table (filled in the 1st stage)

Input:
        instructions - instruction table to be possibly filled with adresses of symbols for operands
        data - data table for creating the final object (.ob) file 
        commandArr - command table used for veryfing commands seen in the expended source code 
        pHeadOfLabelList - Labels table 
        sourceFileName - name of the expended source code
        DC - The DC
*/
void runSecondPass (codedWord instructions[], dataWord data[], command commandArr[], labelNode **pHeadOfLabelList, 
                      char *sourceFileName, int DC)
{
    FILE *s = NULL;
    static char digitTable[] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>','a', 
                              'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
                              'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};
    int IC = 0, labelFlag, lineNumber = 1, pass = TRUE;
    char line[MAX_LINE_LENGTH], *notEOF = NULL;
    externLabelInstances *headOfExternLabelInstances = NULL;
    	
    s = fopen (sourceFileName, "r");
    if (s == NULL)
    {
        printf ("error in opening file\n");
    }
    while ((notEOF = fgets(line, MAX_LINE_LENGTH, s)) != NULL)/* process all lines until end of file */
    {
        if (!shouldIgnoreLine(line))
        {
            /* line is not blank nor a comment */
            labelFlag=FALSE;
            if (isLabel (line) == 1)
    	    {
    	        labelFlag = TRUE;
    	    }
            if (strstr (line, ".entry"))
        	{
        	  pass = setLabelAsEntry(line, pHeadOfLabelList, lineNumber);
            }
            else if (!isDirective(line))
            {
                codeOperandInSecondPass(line, instructions, &IC, commandArr, pHeadOfLabelList, labelFlag, &headOfExternLabelInstances, 
                                       lineNumber, &pass);
            }
        }
        lineNumber++;
	}


    if (!pass)
    {
        return;
    }
    creatObjectFile(sourceFileName, IC, instructions, DC, data, digitTable);/* create the .ob file */
    creatEntryFile(pHeadOfLabelList, sourceFileName, digitTable);/* create the .ent file if needed */
    creatExternFile(headOfExternLabelInstances, sourceFileName, digitTable);/* create the .ext file if needed */
    
    resetExternLabelInstances(&headOfExternLabelInstances);
    
    if(s)
    {
        fclose(s);
    }
}
