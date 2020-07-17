#ifndef VARIABLES_H
#define VARIABLES_H

//we can truncate the variable/symbol length safely.
//Did you know? C64 v2 basic only had two char var names?
//and var names could not contain keywords/tokens?
#include <stdint.h>
#include <tokens.h>
#define VARNAME_MAX 10


//First char of varname is type (i, s, 
typedef struct VARIABLE {
  char name [VARNAME_MAX];
  value value;
  var_types type;
} variable;


void generate_variable(token *, variable *);
  


#endif
