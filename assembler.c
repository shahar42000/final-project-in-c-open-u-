lude "error.h"
#include "check.h"
#include "assembler.h"



/*first process to macro*/
void processMacrosFromFile(char *filename) { 
    char *output_filename = "ps.am"; 
    MacroList *List_head = NULL; 
    MacroNode *Macro_head = NULL; 
    readFileAndInsertMacros(&List_head, &Macro_head, filename); 
    add_to_file(List_head, filename, output_filename); 
    freeMacroList(List_head);
    freeNodeList(Macro_head);

}
/*second process to binar and octal*/
void processbinarFromFile(char *filename) { 
    WordTable *wordList = NULL;
    Label *labelList = NULL;
    ExternNode *externList = NULL;
    EntryNode *entryList = NULL;
    int max = 0;
    processFile(filename, &wordList, &labelList, &externList, &entryList);
    max = getHighestLineNumber(wordList);
    change_all_to_binary(&labelList, &entryList, &externList, &wordList, max);
    writeWordTableToFile(wordList);
    processEntriesAndExport(wordList, entryList, labelList);
    check_and_export_externs(externList, wordList);
    freeLabels(labelList);
    freeWordTable(wordList);
    freeExternNodes(externList);
    freeEntryNodes(entryList);
}

int main(int argc, char *argv[]) {
    char *filename;
    char *output_filename = "ps.am";
    int errorCheck = 0;
    int errorCheck2 = 0;
    /*check for exist file*/
    if  (argc < 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1; 
    } 

    filename = argv[1]; 
    /*check if error for start first process*/ 
    errorCheck = checkFile1ForErrors(filename);
    if (errorCheck == 0 )
    {   
        printf("%s\n", "Starting preassembler");
        processMacrosFromFile(filename);
        /*check if error for start second process*/  
        errorCheck2 = checkFile2ForErrors(output_filename);
        if (errorCheck2 == 0) {
            printf("%s\n", "Starting binary processing");
            processbinarFromFile(output_filename); 
        } 
        else 
        {   /*if find error*/
            printf("Error in file '%s'\n", output_filename );
            printf("%s\n", "Does not continue the process");
        }
    } else
    { /*if find error*/
        printf("Error in file '%s'\n", filename);
        printf("%s\n", "Does not continue the process");
    }

    return 0; 
}

