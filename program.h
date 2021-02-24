#include <stdio.h>
#define MAX_GOSUB_DEPTH 20

typedef uint16_t line_index;

enum return_type {r_next, r_goto,r_gosub,r_return, r_ok};

struct PROGRAM_LINE {
    line_index line_number;
    char *line;
    struct PROGRAM_LINE *next
}

struct SUB_STACK_ENTRY {
    line_index line;
    char *position; //position in line to resume from a gosub.
}

extern enum return_type return_type;
extern line_index next_line;

typedef PROGRAM_LINE program_line;
typedef SUB_STACK_ENTRY sub_stack_entry;
extern sub_stack_entry return_stack[MAX_GOSUB_DEPTH];
extern unsigned int return_stack_top;

void add_line(char *text);
void flush_program();

