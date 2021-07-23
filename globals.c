#include "globals.h"

unsigned int string_buffer_position;
char string_buffer[STRING_BUFF_SIZE];
variable working1;
variable working2;
unsigned int gosub_depth;
variable v_stack[10];
enum eq_switch EQ_SWITCH;
enum interpreter_state GLOBAL_STATE;
line_index current_line;
enum return_type line_return_type;
line_index next_line;
char *return_position;
for_value new_for;

