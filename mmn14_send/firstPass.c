#include "firstPass.h"
#include "directive.h"
#include "instructions.h"
#include "label.h"




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

int runFirstPass (codedWord instructions[], dataWord data[], const command commandArr[], labelNode **pHeadOfLabelList, char *sourceFileName, int *pDC)
{
    int pass = TRUE, labelFlag = 0, commandIndex, lineNumber=0, legal, IC = 0, DC = 0, labelBeforeExtern;
    FILE *s = NULL;
    char line[MAX_LINE_LENGTH], *externLabel, checkLine[MAX_LINE_LENGTH], *labelOrCommand, *notEOF = NULL;
    
    s = fopen (sourceFileName, "r");
    if (s == NULL)
    {
        printf ("error in opening file\n");
    }
    
    while ((notEOF = fgets(line, MAX_LINE_LENGTH, s)) != NULL)
    {
        lineNumber++;
        if (strchr(line, '\n') == NULL && (!feof(s)))
        {
            printf("line %d: ERROR: line longer then allowed\n", lineNumber);
            fgets(line, MAX_LINE_LENGTH, s);
            pass = FALSE;
            continue;
        }
        if (!shouldIgnoreLine(line))
        {
            /* line is not blank nor a comment */
            labelFlag = FALSE;
            /*flag = 1;*/
            strcpy(checkLine, line);
            /*get first token of line and find out if it is a label*/
            labelOrCommand = strtok(checkLine, " \t\n\r");
            if (labelOrCommand[strlen(labelOrCommand)-1] == ':')
            {
                labelFlag = TRUE;
            }
            /*labelFlag = checkLabel(line);*/
            
            if ((legal = isStorageDirective (line, labelFlag, lineNumber, &pass)))
            {			
                if (labelFlag && (verifyLabel(*pHeadOfLabelList, labelOrCommand, lineNumber, commandArr, &pass)))
                {			
                  addLabel (line, pHeadOfLabelList, DATA, DC, commandArr, lineNumber);
                }
                if (legal == 1)
                {
                    codeData (line, data, &DC, labelFlag);
                }
            }
            else if ((labelBeforeExtern = isExternDirective(line, labelFlag, lineNumber)))
            {
                if (labelBeforeExtern == 2)
                {
                    pass = FALSE;
                    continue;
                }
                
                strtok(line, " \t\n\r");
                externLabel = strtok(NULL, " \t\n\r");
                if (externLabel == NULL || strtok(NULL, " \t\n\r") != NULL)
                {
                    printf("line %d: ERROR: .extern directive should get one valid label\n", lineNumber);
                    continue;
                }
                if (verifyLabel(*pHeadOfLabelList, externLabel, lineNumber, commandArr, &pass))
                {
                    addLabel (externLabel, pHeadOfLabelList, EXTERNAL, 0, commandArr, lineNumber);
                }
            }
            else if (!isEntryDirective(line, labelFlag, lineNumber))
            {/*instruction*/
                if (labelFlag && (verifyLabel(*pHeadOfLabelList, labelOrCommand, lineNumber, commandArr, &pass)))
                {
                  addLabel (line, pHeadOfLabelList, CODE, IC, commandArr, lineNumber);
                }
                if (labelFlag)
                {
                     /* we must re-do copy since strtok was used at checkEntry()*/
                    strcpy(checkLine, line);
                    strtok(checkLine, " \t\n\r");
                    labelOrCommand = strtok(NULL, " \t\n\r");
                }
                if (((commandIndex = findCommand (labelOrCommand, commandArr, lineNumber, FALSE)) != -1)
                     && verifyNumOfOperands (line, commandArr, commandIndex, lineNumber, labelFlag))
                {
                  codeInstruction (line, commandArr, commandIndex, &IC, instructions, 
                                         labelFlag, lineNumber, &pass);
                }
                else
                {
                    pass = FALSE;
                }
            }
        }
    }
    *pDC = DC;
    updateDataLabels(pHeadOfLabelList, IC);
    
    if(s)
    {
        fclose(s);
    }

    return pass;	

}
