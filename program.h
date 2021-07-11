#ifndef PROGRAM_H
#define PROGRAM_H 1
#include <stdio.h>
#include <stdint.h>
#include "globals.h"


#define MAX_CONTROL_STACK 20
/**********
    Program is a doubly linked list. I have a lot of appreciation for including line numbers in the source.
    Not only did it allow simple program flow, it made BASIC easy to edit in line orientied editors like
    Unix ed.

    Inserted lines are linked into the list in the appropriate position. Existing lines are overwritten.

*/

typedef uint16_t line_index;


enum return_type {r_next, r_goto,r_gosub,r_return, r_ok, r_error, r_for} ;


typedef struct PROGRAM_LINE {
    line_index line_number;
    char *line_text;
    struct PROGRAM_LINE *next;
    struct PROGRAM_LINE *previous;
} program_line;

//Control flow stack - this should have FOR/WHILE/GOSUB Entries for record keeping.
//Lookbacks for FOR/NEXT, WHILE/WEND should stop at the last gosub, and everything up to the last
//gosub should be removed upon a return.
//FOR and WHILE entries should not be removed in case GOTO is used to break out of the loop

//should definitely check for ambiguous next... which can be avoided by specifying a variable... egads!

typedef enum CF_STACK {cf_none, cf_for, cf_gosub, cf_while} cf_stack_type;


typedef union FOR_RANGE {int i, float f} for_range;
struct for_entry

//current should be assigned to the initial value. for a = b to c means current represents a, and has an initial value of b
//type (float/int) is held by the variable pointed to by the cf_stack_entry.
typedef struct FOR_VALUE {
    for_range stop;
    for_range step;
} for_value;

struct CF_STACK_ENTRY {
    cf_stack_type cf_type;
    line_index return_line;
    char *position; //OPTIMIZATION - SPACE SAVINGS TO USE INT OFFSET
    for_entry f_range;
};


//These two are used to avoid return values/copying.



typedef struct CF_STACK_ENTRY cf_stack_entry;
extern cf_stack_entry cflow_stack[MAX_CONTROL_STACK];
extern  int cf_stack_top;
extern program_line *program_start;


void add_line(char *);
//seperates line number and text.
void process_line(char *);
void flush_program();

void execute();

#endif
