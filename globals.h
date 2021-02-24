#ifndef GLOBALS_H
#define GLOBALS_H

#include "variables.h"
#include "tokens.h"
#include "program.h"

//emacchar reserved_words[][] = {"AND", "OR", "NOT", "IF", "THEN", "GOTO", "GOSUB", "END", "FOR", "NEXT", "WHILE", "WEND"}

#define STRING_BUFF_SIZE 1024
extern unsigned int string_buffer_position;
extern line_index current_line;
extern char string_buffer[STRING_BUFF_SIZE];
extern variable working1;
extern variable working2;
extern unsigned int gosub_depth;
extern variable v_stack[10];
extern int v_top;

//If inside special state
enum interpreter_state {IN_IF, IN_DIM, IN_FOR, TO_ELSE, TO_END, REGULAR};
enum eq_switch {ASSIGNMENT, NON_ASSIGNMENT};
extern enum eq_switch EQ_SWITCH;
extern enum interpreter_state GLOBAL_STATE;
variable evaluate();
unsigned int execute_line(char *);
void dump_token(token );

void convert_stack_to_int_below_n(unsigned int n);
unsigned int calculate_array_size();


#endif
