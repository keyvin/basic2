#ifndef VARIABLES_H
#define VARIABLES_H

//we can truncate the variable/symbol length safely.
//Did you know? C64 v2 basic only had two char var names?
//and var names could not contain keywords/tokens?
#include <stdint.h>
#include "tokens.h"
#define VARNAME_MAX 8


//First char of varname is type (i, s, 

enum var_types { F, I, S, D, STR, IA, SA, DA, STRA, INV};

typedef struct STRING {
  char *ptr;
  int length;
} string;

typedef union VALUE {
  int intg;
  float sing;
  double dubl;
  string str;
  void *array;
} value;



//null first byte means it's free.
typedef struct VARIABLE {
  char name [VARNAME_MAX];
  value value;
  enum var_types type;
} variable;



//256 vars to start... we can do something crazy later

#define MAX_VARS 256
//64k of string memory... for now...
char string_area[64000];
variable vars[256];
unsigned int num_vars;


//void free_variable(variable *);
void free_variable(int);
int  find_variable(int);

int read_int(char *);
double read_double(char *);
float read_float(char *);

//int find_variable_index(char *);
//variable * find_variable(char *);
int get_int__value(char *);
float get_float_value(char *);
double get_double_value(char *);
void put_string_in_buffer(char *);
void append_string_to_buffer(char *);
void free_by_name(char *);
void free_by_number(int);	 		 
uint8_t populate_variable(variable *, token *, token *);
  
void read_anonymous_variable(variable *, token *);

#endif
