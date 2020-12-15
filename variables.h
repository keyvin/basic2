
#ifndef VARIABLES_H
#define VARIABLES_H

//we can truncate the variable/symbol length safely.
//Did you know? C64 v2 basic only had two char var names?
//and var names could not contain keywords/tokens?
#include <stdint.h>
#include "tokens.h"
#define VARNAME_MAX 8


//First char of varname is type (i, s, 

enum VAR_types { F, I, D, STRV, IA, FA, DA, STRA, FV,INV};
typedef enum VAR_types var_types;

typedef struct ARRAY {
  void *ptr;
  unsigned int size;
  uint8_t number_of_dimensions;
  uint16_t *dimensions;
} array;

typedef struct STRING {
  char *ptr;
  int length;
} string;

//can use more restrictive type later. could contain pointer to line no
typedef struct FOR_VAR {
  int value;
  int stop;
  int step;
  uint32_t line_number;
} for_var;

//value is *ALWAYS* an integer for arrays. After dimming, they live somewhere else. This value is used
//for lookup and setting.

typedef union VALUE {
  array ary;
  int intg;
  float sing;
  double dubl;
  string str;
  for_var fvar;
} value;



//null first byte means it's free.
typedef struct VARIABLE {
  char name [VARNAME_MAX];
  value value;
  var_types type;
} variable;



//256 vars to start... we can do something crazy later

#define MAX_VARS 256
//64k of string memory... for now...
//char string_area[64000];
variable vars[256];
unsigned int num_vars;

void free_variable_by_reference(variable *);
//void free_variable(variable *);
void free_variable(int);
//int  find_variable(int);

int read_int(char *);
double read_double(char *);
float read_float(char *);

variable *find_variable(char *);
variable *set_variable(char *, variable *);

//variable * find_variable(char *);
int get_int__value(char *);
int get_array_dims(char *);
void put_array_value_in_var(variable *, char *);
float get_float_value(char *);
double get_double_value(char *);
void put_string_in_buffer(char *);
void append_string_to_buffer(char *);
void free_by_name(char *);
void free_by_number(int);	 		 
uint8_t populate_variable(variable *, token *, token *);
uint8_t dim_array(token *name, unsigned int size, uint8_t);
uint8_t get_value_from_array_into(char *name, unsigned int offset, variable *);
uint8_t set_value_in_array_from(char *name, unsigned int offset, variable *);
uint16_t get_dimension_n(char *name, uint8_t offset);
void read_anonymous_variable(variable *, token *);
int variable_exists(char *);
void auto_convert(variable *, variable *);
void convert_variable_to_int(variable *);
uint8_t set_dimension_n(char *, uint8_t, uint16_t);

#endif
