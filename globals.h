#ifndef GLOBALS_H
#define GLOBALS_H
#include "variables.h"
#include "tokens.h"


//emacchar reserved_words[][] = {"AND", "OR", "NOT", "IF", "THEN", "GOTO", "GOSUB", "END", "FOR", "NEXT", "WHILE", "WEND"}

#define STRING_BUFF_SIZE 1024			   
char string_buffer[STRING_BUFF_SIZE];
variable working1;
variable working2;
unsigned int gosub_depth;

//If inside special state
enum interpreter_state {IN_IF, IN_DIM, IN_FOR, TO_ELSE, TO_END, REGULAR};
enum eq_switch {ASSIGNMENT, NON_ASSIGNMENT};
enum eq_switch EQ_SWITCH;
enum interpreter_state GLOBAL_STATE;
variable evaluate();
unsigned int execute_line(char *);
void dump_token(token );

void convert_stack_to_int_below_n(unsigned int n);
unsigned int calculate_array_size();

variable v_stack[10];
int v_top;

#endif
