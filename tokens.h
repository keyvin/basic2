#ifndef TOKENS_H 
#define TOKENS_H 1

#include <stdint.h>
#define TOKEN_ERROR_1 "INVALID TOKEN"
#define MAX_TOKEN_LENGTH 20
#define MAX_STRING 128
#define MAX_TOKENS 19

#define PLUS 1
#define MINUS 2
#define DIVIDE 3
#define MULTIPLY 4
#define POWER 5
#define OPAREN 6
#define CPAREN 7
#define COMMA 8
#define SEMI 9
#define COLON 10
#define AND 110
#define OR 12
#define NOT 13
#define GT 14
#define LT 15
#define GTE 16
#define LTE 17
#define NOT_EQ 18
#define EQ 19
static char TOKEN_CHAR[] = {'\0', '+', '-', '/', '*', '^', '(', ')', ',', ';', ':','A', 'O', 'N', 'G', 'L','E', 'e','!', '='};       
static int TOKEN_PRECEDENCE[MAX_TOKENS] = {0,0,0,1,1,1,2,2,0,0,0,0,0,0,0,0,0,0,0};

enum t_type {SYMBOL, INTEGER, FLOAT, ERROR, OPERATOR, STRING,EOL, INVALID };
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
