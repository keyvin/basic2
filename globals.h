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

#endif
