#ifndef GLOBALS_H
#define GLOBALS_H
#include "variables.h"
#include "tokens.h"


char reserved_words[][] = {"AND", "OR", "NOT", "IF", "THEN", "GOTO", "GOSUB", "END", "FOR", "NEXT", "WHILE", "WEND"}

			   
char string_buffer[400];
variable working1;
variable working2;

unsigned int gosub_depth = 0;

#endif
