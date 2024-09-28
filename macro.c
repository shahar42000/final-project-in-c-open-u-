/*this is the pre prossecor part */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "error.h"
#include "check.h"
#include "assembler.h"




/*function for create the nodes*/
MacroNode* createNode(char* line) {
    MacroNode* newNode = (MacroNode*)malloc(sizeof(MacroNode));
    if (newNode == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    strncpy(newNode->line, line, LINE_SIZE);
    newNode->next = NULL;
    return newNode;
}

/*Function to append a node to the linked list*/
void appendNode(MacroList* head, char* line, char* CurrentMacro) {
    MacroNode *newNode = createNode(line);

    /* Find the correct macro by name*/
    MacroList *currentList = head;
    while (currentList != NULL) {
        if (strcmp(currentList->name, CurrentMacro) == 0) {
            /* Found the correct macro, now append the node*/
            if (currentList->first_order == NULL) {
                    currentList->first_order = newNode;
            } 
            else {
                MacroNode *current = currentList->first_order;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newNode;
            }
            return; /* Node appended, exit function*/ 
        }
        currentList = currentList->next_macro_call;
    }
}
/*print the data of the list , move every node and print the link list
*Not necessary for the project its just for the check
*/
void printMacroLines(MacroNode* head) {
    MacroNode* current = head;
    while (current != NULL) {
        printf("%s", current->line);
        current = current->next;
    }
}

/*A function that frees the memory of all nodes in the linked list.*/
void freeNodeList(MacroNode* head) {
    MacroNode* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

/*skip blank spaces tabs add by shahar */
char* skip_blanks(char* ptr){
    while(((*ptr) == ' ') || ((*ptr) == '\t') || ((*ptr) == '\n')){
        ptr++;
    }
    return ptr;
}

/*----------------------------------------------------------------------------------------------------------*/

/* Function to create a new macro list*/
MacroList* createMacroList(char* macro_call) {
    MacroList* newList = (MacroList*)malloc(sizeof(MacroList));
    if (newList == NULL) {
        printf("Error allocating memory for macro list\n");
        exit(1);
    }
    strncpy(newList->name, macro_call, WORD_SIZE);
    newList->first_order = NULL;
    newList->next_macro_call = NULL;
    return newList;
}

/*Function to append to the macro list*/
void appendMacroList(MacroList** head, char* macro_call) {
    MacroList* newList = (MacroList*)malloc(sizeof(MacroList));
    if (newList == NULL) {
        printf("Error allocating memory for macro list\n");
        exit(1);
    }
    strncpy(newList->name, macro_call, WORD_SIZE );
    newList->first_order = NULL;
    newList->next_macro_call = NULL;

    if (*head == NULL) {
        *head = newList;
    } else {
        MacroList* current = *head;
        while (current->next_macro_call != NULL) {
            current = current->next_macro_call;
        }
        current->next_macro_call = newList;
    }
}

/*Function to print all macros like m_macr all what we defined*/
void printAllMacros(MacroList* head) {
    MacroList* currentMacro = head;
    MacroNode* currentLine ;
    while (currentMacro) {
        printf("Macro Name: %s\n", currentMacro->name);
        currentLine = currentMacro->first_order;
        if (currentLine == NULL) {
            printf("\t(No content)\n");
        } else {
            while (currentLine) {
                printf("%s", currentLine->line);
                currentLine = currentLine->next;
            }
        }
        printf("\n");
        currentMacro = currentMacro->next_macro_call;
    }
}

/*Function to free the memory of the entire macro list*/
void freeMacroList(MacroList* head) {
    MacroList* currentMacro = head;
    MacroList* tmpMacro;
    while (currentMacro != NULL) {
        freeNodeList(currentMacro->first_order);
        tmpMacro = currentMacro;
        currentMacro = currentMacro->next_macro_call;
        free(tmpMacro);
    }
}

/*count how many macros we have */
int countMacros(MacroList* head) {
    int count = 0;
    MacroList* current = head;
    while (current != NULL) {
        count++;
        current = current->next_macro_call;
    }
    return count;
}

/*----------------------------------------------------------------------------------------------------------*/
void readFileAndInsertMacros(MacroList** List_head, MacroNode** Macro_head,  char* filename) 
{
    /*var to check if we are in the macro or not*/

    char c;
    char word[WORD_SIZE] = "";
    char line[LINE_SIZE] = "";
    int insideMacro = 0;
    int first_word_after_macro = 0;
    char current_macro[WORD_SIZE]="";
    /*open the file*/
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    
    
    /*read line after line*/
    while ((c = fgetc(file)) != EOF) {
        if((c=='\n')&&(insideMacro==1)&&((strlen(line) > 1)||(strlen(word)>1))){
            strcat(line," ");
            strcat(line, word );/* dosent add space */
            appendNode(*List_head, line,current_macro);
            memset(line, 0, sizeof(line));
        }
        
        if ((c == ' ') || (c == '\t')) {
            if ((strlen(word) > 1) && (insideMacro == 1)) {
                strcat(line," ");
                strcat(line, word );/* dosent add space */
            }
            memset(word, 0, sizeof(word));
        }
        if ((first_word_after_macro == 1) && (c == '\n')) {
            appendMacroList(List_head, word);
            strcat(current_macro,word);
            memset(word, 0, sizeof(word)); 
            first_word_after_macro = 0; 
            insideMacro = 1;
        }
        
        
        else if ((c != ' ') && (c != '\t')) {
            strncat(word, (char *)&c, 1);
        }
        
        if ((strcmp(word, MACRO_START)) == 0) {
            
            first_word_after_macro = 1;
            memset(word, 0, sizeof(word)); 
        }
        
        if ((strcmp(word, MACRO_END)) == 0) {
            insideMacro = 0;
             memset(word, 0, sizeof(word));
            memset(current_macro, 0, sizeof(current_macro));
        }
        
        
        
        if (c == '\n') {
            
            memset(word, 0, sizeof(word));

        }
    }
    /*close the file*/
    fclose(file);
}


int find_in_list(MacroList* head, char* word) 
{
    MacroList* currentList = head;
    while (currentList != NULL) 
    {
        if (strcmp(currentList->name, word) == 0) 
        {
            return 1; 
        }
        currentList = currentList->next_macro_call;
    }
    return 0;
}
/*this funtion search for the macro and replace it with the saved orders  */
void copy_paste_files(MacroList *List_head, char *word, FILE *output_file) {
    MacroList *current = List_head;
    while (current != NULL) {
        if (strcmp(current->name, word) == 0) {
            MacroNode *node = current->first_order;
            while (node != NULL) {
                fputs(node->line, output_file);
                node = node->next;
                fputc('\n',output_file);
            }
            return;
        }
        current = current->next_macro_call;
    }
}

/*make a new file after all the changes */
void add_to_file(MacroList* List_head, char *input_file, char *output_file) {
    int last_round = 0 ; 
    int inside_macro = 0;
    char c;
    char word[WORD_SIZE] = "";
    char line[LINE_SIZE] = "";
    FILE *input_file_new = fopen(input_file, "r");
    FILE *output_file_new = fopen(output_file, "w");

    if (input_file_new == NULL || output_file_new == NULL) {
        printf("Error opening source or destination file\n");
        return;
    }
    
    while (((c = fgetc(input_file_new)) != EOF)&&(last_round=1)) {
        if(c==EOF){
            last_round=1 ;
        }    
        /*add char to word*/    
        if ((c != ' ') && (c != '\t')&&(c!='\n')) {
            strncat(word, (char *)&c, 1);
        }
        /*find start of macro */
        if ((strcmp(word, MACRO_START) == 0)) {
            inside_macro = 1;
            memset(word, 0, sizeof(word));
            memset(line, 0, sizeof(line));   
        }
        
        
        /*find end of macro */
        if ((strcmp(word, MACRO_END) == 0)) {
            inside_macro = 0;
            memset(word, 0, sizeof(word));
            memset(line, 0, sizeof(line));
        }


        /*paste to file when it found macro */        
        if ((find_in_list(List_head, word) == 1) && (inside_macro == 0)&&((c=='\n')||(c=='\t')||(c==' '))) {

            copy_paste_files(List_head, word, output_file_new);
            memset(word, 0, sizeof(word));
            memset(line, 0, sizeof(line)); 
            
        }

        /*----------------------------------------------------------------------------------*/
        if ((c=='\n')&&(inside_macro==0)&&(strlen(word)>=1)) {
            strcat(line, " ");
            strcat(line, word);
            memset(word, 0, sizeof(word));  
            fputs(line, output_file_new);
            fputc('\n', output_file_new);
            memset(line, 0, sizeof(line));  
            }
        
        if (((c == ' ') || (c == '\t') || (c == '\n')) && (strlen(word) > 0)) {
            strcat(line, " ");
            strcat(line, word);
            memset(word, 0, sizeof(word));  

            
            if (c == '\n' && inside_macro == 0) {
                fputs(line, output_file_new);
                
                memset(line, 0, sizeof(line));  
            }
        }
    }
            
            strcat(line, " ");
            strcat(line, word);
            memset(word, 0, sizeof(word));  
            fputs(line, output_file_new);
            fputc('\n', output_file_new);
            memset(line, 0, sizeof(line));
    
    fclose(input_file_new);
    fclose(output_file_new);
}
