//This file generates a variable from a token

#include <string.h>
#include <stdlib.h>
#include "tokens.h"
#include "variables.h"
#include "globals.h"
#include "expression.h"
#include <stdio.h>
#define ERROR "error"

//if token2 is a string, it should have already been read into the string buffer
//we can use the pointer to program memory as well... actually....
//anon strings just use string buffer till assignment.

void print_variable(variable v){
  char *type;
  double value;
  type = "STRING OR DOUBLE";
  switch (v.type){
    
  case I:
    type = "Integer";
    value = (double)v.value.intg;
    break;
  case F:
    type = "Float";
    value = (double)v.value.sing;
    break;
  case IA:
    type = "Integer Array";
    value = (double)v.value.ary.size;
    break;
  case FA:
    type = "Float Array";
    value = (double)v.value.ary.size;
    break;
  }  
  printf ("variable: %s, type %s, value or length: %f\n", v.name, type, (double) value);
}


void copy_string_var_to_ptr(char *to, variable *var){
  if(to != NULL)
    (free);
  if (var->type != STR)
    return; //TODO -- ERROR

      //Value is in the string buffer
  if (var->value.str.ptr==NULL){
    to = (char *) malloc(sizeof(char)*string_buffer_position);
    strncpy(to, string_buffer, string_buffer_position);
    to[string_buffer_position]='\0';
  }
      //we are making a copy of var (this code path shouldn't be used, but it's here)
  else {
    to = (char *) malloc(sizeof(char)*var->value.str.length);
    strncpy(to, var->value.str.ptr, var->value.str.length);    
  }
  
}

  

unsigned int find_next_free(){
  //find next free.
  int offset = -1;
  for (int i=0;i< MAX_VARS; i++){
    if (vars[i].name[0]=='\0'){
      offset = i;
      break;
    }
  }
  return offset;  
}

int get_array_dims(char *to_find)
{
  for (int i = 0; i< MAX_VARS; i++){
    if(strcmp(vars[i].name, to_find)==0){
      return vars[i].value.ary.number_of_dimensions;
      break;
    }    
  }      
  return 0;
}

//uint8_t dim_array(variable *location, token *name, unsigned int size);
//uint8_t get_variable_from_array(variable *result, char *name, unsigned int offset);

//uint8_t set_value_in_array(variable *value, char *name, unsigned int offset);
//FV is for value
//enum var_types { F, I, S, D, STRV, IA, SA, DA, STRA, FV,INV};

void free_variable_by_reference(variable *target){


}

void check_copy_name(char *dest, char *name){
  int name_length = strlen(name);
  if (strlen(name) > VARNAME_MAX){
    //error
  }
  if (name_length == 0) {
    //error
  }
  strncpy(dest, name, name_length);
  dest[name_length+1] = '\0';
}

//invoke populate array first to set type, check copy name
//need a check on number of dimensions.
//name has a type specifier ($, %, or none)
uint8_t dim_array(token *name, unsigned int size, uint8_t num_dims){
  if (variable_exists(name->value)!=-1){
    //error condition
    printf("error, already exists");    
  }

  token t_tmp;
  strcpy(t_tmp.value,"0.0");
  t_tmp.type = FLOAT;
  
  variable location;    
  populate_variable(&location, name, &t_tmp); 
  location.value.ary.number_of_dimensions = num_dims;
  location.value.ary.dimensions = malloc(sizeof(uint16_t)*num_dims);
  unsigned int bytes;
  switch (location.type){
  case F:
    location.type = FA;
    bytes = sizeof(float)*size;
    break;
  case I:
    location.type = IA;
    bytes = sizeof(int)*size;
    break;
  case STR:
    location.type = STRA;
    bytes = sizeof(char *)*size;
    break;
  case D:
    location.type = DA;
    bytes = sizeof(double)*size;
    break;
  default:
    break;
    //else error
  };   
    
  location.value.ary.ptr = (void *) malloc(bytes);
  location.value.ary.size = size;

  set_variable(location.name, &location);
  //if a string,
  //more work?
  print_variable(location);
  //  check_copy_name(location->name, name->value);
  return 1;
  
}

uint8_t set_dimension_n(char *name, uint8_t dimension, uint16_t dimension_size) {
  unsigned int offset = -1;
  offset = variable_exists(name);
  if (offset==-1)
    return 0;
  //TODO-Set an ERROR
  if (dimension_size==0)
    //TODO-Set an ERROR
    return 0;
  if (vars[offset].value.ary.number_of_dimensions < dimension){}
  vars[offset].value.ary.dimensions[dimension] = dimension_size;
  return dimension;
}

uint16_t get_dimension_n(char *name, uint8_t dimension){
  unsigned int offset = -1;
  offset = variable_exists(name);
  if (offset==-1)
    return 0;
  //TODO--SET ERROR
  if (dimension > vars[offset].value.ary.number_of_dimensions)
    return 0;
  //TODO --SET ERROR
  return vars[offset].value.ary.dimensions[dimension];    
}

/*void put_array_value_in_var(variable *a, char *b)
{
  a->type = F;
  a->value.sing = 0.0;
  }*/

//call get variable first - should look like (&var_from, int offset, &var_to)
// set global error state to out of bounds and return 0.

