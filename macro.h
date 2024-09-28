#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 82
#define WORD_SIZE 15
#define MACRO_START "macr"
#define MACRO_END "endmacr"

typedef struct MacroNode {
    char line[LINE_SIZE];
    struct MacroNode *next;
} MacroNode;

typedef struct MacroList {
    char name[WORD_SIZE];
    struct MacroList* next_macro_call;
    struct MacroNode* first_order;
} MacroList;

MacroNode* createNode(char* line);
void appendNode(MacroList* head, char* line, char* CurrentMacro);
void printMacroLines(MacroNode* head);
void freeNodeList(MacroNode* head);
char* skip_blanks(char* ptr);
MacroList* createMacroList(char* macro_call);
void appendMacroList(MacroList** head, char* macro_call);
void printAllMacros(MacroList* head);
void freeMacroList(MacroList* head);
int countMacros(MacroList* head);
void readFileAndInsertMacros(MacroList** List_head, MacroNode** Macro_head, char* filename);
int find_in_list(MacroList* head, char* word);
void copy_paste_files(MacroList *List_head, char *word, FILE *output_file);
void add_to_file(MacroList* List_head, char *input_file, char *output_file);

#endif 
