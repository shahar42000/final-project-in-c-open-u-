#ifndef ASSEMBLERr_H
#define ASSEMBLERr_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define OCTAL_LENGTH 5

typedef struct {
    char commandName[WORD_SIZE];
    char binaryValue[5];
} Command;

typedef struct Label {
    char labelName[WORD_SIZE];
    int position;
    int type;
    struct Label *next;
} Label;

typedef struct WordTable {
    char wordName[WORD_SIZE];
    int position;
    int lineNumber;
    char binaryValue[16];
    struct WordTable *next;
} WordTable;

typedef struct ExternNode {
    char externName[WORD_SIZE];
    struct ExternNode *next;
} ExternNode;

typedef struct EntryNode {
    char entryName[WORD_SIZE];
    struct EntryNode *next;
} EntryNode;

Label* createLabelNode(char *name, int ic, int type);
void appendLabelNode(Label **head, char *name, int ic, int type);
WordTable* createWordNode(char *name, int position, int lineNumber);
void appendWordNode(WordTable **head, char *name, int position, int lineNumber);
ExternNode* createExternNode(char *name);
void appendExternNode(ExternNode **head, char *name);
EntryNode* createEntryNode(char *name);
void appendEntryNode(EntryNode **head, char *name);
void freeLabels(Label *head);
void freeWordTable(WordTable *head);
void freeExternNodes(ExternNode *head);
void freeEntryNodes(EntryNode *head);
void numberToBinary(int number, char output[15]);
void dataToBinary(int number, char output[12]);
char* getCommandBinaryValue(char* command);
Label* findLabel(Label* head, char* name);
void processFile(char *filename, WordTable **wordList, Label **labelList, ExternNode **externList, EntryNode **entryList);
void printWordList(WordTable *head);
void printLabelList(Label *head);
void printExternList(ExternNode *head);
void printEntryList(EntryNode *head);
int check_if_labael(char word[WORD_SIZE], Label **labelList);
int getHighestLineNumber(WordTable *head);
int is_in_entry(EntryNode* HeadEntry, char* word);
int is_in_extern(ExternNode* HeadExtern, char* word);
int check_sort(char op[WORD_SIZE], Label** labelList, ExternNode** externlist);
void change_to_binary(Label** labelList, EntryNode** HeadEntry, ExternNode** HeadExtern, WordTable **HeadWordList, int search_this_line);
void change_all_to_binary(Label** labelList, EntryNode** HeadEntry, ExternNode** HeadExtern, WordTable** HeadWordList, int max_line);
void binary_to_octal(WordTable* listWord, char* octal);
void writeWordTableToFile(WordTable *wordTable);
void processEntriesAndExport(WordTable *wordList, EntryNode *entryList, Label *labelList);
void check_and_export_externs(ExternNode *externList, WordTable *wordTable);

#endif 