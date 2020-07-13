//This file generates a variable from a token

#include <string.h>
#include "tokens.h"
#include "variables.h"

#define ERROR "error"

uint8_t  populate_variable(variable *var, token *token1 , token *token2)
{
  //assume token1 is a variable name. If the last char is $, %, ! or alpha matters. First char of name is type.....
  uint8_t t1_l = strlen(token1->value);
  uint8_t t2_l = srtlen(token2->value);
  char t = '\0';
  if (token1->type == VARIABLE){
    t = token1->value[t1_l];
  }
  switch (t) {
  case '!':
    t
    

     token1->value[t1_1] = '\0';
  var->name[0]
      
  return 1;
}

float read_float(char *s)
{

  return 0f;
}

double read_double(char *s)
{
  return 0d;
}

int read_int(char *)
{
  return 0;
}