uint8_t get_value_from_array_into(char *name, unsigned int index,  variable *target){  
  //TODO - error condition
  variable *from = find_variable(name);
  if (!from)
    return 0;
  if (index >= from->value.ary.size || index < 0){
    // out of bounds
    return 0;
  }  
  //may not be necessary with garbage collection
  //  free_variable_by_reference(target);
  target->name[0] = '\0';
  array *ary = &from->value.ary;
  switch (from->type){
  case IA:
    target->type = I;
    target->value.intg =*(((int *)ary->ptr)+index);
    break;
  case FA:
    target->type = F;
    target->value.sing = *(((float *)ary->ptr)+index);
    break;
  case STRA:
    //Read into string buffer
    target->type = STR;
    target->value.str.ptr = '\0';
    target->value.str.length = strlen(((char *)ary->ptr)+index);
    strcpy(((char *)ary->ptr)+index, string_buffer+string_buffer_position);
    string_buffer_position += target->value.str.length;
    break;
  case DA:
    target->type = D;
    target->value.dubl = *(((double *)ary->ptr)+index);
    break;
  default:
      break;
      //error condition
  }
  print_variable(*target);
  return 0;
}

//call find variable first
//verfiy types or cast?
uint8_t set_value_in_array_from(char *name, unsigned int index, variable *from) {
  //may not be necessary with garbage collection
  //definitely necessary with strings.
  variable *target = find_variable(name);
  if(!target){
    printf("not found\n");
    //error
    return 0;
  }
  if (index >= target->value.ary.size || index < 0){
    printf("Out of bounds\n");
   // out of bounds
    return 0;
  }
  
  auto_convert(target, from);  
  //strings
  array *ary = &target->value.ary;
  switch (target->type){
  case IA:
      *(((int *)ary->ptr)+index) = from->value.intg;
      break;
  case FA:
    *(((float *)ary->ptr)+index) = from->value.sing;
    break;
  case STRA:
    //need to make a copy
    copy_string_var_to_ptr(((char *) ary->ptr)+index, from);
    //target->value.str->ptr = '\0';
    //target->value.str->len = 0;
    break;
  case D:
    // target->type = D;
    //      target->value.dubl = *(((double *)ary->ptr)+index);
    break;
  default:
    break;
    //error condition
  }
  return 0;
}


//can b
int variable_exists(char *to_find) {
  int offset = -1;
  for (int i=0;i < MAX_VARS;i++){
    if(strcmp(vars[i].name, to_find)==0){
      offset = i;
      break;
    }
  }
  return offset;
}

variable *find_variable(char *to_find)
{
  for (int i=0; i < MAX_VARS;i++){
    if (strcmp(vars[i].name, to_find)==0)
      return &vars[i];
  }
  return NULL;  
}

variable *set_variable(char *to_set, variable *var)
{
  if (num_vars == MAX_VARS)
    return NULL;
  int offset = variable_exists(to_set);  
  //find next free.
  if (offset == -1)
    for (int i=0;i< MAX_VARS; i++){
      if (vars[i].name[0]=='\0'){
	offset = i;
	break;
      }
    }
  
  if (offset !=-1){
    if (vars[offset].type==STR){	
      if (vars[offset].value.str.ptr)
	free(vars[offset].value.str.ptr);
      return &vars[offset];
    }
    else if (vars[offset].type==IA || vars[offset].type == FA || vars[offset].type==STRA || vars[offset].type==DA) {
      if (vars[offset].value.ary.ptr){
	if (vars[offset].value.ary.ptr)
	  free (vars[offset].value.ary.ptr);
	if (vars[offset].value.ary.dimensions)
	  free(vars[offset].value.ary.dimensions);
      }
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



//Anonymous strings 

void fill_string_from_token(variable *var, token *token1){
  var->type = STR;    
  unsigned int length = ((t_string_info *)token1->value)->length+1;
  char *str_ptr = ((t_string_info *)token1->value)->start;      
  var->value.str.ptr = (char *) malloc(sizeof(char)*length);
  var->value.str.length = length;
  strncpy(var->value.str.ptr,str_ptr, length);
  var->value.str.ptr[length]='\0';
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
    var->type = STR;        
    var->value.str.ptr = '\0';
    var->value.str.length = ((t_string_info *) token1->value)->length;
    strncpy(string_buffer+string_buffer_position, ((t_string_info *) token1->value)->start, var->value.str.length);
    string_buffer_position += var->value.str.length;
    break;
  default:
    var->type = INV;    
  }
  var->name[0]='\0';
}


           
uint8_t populate_variable(variable *var, token *token1 , token *token2)
{
  //assume token1 is a variable name. If the last char is $, %, ! or alpha matters. First char of name is type.....
  uint8_t t1_l = strlen(token1->value)-1;
  uint8_t t2_l = strlen(token2->value)-1;
  char t = '\0';
  if (token1->type == SYMBOL || token1->type == ARRAY){
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
    if (token2->type == STRING)
      fill_string_from_token(var,token2);
    else
      var->type=STR;
    break;
  default:
    var->value.sing = read_float(token2->value);
    var->type = F;
    break;
  }
  strcpy(var->name, token1->value);      
  return 1;
}

void convert_variable_to_int(variable *a){
  if (!a)
    return;
  switch (a->type){
  case F:
    a->value.intg = a->value.sing;
    break;
  case D:
    a->value.intg = a->value.dubl;
    break;
  default:
    //Should set a global error state
    break;
  }
  a->type = I;
}

void auto_convert(variable *a, variable *b){
  if (a->type == b->type)
    return;
  if ((a->type == F || a->type==FA) && b->type == I){
    b->value.sing = (float) b->value.intg;
    b->type = F;
  }
  else if ((a->type == I || a->type==IA) && b->type == F) {
    b->value.intg = (int) b->value.sing;
    b->type = I;
  }
  else {
    //TODO - error condition, can only autoconvert between int and float
    return;  
  }
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
