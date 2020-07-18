//This file generates a variable from a token

#include <string.h>
#include <stdlib.h>
#include "tokens.h"
#include "variables.h"
#include "expression.h"
#define ERROR "error"

//Absoluteley need to figure out how to alloc string and array areas without using malloc.

//if token2 is a string, it should have already been read into the string buffer
//assume strings are null terminated (for now... can change later)

void free_by_number(int num) {
  vars[num].name = '\0';
  if (vars[nim].type == S){
    free (vars[num].s.ptr);
  }
  num_vars--;
}

int find_next_free(){
  for (int a =0; a < MAX_VARS; a++){
    if (vars[a].name == '\0')
      return a;
  }
  return -1;
}

variable * find_by_name(char *name){
  for (int a=0; a <MAX_VARS;a++){
    if (strcmp(vars[a].name, name)==0){
      return (&(vars[a]));
    }
    
  }
  return NULL;
}

void append_string_to_buffer(char *name){
  variable *vn = NULL;
  vn = find_by_name(name);
  if (vn) strcat(string_buffer, vn); 
} 

void put_string_in_buffer(char *name){
  variable *vn = NULL;
  vn = find_by_name(name);
  if (vn) strcpy(string_buffer, vn); 
}
 
  
  
//TODO fix using full int. string buffer is the location of all strings currently


void create_anon_var(*variable to_v, *token token) {
  switch (token->type){
  case INTEGER:
    to_v->type = I;
    to_v->value = read_int(token->value);
    break;
  case FLOAT:
    to_v->type = F;
    to_v->value = read_float(token->value);
    break;
  case STRING:
    to_v->type = S;
    to_v->s.ptr = string_buffer;
    break;
  default:
    to_v->type = INVALID
  }
 
int  populate_variable(variable *var, token *token1 , token *token2)
{
  //assume token1 is a variable name. If the last char is $, %, ! or alpha matters. First char of name is type.....
  uint8_t t1_l = strlen(token1->value);
  uint8_t t2_l = srtlen(token2->value);
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
    var->value.str = (char *) malloc(sizeof(char)*strlen(string_buffer));
    break;
  default:
    var->value.sing = read_float(token2->value);
    var->type = f;
    break;
  }
  strcpy(var->name, token1->value);
  
      
  return 1;
}
/*replace with my own*/
float read_float(char *s)
{
  return atof(s);
  return 0.0;
}

double read_double(char *s)
{
  return atod(s);
  return 0.0;
}

int read_int(char *)
{
  return atoi(s);  
}
