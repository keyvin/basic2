#include <stdio.h>
#include <stdint.h>
#define MAX_GOSUB_DEPTH 20
/**********
    Program is a doubly linked list. I have a lot of appreciation for including line numbers in the source.
    Not only did it allow simple program flow, it made BASIC easy to edit in line orientied editors like
    Unix ed.

    Inserted lines are linked into the list in the appropriate position. Existing lines are overwritten.

*/


typedef uint16_t line_index;

enum return_type {r_next, r_goto,r_gosub,r_return, r_ok};

typedef struct PROGRAM_LINE {
    line_index line_number;
    char *line_text;
    struct PROGRAM_LINE *next;
    struct PROGRAM_LINE *previous;
} program_line;

struct SUB_STACK_ENTRY {
    line_index line;
    char *position; //position in line to resume from a gosub.
};

//These two are used to avoid return values/copying.
extern enum return_type return_type;
extern line_index next_line;


typedef struct SUB_STACK_ENTRY sub_stack_entry;
extern sub_stack_entry return_stack[MAX_GOSUB_DEPTH];
extern unsigned int return_stack_top;
extern program_line *program_start;


void add_line(line_index, char *);
//seperates line number and text.
void process_line(char *);
void flush_program();

