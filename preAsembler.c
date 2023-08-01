#include "preAsembler.h"

/*
Free all allocated memory for the macro table linked list
Input: pHead - A pointer to start of the list
Ouput: None
*/
static void resetMacroTable(macroNode **pHead)
{
    /*free all allocated nodes*/
    macroNode* tmp;

    while (*pHead != NULL)
    {
       /* in each step, move head to point to the next node before deleting the current */
       tmp = *pHead;
       *pHead = (*pHead)->nextMacro;
       free(tmp);
    }
}

/*
Add a new macro and its expansion code to the macro table.
New macro node is inserted at the head of the list
Input: line - the macro name 
       s - the source file (.as)
       pHead - pointer to the macro table  
Output: None
*/
static void addMacro(char *line, FILE *s, macroNode **pHead)
{
    char *token, temp[MAX_MACRO_CONTENT_LENGTH]={0};
    macroNode *ptr = (macroNode *) malloc(sizeof(macroNode));
    strtok(line, " \t");
    token = strtok(NULL, " \t");
    
    strcpy(ptr->macroName, token);
    fgets(line, MAX_LINE_LENGTH, s);
    /*store content of macro */
    while (strstr(line, "endmacro")==NULL)
    {
        strcat(temp,line);
        fgets(line, MAX_LINE_LENGTH, s);
    }
    strcpy(ptr->macroContent, temp);
    
    /*head will now point to the new macro */
    ptr->nextMacro = *pHead;
    *pHead = ptr;
}

/*
Checks whether this source code line is of MACRO type
Input: line - potential macro decleration
Output: 1 if it is a macro declaration and 0 otherwise
*/
static int isMacroDefinition(char *line)
{
    char *macroDefinition, checkLine[MAX_LINE_LENGTH];
    strcpy(checkLine, line);
    macroDefinition = strtok(checkLine, " \t");
    return (!(strcmp(macroDefinition, "macro")));
}

/*
Checks whether the provided line contains a MACRO which is familiar and 
appears in the MACRO table implemented by a linked list
Input: line - line of source code
       head - pointer to macro table linked list
Output: A pointer to the relevant entry of macro table or NULL if macro not found
*/
static macroNode* wasMacroDefined(char *line, macroNode *head)
{
    char *macroName, checkLine[MAX_LINE_LENGTH];
    macroNode *ptr;
    strcpy(checkLine, line);
    macroName = strtok(checkLine, " \t\n\r");
    if (macroName != NULL && strtok(NULL, " \t\n\r")==NULL)
    {
        for(ptr=head;ptr!=NULL;ptr=ptr->nextMacro)
        {
            if (!strcmp(ptr->macroName,macroName))
                return ptr;
        }
    }
    return NULL;
}

/*
Replaces the MACRO instanse in the source code with the content it was defined to be
Input: ptr - the macro table entry
       n - the .am expanded source file to be written to
Output: None
*/
static void switchMacro(macroNode *ptr, FILE *n)
{
    fputs(ptr->macroContent,n);
    fflush(n);
}

/*
Expandthe macros defined in the code (if any) and generate the expanded source code.
Assumption: Usage of macro must happen after its definition. Therefore when passing on the code
we collect macro definitions data and store them in a macro table (macroNode *head).
Then, when macro usage is found, we substitute the its instance with its content

Input: fileName - source code file name 
       pExpandedSourceFileNameWithExt - a pointer to the string that should be allocated inside 
       the function and be filled with the expanded source file name 
       Note: Caller should free this memory!
Output: None
*/
void runPreAsembler(char *sourceFileName, char** pExpandedSourceFileNameWithExt)
{
    FILE *source = NULL, *expanded = NULL;
    char* sourceFileNameWithExt;
    
    int  fd;
    macroNode *head = NULL;
    char line[MAX_LINE_LENGTH];
    /*int lastLine = FALSE;*/
    macroNode *temp = NULL;
    char* notEOF = NULL;
    sourceFileNameWithExt = (char*)calloc(1, strlen(sourceFileName) + 4); /*4 additional char for ".as\0"*/
    *pExpandedSourceFileNameWithExt = (char*)calloc(1, strlen(sourceFileName) + 4); /*4 additional char for ".am\0"*/
    
    strcpy(sourceFileNameWithExt, sourceFileName);
    strcpy(*pExpandedSourceFileNameWithExt, sourceFileName);
        
    strcat(sourceFileNameWithExt, ".as");
    strcat(*pExpandedSourceFileNameWithExt, ".am");
        
    source=fopen(sourceFileNameWithExt,"r");
    if (source==NULL)
    {
        printf("error in opening file\n");
    }

    if((fd = creat(*pExpandedSourceFileNameWithExt, PERMS))==-1)
    {
        printf("error in creating file");
    }
    expanded=fopen(*pExpandedSourceFileNameWithExt,"w");
    if (expanded==NULL)
    {
        printf("error in opening file");
    }
    
    /*gets(line, MAX_LINE_LENGTH, s);*/
    while ((notEOF = fgets(line, MAX_LINE_LENGTH, source)) != NULL)
    {
        temp = wasMacroDefined(line, head);
        if ( temp != NULL )
        {
            /* we found a usage of macro, expand it in the .am source file */
            switchMacro(temp, expanded);
        }
        else if ( isMacroDefinition(line) )
        {
            /*we found a macro declaration, store it with its content */
            addMacro(line, source, &head);
        }
        else 
        {
            fputs(line, expanded);
            fflush(expanded);/*Flush immediately to the file*/
        }
    }
    
    if(source)
    {
        fclose(source);
    }
    
    if(expanded)
    {
        fclose(expanded);
    }
    resetMacroTable(&head);
    free(sourceFileNameWithExt);
}


    
