#include "stdlib.h"
#include "program.h"
#include "ctype.h"
#include <string.h>



program_line *program_start = NULL;


//should be formatted line_number - text. Otherwise should be considered immediate.
//Likewise should be checked beforehand to ensure the length doesn't overflow.

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



void add_line(line_index line_no, char *line_text) {
    program_line *working;
    program_line *tmp;
    working = tmp = NULL;
    working = make_new_line(line_text);
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
    while (tmp->next !=NULL && line_no <= tmp->line_number)
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
    if (tmp->line_number > line_no) {
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
