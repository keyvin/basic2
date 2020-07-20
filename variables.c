//This file generates a variable from a token

#include <string.h>
#include <stdlib.h>
#include "tokens.h"
#include "variables.h"
#include "globals.h"

#define ERROR "error"

//if token2 is a string, it should have already been read into the string buffer
//we can use the pointer to program memory as well... actually....
void read_anonymous_variable(variable *var, token *token1)
{
  switch (token1->type){
  case INTEGER:
    var->type = I;
    var->value.intg = read_int(token1->value);
    break;
  case FLOAT:
    var->type = F;
    var->value.sing = read_float(token1->value);
    break;
  case STRING:
    var->type = S;    
    var->value.str.ptr = (char *) malloc(sizeof(char)*strlen(string_buffer));
    var->value.str.length = strlen(string_buffer);
    break;
  default:
    var->type = INV;
    
  }
  var->name[0]='\0';

}
  
      
     

uint8_t  populate_variable(variable *var, token *token1 , token *token2)
{
  //assume token1 is a variable name. If the last char is $, %, ! or alpha matters. First char of name is type.....
  uint8_t t1_l = strlen(token1->value);
  uint8_t t2_l = strlen(token2->value);
  char t = '\0';
  if (token1->type == SYMBOL){
    t = token1->value[t1_l];
  }
  switch (t) {
  case '!':
    var->type = D;
    var->value.dubl = read_double(token2->value);
    break;
  case '%':
    var->type = I;
    var->value.intg = read_int(token2->value);
    break;
    //copy out of the global string buffer
  case '$':
    var->type = S;    
    var->value.str.ptr = (char *) malloc(sizeof(char)*strlen(string_buffer));
    var->value.str.length = strlen(string_buffer);
    break;
  default:
    var->value.sing = read_float(token2->value);
    var->type = F;
    break;
  }
  strcpy(var->name, token1->value);
  
      
  return 1;
}
/*replace with my own*/
float read_float(char *s)
{
  return atof(s);
  //return 0.0;
}

double read_double(char *s)
{
  return atof(s);
  //return 0.0;
}

int read_int(char *s)
{
  return atoi(s);
  // return 0;
}
