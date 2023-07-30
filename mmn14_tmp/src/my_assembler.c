
#include "preAsembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "instructions.h"
#include "label.h"

/*
This function is a cleanup function called after every source code assemblying
It frees allocated memory (that was yet freed) and initializes the data structures
for a possible assemblying of the next file
Input: instructions - the instruction table std::array
       data - the data table array
       pDC - pointer to the DC (will be reset to 0)
       pHeadOfLabelList - the label table linked list
       expandedSourceFileNameWithExt - the former exapandeed source file name with extenstion that was memory allocated
Output: None
*/
static void cleanSourceFileAssemblyData(codedWord instructions[], dataWord data[], int* pDC, 
                                      labelNode** pHeadOfLabelList, char* expandedSourceFileNameWithExt)
{
    /*reset all data structure for the next source code file assembling*/
    memset(instructions, 0, MAX_NUM_OF_INSTRUCTIONS*sizeof(codedWord));
    memset(data, 0, MAX_NUM_OF_DATA*sizeof(dataWord));
    *pDC = 0;
    
    /*free all allocated memory*/
    resetLabelTable(pHeadOfLabelList);
    free(expandedSourceFileNameWithExt);
}

/*
This program implements an assembler. The assembler gets a list of source files
written in assembly language and outputs object files for the defined machine language
as well as entries and externals symbols (aimed for the linker and loader which are 
out of scope of this program)
The assembler first runs over the source code (pre assembler) where it expands 
the macros defined in the code (if any) and generates the expanded source code.
Then the assembler runs 2 times on the expanded source code:
1. First run is used to build a symbol tables storing the address for every found symbol. 
   Partial coding of words is also done at this stage.
2. Second run is used to complete the coding of all words. 

If both runs go well, source code is now coded in a binary format (each word is 10 bits of data).
Final step is to convert the binary format to the defined 32 based code.

Input: list of source files haveing .as extenstion (<filename1>.as, <filename2>.as, ...)
Output: Files:
        
       1. <fileName>.am - the expanded .as assembly file after macros are replaced with their content
       2. <fileName>.ent - (optional) printout of all internal symbols and their adresses. If no such symbols, file is not created.
       3. <fileName>.ext - (optional) printout of all external symbols and their adresses. If no such symbols, file is not created.
       4. <fileName>.ob - the final object file coded in the given machine language
*/
int main(int argc, char *argv[])
{
    int i; /*source files counter*/
    int DC = 0; /*data counter*/
    codedWord instructions[MAX_NUM_OF_INSTRUCTIONS] = {0}; /*holds binary coded instructions*/
    dataWord data[MAX_NUM_OF_DATA] = {0}; /*holds the data */
    command commandArr[16]; /*holds all the supported commands and their parameters */
    labelNode *headOfLabelList = NULL; /*table of labels*/
    char *expandedSourceFileNameWithExt=NULL;/* pointer to the next exapanded source file name with extenstion*/
    int success;
    
    commandTableInitialize(commandArr);/*Done only once according to the rules of each command */
   
    /* for each of the input source code files */
    for (i=1;i<argc;i++)
    {
        runPreAsembler(argv[i], &expandedSourceFileNameWithExt);/* expand macros */
        
        /* try to do the first transition */
        success = runFirstPass(instructions, data, commandArr, &headOfLabelList, expandedSourceFileNameWithExt, &DC); 
        
        if(success)
        {
            /* first transition succeeded, try to do the second transition */
            runSecondPass(instructions, data, commandArr, &headOfLabelList, expandedSourceFileNameWithExt, DC);
        }    
        
        /* we finished hadling the current file. clean and prepare for next file (if exists)*/
        cleanSourceFileAssemblyData(instructions, data, &DC, &headOfLabelList, expandedSourceFileNameWithExt);
        
        
        
    }
    return 0;
}
