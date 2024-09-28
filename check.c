#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro.h"
#include "error.h"
#include "check.h"
#include "assembler.h"



Command commands[] = {
    {"mov", "0000"},
    {"cmp", "0001"},
    {"add", "0010"},
    {"sub", "0011"},
    {"lea", "0100"},
    {"clr", "0101"},
    {"not", "0110"},
    {"inc", "0111"},
    {"dec", "1000"},
    {"jmp", "1001"},
    {"bne", "1010"},
    {"red", "1011"},
    {"prn", "1100"},
    {"jsr", "1101"},
    {"rts", "1110"},
    {"stop","1111"}
};



Label *createLabelNode( char *name, int ic, int type) {
    Label *newLabel = (Label *)malloc(sizeof(Label));
    if (newLabel == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    strncpy(newLabel->labelName, name, WORD_SIZE);
    newLabel->position = ic;
    newLabel->type = type;
    newLabel->next = NULL;
    return newLabel;
}

void appendLabelNode(Label **head,  char *name, int ic, int type) {
    Label *newLabel = createLabelNode(name, ic, type);
    if (newLabel == NULL) return;

    if (*head == NULL) {
        *head = newLabel;
    } else {
        Label *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        newLabel->position = ic; 
        current->next = newLabel;
    }
}


WordTable *createWordNode( char *name, int position, int lineNumber) {
    WordTable *newWord = (WordTable *)malloc(sizeof(WordTable));
    if (newWord == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    strncpy(newWord->wordName, name, WORD_SIZE);
    newWord->position = position;
    newWord->lineNumber = lineNumber;
    strncpy(newWord->binaryValue, "000000000000000", 16);  
    newWord->next = NULL;
    return newWord;
}


void appendWordNode(WordTable **head,  char *name, int position, int lineNumber) {
    WordTable *newWord = createWordNode(name, position, lineNumber);
    if (newWord == NULL) return;

    if (*head == NULL) {
        *head = newWord;
    } else {
        WordTable *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newWord;
    }
}

ExternNode *createExternNode( char *name) {
    ExternNode *newExtern = (ExternNode *)malloc(sizeof(ExternNode));
    if (newExtern == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    strncpy(newExtern->externName, name, WORD_SIZE);
    newExtern->next = NULL;
    return newExtern;
}

void appendExternNode(ExternNode **head,  char *name) {
    ExternNode *newExtern = createExternNode(name);
    if (newExtern == NULL) return;

    if (*head == NULL) {
        *head = newExtern;
    } else {
        ExternNode *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newExtern;
    }
}

EntryNode *createEntryNode( char *name) {
    EntryNode *newEntry = (EntryNode *)malloc(sizeof(EntryNode));
    if (newEntry == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    strncpy(newEntry->entryName, name, WORD_SIZE);
    newEntry->next = NULL;
    return newEntry;
}

void appendEntryNode(EntryNode **head,  char *name) {
    EntryNode *newEntry = createEntryNode(name);
    if (newEntry == NULL) return;

    if (*head == NULL) {
        *head = newEntry;
    } else {
        EntryNode *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEntry;
    }
}


void freeLabels(Label *head) {
    Label *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeWordTable(WordTable *head) {
    WordTable *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeExternNodes(ExternNode *head) {
    ExternNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeEntryNodes(EntryNode *head) {
    EntryNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}


void numberToBinary(int number , char output[15]) {/*get char and change the number to binary represent by string */
    int i  ;
    for (i=14; i >= 0; i--){
        if (number & 1) {
            output[i] = '1';
    } else {
        output[i] = '0';
                }
        number >>= 1;
    }
    
    
    output[15] = '\0';
}
 
 void dataToBinary(int number , char output[12]) {/*get char and change the number to binary represent by string */
    int i  ;
    for (i=11; i >= 0; i--){
        if (number & 1) {
            output[i] = '1';
    } else {
        output[i] = '0';
                }
        number >>= 1;
    }
    
    
    output[12] = '\0';
}

char* getCommandBinaryValue(char* command) {
    int i ; 
    for (i=0; i <= 15; i++) {
        if (strcmp(commands[i].commandName, command) == 0) {
            return commands[i].binaryValue;
        }
    }
    
    return NULL; 
}


Label* findLabel(Label* head, char* name) {
    Label* current = head;
    
    while (current != NULL) {
        if (strcmp(current->labelName, name) == 0) {
            
            return current;
        }
        current = current->next;
    }

   
    return NULL;
}

void processFile( char *filename, WordTable **wordList, Label **labelList, ExternNode **externList, EntryNode **entryList) {
    FILE *file;
    char line[LINE_SIZE];
    int lineNumber = 0;
    int wordPosition = 100;
    int i;
    char digit[WORD_SIZE];
    
    file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file.\n");
        return;
    }


    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \t\n,");
        while (token != NULL) {
            if (strcmp(token, ".entry") == 0) {
                token = strtok(NULL, " \t\n,");
                if (token) {
                    appendEntryNode(entryList, token);
                }
            } else if (strcmp(token, ".extern") == 0) {
                token = strtok(NULL, " \t\n,");
                if (token) {
                    appendExternNode(externList, token);
                }
            } else if (token[strlen(token) - 1] == ':') {
                token[strlen(token) - 1] = '\0';
                appendLabelNode(labelList, token, wordPosition, 3);
            } else if (strcmp(token, ".string") == 0) {
                token = strtok(NULL, " \t\n,");
                if (token) {
                    for (i = 0; i < strlen(token); i++) {
                        char letter[3];
                        if (token[i] != '"') {
                            letter[0] = '!';
                            letter[1] = token[i];
                            letter[2] = '\0';
                            appendWordNode(wordList, letter, wordPosition++, lineNumber++);
                        }
                    }
                appendWordNode(wordList, "$\\\0", wordPosition++, lineNumber++);
                }
            } else if (strcmp(token, ".data") == 0) {
                token = strtok(NULL, " \t\n,");
                while (token && (isdigit(token[0])|| isdigit(token[1]))) {
                    digit[0]='@';
                    strncpy(digit+1,token,WORD_SIZE);
                    digit[WORD_SIZE - 1] = '\0'; 
                    
                    appendWordNode(wordList, digit, wordPosition++, lineNumber++);
                    token = strtok(NULL, " \t\n,");
                }
                continue;
            } else if ((token[0] == 'r' && token[1] >= '1' && token[1] <= '7' && token[2] == '\0')||((token&&token[0]=='*')&&token[1]=='r')) {
                char *nextToken = strtok(NULL, " \t\n"); 
                char nextLine[LINE_SIZE];
                strncpy(nextLine, token, WORD_SIZE);
                if ((nextToken && nextToken[0] == 'r' && nextToken[1] >= '1' && nextToken[1] <= '7' && nextToken[2] == '\0')||((nextToken&&nextToken[0]=='*')&&nextToken[1]=='r')) {
                    strcat(nextLine, ","); 
                    strcat(nextLine, nextToken);
                    strcat(nextLine, "?");
                    nextLine[strlen(nextLine)]='\0';
                    appendWordNode(wordList, nextLine, wordPosition++, lineNumber);
                } else {
                    
                    appendWordNode(wordList, token, wordPosition++, lineNumber);
                    if (nextToken) {
                        token = nextToken;
                        continue;
                    }
                }
            } else {
                appendWordNode(wordList, token, wordPosition++, lineNumber);
            }
            token = strtok(NULL, " \t\n,");
        }
        lineNumber++;
    }

    fclose(file);
}

void printWordList(WordTable *head) {
    WordTable *current = head;
    while (current != NULL) {
        printf("Word: %s, Position: %d, Line Number: %d , binaryvalue : %s \n", current->wordName, current->position, current->lineNumber,current->binaryValue);
        current = current->next;
    }
}

void printLabelList(Label *head) {
    Label *current = head;
    while (current != NULL) {
        printf("Label: %s, Position: %d, Type: %d\n", current->labelName, current->position, current->type);
        current = current->next;
    }
}

void printExternList(ExternNode *head) {
    ExternNode *current = head;
    while (current != NULL) {
        printf("Extern: %s \n", current->externName);
        current = current->next;
    }
}

void printEntryList(EntryNode *head) {
    EntryNode *current = head;
    while (current != NULL) {
        printf("Entry: %s \n", current->entryName);
        current = current->next;
    }
}

int check_if_labael(char word [WORD_SIZE],Label **labelList ){

    struct Label *current = *labelList ;
    while (current)
    {
        if(strcmp(current->labelName,word)==0){
            return 1 ;
        }
        current=current->next ;

    }
    return 0 ;

}

int getHighestLineNumber(WordTable *head) {
    int highestLineNumber = 0;
    WordTable *current = head;

    while (current != NULL) {
        if (current->lineNumber > highestLineNumber) {
            highestLineNumber = current->lineNumber;
        }
        current = current->next;
    }

    return highestLineNumber;
}
int is_in_entry(EntryNode* HeadEntry, char* word) {
    EntryNode* current = HeadEntry;
    while (current != NULL) {
        if (strcmp(current->entryName, word) == 0) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}

int is_in_extern(ExternNode* HeadExtern, char* word) {
    ExternNode* current = HeadExtern;
    while (current != NULL) {
        if (strcmp(current->externName, word) == 0) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}

int check_sort (char op [WORD_SIZE],Label** labelList ,ExternNode** externlist){

    if(op[0]=='#'){
        return 0 ; 
    }
    if(op[0]=='*'){
        return 2 ;
    }
    if(op[0]=='r'){
        return 3 ;
    }
    if((check_if_labael(op,labelList)==1)){
        return 1 ;
    }
    if((is_in_extern(*externlist,op)==1)){
            return 5;
    }
    else {
        return 6;
    }
    
    
}




void change_to_binary(Label** labelList,EntryNode** HeadEntry,ExternNode** HeadExtern,WordTable **HeadWordList,int search_this_line){
    char *action_binary;
    char action[WORD_SIZE]="\0";
    char op1[WORD_SIZE]="\0";
    char op2[WORD_SIZE]="\0";
    char binary_slot[16];
    struct WordTable* current = *HeadWordList;
    Label* templabel = NULL;
    int word_counter = 1;
    int found = 0;
    int i = 0;
    char* nextToken;
    struct WordTable* temp1 = NULL;
    struct WordTable* temp2 = NULL;
    struct WordTable* temp3 = NULL;
    int current_num;
    int length_of_op1; 
    char op1_copy[WORD_SIZE];
    


    while (current) {
        if (current->lineNumber == search_this_line) {
            if (word_counter == 1) {
                strncpy(action, current->wordName,WORD_SIZE);
                temp1 = current;
            }
            if (word_counter == 2) {
                strncpy(op1, current->wordName,WORD_SIZE);
                temp2 = current;
            }
            if (word_counter == 3) {
                strncpy(op2, current->wordName,WORD_SIZE);
                temp3 = current;
            }
            found = 1;
            word_counter++;
        }
        current = current->next;
    }
    if (found == 0 || temp1 == NULL) {
        return;
    }
    length_of_op1 = strlen(op1);
    
    
    
    
    if (action[0] != '\0') {
    
        if (action[0] == '!') {
            current_num = (int)(action[1]);
            numberToBinary(current_num, binary_slot);
            strncpy(temp1->binaryValue, binary_slot, 15);
    }   
        else if (action[0] == '@') {
            current_num = atoi(action + 1);
            numberToBinary(current_num, binary_slot);
            strncpy(temp1->binaryValue, binary_slot, 15);
        }
        else if (action[0] == '$') {
            return;}

        else{
        
        
        
        strncpy(op1_copy,op1,WORD_SIZE);
        temp1->binaryValue[12]='1';/* binary 1 to are a */
        action_binary=getCommandBinaryValue(action);
        i=0;
        while(i<strlen(action_binary))
            { 
                temp1->binaryValue[i]=action_binary[i];
                i++;
                }
        if((action[0] != '\0')&&(op1[0]=='\0')&&(op2[0]=='\0')){

                temp1->binaryValue[12]='1';
        
            }
        
        else if(op2[0]=='\0'&&op1!='\0'){/* enter to the action binary code the values needed when there are 2 regesiters  */
            if((op1[length_of_op1-1]) == '?'){
                    char *token = strtok(op1_copy, ",");
                if(check_sort(token,labelList,HeadExtern)==2){
                    temp1->binaryValue[5]='1';
                }
                if(check_sort(token,labelList,HeadExtern)==3){
                    temp1->binaryValue[4]='1';
                }
                nextToken = strtok(NULL, " \t\n,");
                if(check_sort(nextToken,labelList,HeadExtern)==2){
                    temp1->binaryValue[9]='1';
                }
                if(check_sort(nextToken,labelList,HeadExtern)==3){
                    temp1->binaryValue[8]='1';
                }
            }
        
            else {
            
                if(check_sort(op1,labelList,HeadExtern)==0){  /* enter the right binary code when the action can get only 1 opretor */
                    temp1->binaryValue[11]='1';
            }
                if((check_sort(op1,labelList,HeadExtern)==1)||(check_sort(op1,labelList,HeadExtern)==5)){
                    temp1->binaryValue[10]='1';
            }
                if(check_sort(op1,labelList,HeadExtern)==2){
                    temp1->binaryValue[9]='1';
            }
                if(check_sort(op1,labelList,HeadExtern)==3){
                    temp1->binaryValue[8]='1';
            }
            
            }
        }
        
        else if(op2!='\0'&&op1[0]!='\0'){/* enter to the action binary code the values needed when there are 2 regesiters  */
            /* ----------------------------check binary code for op1 in action--------------------------------*/
                if(check_sort(op1,labelList,HeadExtern)==0){  
                    temp1->binaryValue[7]='1';
            }
                if((check_sort(op1,labelList,HeadExtern)==1)||(check_sort(op1,labelList,HeadExtern)==5)){
                    temp1->binaryValue[6]='1';
            }
                if(check_sort(op1,labelList,HeadExtern)==2){
                    temp1->binaryValue[5]='1';
            }
                if(check_sort(op1,labelList,HeadExtern)==3){
                    temp1->binaryValue[4]='1';
            }
               /* ----------------------------check binary code for op2 in action--------------------------------*/
                if(check_sort(op2,labelList,HeadExtern)==0){  
                    temp1->binaryValue[11]='1';
            }
                if((check_sort(op2,labelList,HeadExtern)==1)||(check_sort(op2,labelList,HeadExtern)==5)){
                    temp1->binaryValue[10]='1';
            }
                if(check_sort(op2,labelList,HeadExtern)==2){
                    temp1->binaryValue[9]='1';
            }
                if(check_sort(op2,labelList,HeadExtern)==3){
                    temp1->binaryValue[8]='1';
            }
            
        }
    
    }
   
   /*only 1 operend++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        if((op1[0]!='\0')&&(op2[0]=='\0')){

            if(op1[length_of_op1-1] != '?'){

                if(check_sort(op1,labelList,HeadExtern)==0){  
                    
                    dataToBinary((atoi(op1+1)),binary_slot);
                    temp2->binaryValue[12]='1';
                    i=0;
                    while(i<=11){ 
                        temp2->binaryValue[(11-i)]=binary_slot[(11-i)];/* fix the placement of the binary */
                        i++;
                }
                    
            }
                
            if(is_in_entry(*HeadEntry,op1)==1){
                temp2->binaryValue[13]='1';
                }

            if(is_in_extern(*HeadExtern,op1)==1){
                temp2->binaryValue[14]='1';
            }
            if(check_sort(op1,labelList,HeadExtern)==1){
                templabel=findLabel(*labelList,op1);
                temp2->binaryValue[13]='1';
                dataToBinary(templabel->position,binary_slot);
                i=0;
                while(i<11){ 
                    temp2->binaryValue[(11-i)]=binary_slot[(11-i)];
                    i++;
            }
            }
            
            
            
            
                if(check_sort(op1,labelList,HeadExtern)==2){
                    temp2->binaryValue[12]='1';
                    numberToBinary(atoi(op1+2),binary_slot); /*add the correct place to the binary last updated16.9*/
                    i = 0 ;
                    while(i<3){ 
                        temp2->binaryValue[i+9]=binary_slot[12+i];
                        i++;
            }
             }
                if(check_sort(op1,labelList,HeadExtern)==3){
                    temp2->binaryValue[12]='1';
                    numberToBinary(atoi(op1+1),binary_slot);
                    i=0 ; 
                    while(i<3){ 
                        (temp2->binaryValue[i+9])=binary_slot[i+12];
                        i++;
                        
            }
            }


        }


        }
   
   /*op1  and op2 -------------------------------------------------------------------------------*/
   
     if((op1[0]!='\0'&&(op2[0]!='\0'))){
        if(op1[length_of_op1-1] != '?'){

            if(check_sort(op1,labelList,HeadExtern)==0){  
                    
                    dataToBinary((atoi(op1+1)),binary_slot);
                    temp2->binaryValue[12]='1';
                    i = 0 ;
                    while(i<=11){ 
                        temp2->binaryValue[(11-i)]=binary_slot[(11-i)];/* fix the placement of the binary */
                        i++;
                }
                    
            }
                
            if(is_in_entry(*HeadEntry,op1)==1){
                temp2->binaryValue[13]='1';
                }

            if(is_in_extern(*HeadExtern,op1)==1){
                temp2->binaryValue[14]='1';
            }
            if(check_sort(op1,labelList,HeadExtern)==1){
                templabel=findLabel(*labelList,op1);
                temp2->binaryValue[13]='1';
                dataToBinary(templabel->position,binary_slot);
                 i=0;
                while(i<=11){ 
                    temp2->binaryValue[(11-i)]=binary_slot[(11-i)];
                    i++;
            }
            }
            
            
            
            
                if(check_sort(op1,labelList,HeadExtern)==2){
                    temp2->binaryValue[12]='1';
                    numberToBinary(atoi(op1+2),binary_slot); /*add the correct place to the binary last updated16.9*/
                    i = 0 ;
                    while(i<3){ 
                        temp2->binaryValue[i+6]=binary_slot[12+i];
                        i++;
            }
             }
                if(check_sort(op1,labelList,HeadExtern)==3){
                    temp2->binaryValue[12]='1';
                    numberToBinary(atoi(op1+1),binary_slot);
                    i=0 ; 
                    while(i<3){ 
                        (temp2->binaryValue[i+6])=binary_slot[i+12];
                        i++;
                        
            }
            }
            /*op2 -----------------------------------------------------------------------------------------*/
            if(check_sort(op2,labelList,HeadExtern)==0){  
                    
                    dataToBinary((atoi(op2+1)),binary_slot);
                    temp3->binaryValue[12]='1';
                    i = 0 ;
                    while(i<=11){ 
                        temp3->binaryValue[(11-i)]=binary_slot[(11-i)];/* fix the placement of the binary */
                        i++;
                }
                    
            }
                
            if(is_in_entry(*HeadEntry,op2)==1){
                temp3->binaryValue[13]='1';
                }

            if(is_in_extern(*HeadExtern,op2)==1){
                temp3->binaryValue[14]='1';
            }
            if(check_sort(op2,labelList,HeadExtern)==1){
                templabel=findLabel(*labelList,op2);
                temp3->binaryValue[13]='1';
                dataToBinary(templabel->position,binary_slot);
                i=0;
                while(i<11){ 
                    temp3->binaryValue[(11-i)]=binary_slot[(11-i)];
                    i++;
            }
            }
            
            
            
            
                if(check_sort(op2,labelList,HeadExtern)==2){
                    temp3->binaryValue[12]='1';
                    numberToBinary(atoi(op2+2),binary_slot); /*add the correct place to the binary last updated16.9*/
                    i = 0 ;
                    while(i<3){ 
                        temp3->binaryValue[i+9]=binary_slot[12+i];
                        i++;
            }
             }
                if(check_sort(op2,labelList,HeadExtern)==3){
                    temp3->binaryValue[12]='1';
                    numberToBinary(atoi(op2+1),binary_slot);
                    i=0 ; 
                    while(i<3){ 
                        (temp3->binaryValue[i+9])=binary_slot[i+12];
                        i++;
                        
            }
            }
            
    }
    }
/*when there are 2 registers --------------------------------------------------------------------------------------------------------*/
        
        if((op1[strlen(op1)-1]) == '?'){
        char *nextToken ; 
        char *token;
        token = strtok(op1,", /t");
        temp2->binaryValue[12]='1';
        if(check_sort(token,labelList,HeadExtern)==2){
            numberToBinary(atoi(token+2),binary_slot);
        }
        if(check_sort(token,labelList,HeadExtern)==3){
            numberToBinary(atoi(token+1),binary_slot);
        }
        i = 0 ;
        while(i<3){ 
            temp2->binaryValue[i+6]=binary_slot[i+12];
            i++;
        }
        nextToken=strtok(NULL,"?");
        
        if(check_sort(nextToken,labelList,HeadExtern)==2){
                    
                    numberToBinary(atoi(nextToken+2),binary_slot); /*add the correct place to the binary last updated16.9*/
                        i = 0 ;
                        while(i<3){ 
                        temp2->binaryValue[i+9]=binary_slot[i+12];
                        i++;
            }
             }
            if(check_sort(nextToken,labelList,HeadExtern)==3){
                    
                    numberToBinary(atoi(nextToken+1),binary_slot);
                        i=0;
                        while(i<3){ 
                        temp2->binaryValue[i+9]=binary_slot[i+12];
                        i++;
            }
            }    
    
    
    }        

    }
}

void change_all_to_binary(Label** labelList, EntryNode** HeadEntry, ExternNode** HeadExtern, WordTable** HeadWordList, int max_line) {
    int current_line;

    
    for (current_line = 0; current_line <= max_line; current_line++) {
        
        change_to_binary(labelList, HeadEntry, HeadExtern, HeadWordList, current_line);
    }
}


void binary_to_octal(WordTable* listWord, char* octal)
{
    int decimal = 0;
    int power = 1;
    int len = strlen(listWord->binaryValue);
    int index = OCTAL_LENGTH - 1;
    int i;

    for (i = len - 1; i >= 0; i--)
    {
        if (listWord->binaryValue[i] == '1')
        {
            decimal += power;
        }
        power *= 2;
    }
    memset(octal, '0', OCTAL_LENGTH);
    octal[OCTAL_LENGTH] = '\0';

    
    if (decimal == 0)
    {
        
    }
    else
    {
        while (decimal > 0 && index >= 0)
        {
            octal[index--] = (decimal % 8) + '0';
            decimal /= 8;
        }
    }
}
void writeWordTableToFile(WordTable *wordTable) {
    char *filename = "ps.ob"; 
    WordTable *current = wordTable;  
    char octal[OCTAL_LENGTH + 1]; 
    if(current==NULL)
    {   
        return;
    }
else{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    while (current != NULL) {
        binary_to_octal(current, octal);
        fprintf(file, "0%d %s\n", current->position, octal);
        current = current->next;
    }
    
   
}
}
void processEntriesAndExport(WordTable *wordList, EntryNode *entryList, Label *labelList) {
    FILE *entryFile;
    EntryNode *currentEntry=entryList;
    Label *currentLabel;
    if(currentEntry == NULL)
    {
        return;
    }
    else{
        entryFile = fopen("ps.ent", "w");
    
    if (entryFile == NULL) {
        printf("Error opening ps.ent file for writing.\n");
        return;
    }
    
    for (currentEntry = entryList; currentEntry != NULL; currentEntry = currentEntry->next) {
        for (currentLabel = labelList; currentLabel != NULL; currentLabel = currentLabel->next) {
            if (strcmp(currentEntry->entryName, currentLabel->labelName) == 0) {
                
                fprintf(entryFile, "%s %04d\n", currentLabel->labelName, currentLabel->position);
                break;
            }
        
        }
    }
    }
    fclose(entryFile);
    printf("Entry information has been exported to ps.ent\n");
}
void check_and_export_externs(ExternNode *externList, WordTable *wordTable) {
    ExternNode *currentExtern = externList;
    if(currentExtern == NULL)
    {
        return;
    }
    else{
    FILE *outputFile = fopen("ps.ext", "w");
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        return;
    }

    while (currentExtern != NULL) {
        WordTable *currentWord = wordTable;
        while (currentWord != NULL) {
            if (strcmp(currentExtern->externName, currentWord->wordName) == 0) {
                fprintf(outputFile, "%s %04d\n", currentExtern->externName, currentWord->position);
            }
            currentWord = currentWord->next;
        }
        currentExtern = currentExtern->next;
    }

    fclose(outputFile);
}
}