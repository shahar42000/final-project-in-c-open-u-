#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro.h"
#include "error.h"
#include "check.h"
#include "assembler.h"

/* Find if this is opcode */
int is_opcode(char *word) 
{
    int i;  
    char *opcodes[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    for (i = 0; i < 16; i++) {
        if (strcmp(word, opcodes[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Find if this is instruction */
int is_instruction(char *word) {
    int i;  
    char *instructions[] = {".data", ".string", ".entry", ".extern"};
    for (i = 0; i < 4; i++) {
        if (strcmp(word, instructions[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Check if word already appeared */
int macro_already_appeared(char *macro, char macros[][WORD_SIZE], int macro_count) {
    int i; 
    for (i = 0; i < macro_count; i++) {
        if (strcmp(macro, macros[i]) == 0) {
            return 1;  /* if already exist*/
        }
    }
    return 0;
}
int label_already_appeared(char *label, char labels[][WORD_SIZE], int label_count) {
    int i; 
    for (i = 0; i < label_count; i++) {
        if (strcmp(label, labels[i]) == 0) {
            return 1;  /* Label already exists */
        }
    }
    return 0;  /* Label not found */
}

int is_valid_label(char *label) {
    int i;
    for (i = 0; i < strlen(label); i++) {
        if (!isalpha(label[i])) {
            return 0;
        }
    }
    return 1;
}

void print_error(int line_num, char *message) {
    printf(RED "Line %d: %s\n" RESET, line_num, message);
}

int is_valid_string(char *str) {
    int i;  
    for (i = 0; i < strlen(str); i++) {
        if (!isalpha(str[i]) && !isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}

int checkFile1ForErrors(char *filename)
{
    char line[LINE_SIZE + 2]; 
    int line_num = 0;
    char macros[MAX_WORDS][WORD_SIZE];  /* save for know if exist*/
    int macro_count = 0;
    char *token;
    int found_macr;
    int word_count_after_macr;
    int error_found = 0; 
    

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1; 
    }

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        found_macr = 0;  
        word_count_after_macr = 0;

        /* check if long */
        if (line[strlen(line) - 1] != '\n' && strlen(line) >= LINE_SIZE) {
            print_error(line_num, "Line is too long");
            error_found = 1; 
            continue;
        }

        token = strtok(line, " \t\n");

        while (token != NULL) {
            /* Check if the word is longer than WORD_SIZE */
            if (strlen(token) > WORD_SIZE) {
                if (token[strlen(token) - 1] == ':') {
                    print_error(line_num, "Label too long");
                } else {
                    print_error(line_num, "Word too long");
                }
                error_found = 1; 
            }

            /* if its macr */ 
            if (strcmp(token, MACRO_START) == 0) {
                found_macr = 1;  
                word_count_after_macr = 0; 

                /* check if after macr came opcode or instruction */ 
                token = strtok(NULL, " \t\n");
                if (token != NULL) {
                    word_count_after_macr++;
                    /* macr after macr */
                    if (strcmp(token, MACRO_START) == 0) {
                        print_error(line_num, "invalide macro - macr after macr");
                        error_found = 1; 
                    }  
                   
                    if (macro_already_appeared(token, macros, macro_count)) {
                        print_error(line_num, "This macro has already been defined.");
                        error_found = 1; 
                        break;
                    }
                    strcpy(macros[macro_count], token);
                    macro_count++;

                    if (is_opcode(token)) {
                        print_error(line_num, "The word after 'macr' cannot be an opcode.");
                        error_found = 1; 
                        break;
                    }
                    if (is_instruction(token)) {
                        print_error(line_num, "The word after 'macr' cannot be an instruction.");
                        error_found = 1; 
                        break;
                    }
                }
            } 
            else if (found_macr) {
                word_count_after_macr++;
                if (word_count_after_macr > 1) {
                    print_error(line_num, "extra text in macro initialization");
                    error_found = 1; 
                    break;
                }
            }

            if (strcmp(token, MACRO_END) == 0) {
                token = strtok(NULL, " \t\n");  
                if (token != NULL) {
                    print_error(line_num, "extra text in end macro initialization");
                    error_found = 1; 
                    break;
                }
                found_macr = 0; 
            }

            token = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    return error_found; 
}

int checkFile2ForErrors(char *filename) {
    char line[LINE_SIZE];
    int line_num = 0;
    int error_found = 0; 
    char line_copy[LINE_SIZE];
    char labels[MAX_WORDS][WORD_SIZE];  
    int label_count = 0;  
    char *first_token; 
    char *token;
    char* pointer_to_data ; 
    int is_a_int =0 ;
    int is_a_comma = 0 ;
    int is_a_space = 0 ;
    int is_a_dot = 0;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1; 
    }
    
     
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        strcpy(line_copy,line);
    
        if (line[strlen(line) - 1] != '\n' && strlen(line) >= LINE_SIZE - 1) {
            print_error(line_num, "Line is too long");
            error_found = 1; 
            continue;
        }

        first_token = strtok(line, " \t\n");

         /* Check if the first token is a label */
        if (first_token != NULL && first_token[strlen(first_token) - 1] == ':') {
            first_token[strlen(first_token) - 1] = '\0';  

            if (is_opcode(first_token) || is_instruction(first_token)) {
                print_error(line_num, "Label cannot be an opcode or instruction");
                error_found = 1;
            } else if (!is_valid_label(first_token)) {
                print_error(line_num, "Label contains invalid characters");
                error_found = 1;
            } else if (label_already_appeared(first_token, labels, label_count)) {
                print_error(line_num, "Label already exist");
                error_found = 1;
            } else {
                strcpy(labels[label_count], first_token);  
                label_count++;  
            }
            first_token = strtok(NULL, " \t\n");
        }

        if (first_token != NULL) {
            if (is_instruction(first_token)) {
                char *second_token = strtok(NULL, "\n");  

                if (strcmp(first_token, ".string") == 0) {
                    if (second_token == NULL) {
                        print_error(line_num, "Missing parameter after .string");
                        error_found = 1; 
                    } else {
                        char *start_quote = strchr(second_token, '"');
                        char *end_quote = strrchr(second_token, '"');
                    if((start_quote==end_quote)&&(start_quote!=NULL)){
                       
                        
                        
                        if ((end_quote != NULL )&&((end_quote[1]=='\0')||((end_quote[1]=='\n')||(end_quote[1]=='\t')||(end_quote[1]==' ')))) {
                            print_error(line_num, "Missing closing quote for string");
                            error_found = 1; 
                        } 
                        else if (start_quote != NULL) {
                            print_error(line_num, "Missing opening quote for string");
                            error_found = 1;
                        }  
                        
                        else {
                            char string_content[LINE_SIZE];
                            strncpy(string_content, start_quote + 1, end_quote - start_quote - 1);
                            string_content[end_quote - start_quote - 1] = '\0';  

                            if (!is_valid_string(string_content)) {
                                print_error(line_num, "Invalid characters in string");
                                error_found = 1; 
                            }
                        }
                    }
                    }
                } else if (second_token == NULL) {
                    print_error(line_num, "Missing parameter after instruction");
                    error_found = 1; 
                }
            } else if (is_opcode(first_token)) {
                char *second_token = strtok(NULL, " ,\t\n"); 
                char *third_token = strtok(NULL, " ,\t\n");  

                if (strcmp(first_token, "mov") == 0 || strcmp(first_token, "cmp") == 0 || 
                    strcmp(first_token, "add") == 0 || strcmp(first_token, "sub") == 0 || 
                    strcmp(first_token, "lea") == 0) {
                    if (second_token == NULL || third_token == NULL) {
                        print_error(line_num, "Missing parameters after opcode");
                        error_found = 1; 
                    } else if (strtok(NULL, " ,\t\n") != NULL) {
                        print_error(line_num, "Too many parameters after opcode");
                        error_found = 1; 
                    }
                } else if (strcmp(first_token, "stop") == 0 || strcmp(first_token, "rts") == 0) {
                    if (second_token != NULL) {
                        print_error(line_num, "That opcode must not be followed by any arguments");
                        error_found = 1;
                    }
                } else {
                    if (second_token == NULL) {
                        print_error(line_num, "Missing parameters after opcode");
                        error_found = 1; 
                    } else if (third_token != NULL) {
                        print_error(line_num, "Too many parameters after opcode");
                        error_found = 1; 
                }
            }
        }

        token = strtok(line, " \t\n");
        while (token != NULL) {
            if (strlen(token) > WORD_SIZE) {
                if (token[strlen(token) - 1] == ':') {
                    print_error(line_num, "Label too long");
                    error_found = 1; 
                } else {
                    print_error(line_num, "Word too long");
                    error_found = 1; 
                }
            }
            token = strtok(NULL, " \t\n");
        }
    
        
        
    }
       pointer_to_data=(strstr(line_copy,".data"));
       
       
       
       
       
       if(pointer_to_data!=NULL){
            pointer_to_data=pointer_to_data+5;
            while(pointer_to_data[0]==' '){
                    pointer_to_data++;
             }
             if(pointer_to_data[0]==','){
                  print_error( line_num,"illegal comma first char after .data is comma");
             }
            while(pointer_to_data[0]!='\0'){
                if(*pointer_to_data=='.'){
                    is_a_space=0;
                    is_a_dot=1;
                }
                
                if (*pointer_to_data == ' ' || *pointer_to_data == '\t') {
                        is_a_space = 1 ;
            } 
                if((isdigit(pointer_to_data[0]))>0){
                    if((is_a_int==1)&&(is_a_space==1)){
                        print_error(line_num, "illegal number after number without comma ");
                        error_found=1;
                        is_a_int =1 ;
                        is_a_comma=0;
                        is_a_space=0;

                    }
                    if((is_a_int==1)&&(is_a_dot==1)){
                        print_error(line_num, "illegal invalid data parameter");
                        error_found=1;
                        is_a_int =1 ;
                        is_a_comma=0;
                        is_a_space=0;
                        is_a_dot=0;
                    }
                    else{
                        is_a_int =1 ;
                        is_a_comma=0;
                        is_a_space=0;
                    }
                }
                if(pointer_to_data[0]==','){
                    if((is_a_comma==1)&&(is_a_int==0)){
                        print_error(line_num, "illegal comma after comma in a row  ");
                        error_found=1;
                         is_a_int =1 ;
                         is_a_comma=0;
                         is_a_space=0;
                        
                    }
                    else{
                        is_a_comma=1;
                        is_a_int=0;
                        is_a_space=0;
                    }
                }


        pointer_to_data++;

        
       }
    }
        }
    fclose(file);
    return error_found; 
    }
    



  