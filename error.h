#ifndef FILE_ERROR_H
#define FILE_ERROR_H
 

#define LINE_SIZE 82
#define WORD_SIZE 15
#define MAX_WORDS 100 
#define macro_start "macr"
#define macro_end "endmacr"
#define RED "\033[1;31m"
#define RESET "\033[0m"

/* Function prototypes */
int is_opcode(char *word);
int is_instruction(char *word);
int macro_already_appeared(char *macro, char macros[][WORD_SIZE], int macro_count);
int label_already_appeared(char *label, char labels[][WORD_SIZE], int label_count);
int is_valid_label(char *label);
void print_error(int line_num, char *message);
int is_valid_string(char *str);
int checkFile1ForErrors(char *filename);
int checkFile2ForErrors(char *filename);

#endif