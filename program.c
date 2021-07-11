#include "stdlib.h"
#include "program.h"
#include "ctype.h"
#include <string.h>

//A line should execut


program_line *program_start = NULL;
int cf_stack_top = -1;
cf_stack_entry cf_stack[MAX_CONTROL_STACK];
//should be formatted line_number - text. Otherwise should be considered immediate.
//Likewise should be checked beforehand to ensure the length doesn't overflow.
void execute() {
    program_line *current;
    char *resume = NULL;
    char *ret = NULL;
    if (program_start == NULL)
        return;
    current = program_start;
    if(!program_start->line_text)
    {
        //set global error condition
    }
    line_return_type = r_ok;
    //This is where to check for global error state and alert the user!
    while (current != NULL && line_return_type != r_error ) {
  //      printf("Line: %d\n", current->line_number);
        if (current->line_number==28){
         printf("f");
         }
        if (resume){

            ret = execute_line(resume);
            resume = NULL;
        }
        else {
            ret = execute_line(current->line_text);
        }
        if (line_return_type == r_ok) {
            current = current->next;
        }
        if (line_return_type == r_goto) {
            //TODO check if ERROR was set - invalid line number.
           // printf("GOTO\n");
            program_line *tmp = program_start;
            while (tmp->next !=NULL && next_line != tmp->line_number)
                tmp = tmp->next;
            if (tmp->line_number == next_line){
                current = tmp;
            }
            else {
                line_return_type == r_error;
            }
        }
        if (line_return_type == r_gosub) {
            //push current_line and ret to gosub stack
            if (cf_stack_top >= MAX_CONTROL_STACK) {
            //TODO - ERROR
            printf("Stack out of room\n");
            return;
            }
            cf_stack_top++;
            cf_stack[cf_stack_top].cf_type = cf_gosub;
            cf_stack[cf_stack_top].position = ret;
            cf_stack[cf_stack_top].return_line = current->line_number;
            program_line *to_find;
            for (to_find = program_start;
                to_find->next != NULL && to_find->line_number!=next_line;
                to_find = to_find->next );
                current=to_find;

        }
        if (line_return_type == r_return){
            //Pop everything back to the last gosub. If no gosub, error
            int stack_top = cf_stack_top;

            while (cf_stack_top >= 0 && cf_stack[stack_top].cf_type != cf_gosub) {
                //TODO - free for variables.
                cf_stack[stack_top].cf_type = cf_none;
                stack_top--;
            }
            if (cf_stack_top < 0) {
                //TODO - ERROR
                printf("return without gosub\n");
                return;
            }
            program_line *to_find = NULL;
            for (to_find = program_start;
                to_find->next != NULL && to_find->line_number!=cf_stack[stack_top].return_line;
                to_find = to_find->next );
            //shouldn't happen
            //TODO - ERROR, PROGRAM LINE IS NULL

            cf_stack_top = stack_top;
            cf_stack_top--;
            //printf("RETURN\n");
            current = to_find;
            resume=cf_stack[stack_top].position;
        }
        if (line_return_type == r_next){

        }
        if (line_return_type == r_for){

        }

    }
}

//splits a line into a line number and a string
unsigned int get_line_number(char **to_add){
    char tmp_buff[10];
    unsigned int pos = 0;
    char *start = *to_add;
    while (*start >= '0' && *start <='9' && *start !='\0' && pos <10 ) {
        tmp_buff[pos] = (*to_add)[pos];
        start++;
        pos++;
    }
    tmp_buff[pos] = '\0';

    *to_add = start;
    return atoi(tmp_buff);
}


program_line *make_new_line(char *line_text) {
    program_line *to_make = (program_line *) malloc(sizeof(program_line));
    if (to_make == NULL){
        //set global error condition
        return NULL;
    }
    to_make->next = NULL;
    to_make->previous = NULL;
    to_make->line_text = (char *) malloc(sizeof(char)*strlen(line_text)+1);
    if(to_make->line_text == NULL) {
            //set global error condition
            free(to_make);
            to_make = NULL;
            return NULL;
    }
    strcpy(to_make->line_text, line_text);
    return to_make;
}



void add_line(char *line_text) {

    char *txt_start = line_text;
    line_index line_no = get_line_number(&txt_start);

    program_line *working;
    program_line *tmp;
    working = tmp = NULL;
    working = make_new_line(txt_start);
    if (working == NULL) //error
        return;
    working->line_number = line_no;
    //Program is empty
    if (program_start==NULL) {
        program_start = working;
        return;
    }
    if (program_start->line_number == line_no){
        tmp = program_start;
        working->next = program_start->next;
        working->previous = NULL;
        //update double link!
        working->next->previous = working;
        program_start = working;
        free(tmp->line_text);
        free(tmp);
        return;

    }
    if (program_start->line_number > line_no) {
        working->next = program_start;
        program_start->previous = working;
        working->previous = NULL;
        program_start = working;
        return;
    }
    tmp = program_start;
    //scan list, find element we will insert at in tmp
    while (tmp->next !=NULL && line_no >= tmp->line_number)
        tmp = tmp->next;
    if (tmp->line_number == line_no){
        //replace here
        working->previous = tmp->previous;
        working->next = tmp->next;
        tmp->previous->next = working;
        if(tmp->next) {
            tmp->next->previous = working;
        }
        free (tmp->line_text);
        free(tmp);
        return;
    }
    //insert at end of list
    if (tmp->next == NULL) {
        tmp->next = working;
        working->previous = tmp;
        working->next = NULL;
        return;
    }
    //insert just before this element
    if(tmp->line_number > line_no) {
        working->previous = tmp->previous;
        tmp->previous->next = working;
        working->next = tmp;
        tmp->previous = working;
        return;
    }
    return;
}


//implement in a bit
void flush_program() {
    return;
}
