#ifndef TOKENS_H 
#define TOKENS_H 1

#include <stdint.h>
#define TOKEN_ERROR_1 "INVALID TOKEN"
#define MAX_TOKEN_LENGTH 20
#define MAX_STRING 128
#define MAX_TOKENS 28

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
#define AND 11
#define OR 12
#define NOT 13
#define GT 14
#define LT 15
#define GTE 16
#define LTE 17
#define NOT_EQ 18
#define EQ 19    //flow starts at 20...
#define IF 20
#define THEN 21
#define FOR 22
#define NEXT 23
#define TO 24
#define ELSE 25
#define GOTO 26
#define GOSUB 27
#define PRINT 28

#define FLOW_START 20

#define STR_MATCH_NUM 12

static char *STR_TO_TOKEN[STR_MATCH_NUM] = { "AND", "OR", "NOT", "IF", "THEN", "ELSE", "FOR", "NEXT", "TO", "GOTO", "GOSUB", "PRINT"};

static int STR_MATCH[] = {AND,OR, NOT, IF, THEN, ELSE, FOR, NEXT, TO, ELSE, GOTO, GOSUB};

#define NUM_FUNCTIONS 11
static char *FUNCTION_MATCH[] = {"PEEK", "POKE", "STR$", "COS", "SIN", "TAN", "LEN", "RND", "LEFT$", "RIGHT$", "CHR$"};

static char *TOKEN_CHAR[] = {'\0', "+", "-", "/", "*", "^", "(", ")", ",", ";", ":","AND", "OR", "NOT", ">", "<" , ">=", "<=", "<>", "=", "IF", "THEN", "FOR", "NEXT", "TO", "ELSE"};

static int TOKEN_PRECEDENCE[MAX_TOKENS] = {0,1,1,2,2,3,4,4,0,0,0,5,5,5,0,0,0,0,0, 0, 5, 5, 5, 5, 5};

enum t_type {SYMBOL, INTEGER, FLOAT, ERROR, OPERATOR, STRING, EOL, INVALID, DOUBLE, ARRAY, FLOW, FUNCTION};





typedef struct TOKEN {
  char value[MAX_TOKEN_LENGTH];
  enum t_type type;
} token;

token buffer;
char read_buffer[MAX_STRING];
//pass by value 

uint8_t is_operator(char);
void read(char **, token *);			  

#endif
