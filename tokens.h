#ifndef TOKENS_H 
#define TOKENS_H

#include <stdint.h>
#define TOKEN_ERROR_1 "INVALID TOKEN"
#define MAX_TOKEN_LENGTH 20
#define MAX_STRING 128


enum t_type {VARIABLE, INTEGER, FLOAT, ERROR, OPERATOR, STRING,EOL, INVALID};
typedef struct TOKEN {
  char value[MAX_TOKEN_LENGTH];
  enum t_type type;
} token;

token buffer;
char read_buffer[MAX_STRING];
//pas by value 

uint8_t is_operator(char);
void read(char **, token *);			  

#endif
