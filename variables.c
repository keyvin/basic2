//This file generates a variable from a token

#include <string.h>
#include <stdlib.h>
#include "tokens.h"
#include "variables.h"
#include "globals.h"
#include "expression.h"

#define ERROR "error"

//if token2 is a string, it should have already been read into the string buffer
//we can use the pointer to program memory as well... actually....
//anon strings just use string buffer till assignment.


/*int get_array_dims(char *to_find)
{
  for (int i = 0; i< MAX_VARS; i++){
    if(strcmp(vars[i].name, to_find)==0){
      return vars[i].value.intg;
      break;
    }    
  }      
  return 0;
}*/

//uint8_t dim_array(variable *location, token *name, unsigned int size);
//uint8_t get_val_from_array(variable *result, char *name, unsigned int offset);
//uint8_t set_val_in_array(variable *value, char *name, unsigned int offset);
//FV is for value
//enum var_types { F, I, S, D, STRV, IA, SA, DA, STRA, FV,INV};

void check_copy_name(char *dest, char *name){
  int name_length = strlen(name);
  if (name > MAX_VAR_NAME){
    //error
  }
  if (name_length == 0) {
    //error
  }
  strncpy(dest, name, name_length);
  dest[name_length+1] = '\0';
}

uint8_t dim_array(variable *location, token *name, unsigned int size, var_types type){
  location->type = type;
  int bytes;
  switch (type){
  case FA:
    bytes = sizeof(float)*size;
  case IA:
    bytes = sizeof(int)*size;
    break;
  case SA:
    bytes = sizeof(string)*size;
    break;
  case DA:
    bytes = sizeof(double)*size;
    break;
  default:
    //else error
  };   
  location->ary->ptr = (void *) malloc(bytes);
  //if a string,
  //more work?
  
  check_copy_name(dest->name, token->value);
  return;
  
}

/*void put_array_value_in_var(variable *a, char *b)
{
  a->type = F;
  a->value.sing = 0.0;
  }*/

//call get variable first - should look like (&var_from, int offset, &var_to)
// set global error state to out of bounds and return 0.
uint8_t get_variable_from_array_into(variable *ar, int index,  variable *target){  
  switch (ar->type){
    case IA:
      break;
    case FA:
      break;
    case SA:
      break;
    case DA:
      break;
    default:
  }
  
}
  
variable * find_variable(char *to_find)
{
  for (int i=0; i < MAX_VARS;i++){
    if (strcmp(vars[i].name, to_find)==0)
      return &vars[i];
  }
  return &vars[0];  
}


variable * set_variable(char *to_set, variable *var)
{
  if (num_vars == MAX_VARS)
    return NULL;
  int offset = -1;
  for (int i=0;i < MAX_VARS;i++){
    if(strcmp(vars[i].name, to_set)==0){
      offset = i;
      break;
    }
  }
  
  //find next free.
  if (offset == -1)
    for (int i=0;i< MAX_VARS; i++){
      if (vars[i].name[0]=='\0'){
	offset = i;
	break;
      }
    }
  
  if (offset !=-1){
    if (vars[offset].type==S){	
      free(vars[offset].value.str.ptr);
      vars[offset].value.str.ptr = (char *) malloc(sizeof(char)*strlen(var->value.str.ptr)+1);
      strcpy(vars[offset].value.str.ptr, var->value.str.ptr);
      return &vars[offset];
    }
    else {
      vars[offset] = *var;
      strcpy(vars[offset].name, to_set);
      return &vars[offset];
    }
    num_vars++;
  }
  return NULL;
}


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
    var->value.str.ptr = string_buffer;
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
