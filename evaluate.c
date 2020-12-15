#include <stdio.h>
#include <string.h>
#include "variables.h"
#include "tokens.h"
#include "expression.h"
#include "globals.h"
#include <stdlib.h>
// I think I can safely get rid of type and value...

//state machine: Line number
//first expression
     // expression - read next, no longer first expression
     
     //flow control (if, for, print)
          //if  (toggle in if(= does something different)
             // read_expression until then. eval expression. 
             // if expression is logical false, read tokens until else or end of line.
             //if expression is logical true, read expressions until EOL or ELSE
          //FOR  -- if encountered and first not set - error at line number
             // read token (symbol), // read '=', //read expression , TO, //read expression. Expressions must be numeric.
     //ON (read, eval expression) goto, gosub, goto, gosub 
          //PRINT (expression)    
    
//evaluate current working stack. 

//variable 
//token calc_stack[25];
int calc_top=1;
int v_top=-1;

int is_operand(token *t){
  if (t->type  == INTEGER || t->type == FLOAT || t->type == STRING || t->type == DOUBLE)
    return 1;
  return 0;
}
uint8_t is_operable(variable *v) {
  if (v->type==STR || v->type==D || v->type==I || v->type==F)
    return 1;
  return 0;
}

//catchy name
void convert_stack_to_int_below_n(unsigned int n){
  if (n > v_top)
    n = v_top;
  if (n<0)
    return;
  for(int a=0; a <= n; a++)
    convert_variable_to_int(&v_stack[a]);
  return;
}

unsigned int calculate_array_size() {
  if (v_top< 0)
    return 0;  
  unsigned int total = v_stack[0].value.intg;
  for (int i=1; i <= v_top; i++) {
    total = total *v_stack[i].value.intg;
  }
  //TODO - Error check range on dimension
  return total;
}

unsigned int calculate_array_offset(unsigned int number_of_dimensions, int i){
  if (v_top - number_of_dimensions <0)
    //TODO - Set ERROR
    return(0);  
  
  unsigned int accumulator = 0;
  for (int ic = 0; ic < (number_of_dimensions-1); ic++){
    convert_variable_to_int(&v_stack[v_top-ic]);
    //TODO - ERROR CONDITION, OUTSIDE BOUNDS
    accumulator += v_stack[v_top-ic].value.intg * get_dimension_n(working_stack[i].value, ic);	  
  }
  return(accumulator);  
}

//what to do about not. write to stack with low or high precedence?
//Not all operators decrement stack....
void do_operator(uint8_t t)
{
  double accum = 0;
  double accum2 = 0;
  //will need another operator table for strings.
  
  // assume integer if not float (fix later for double and string...)
  if (!(is_operable(&v_stack[v_top]) && is_operable(&v_stack[v_top-1]))){
      //TODO-ERROR CONDITION, CANNOT OPERATE DIRECTLY ON ARRAYS
      return;
    }
  //all operators concatenate. ',' did something special in gwbasic
  //many of these variables will have a NULL value - they were copied
  //into the string_buffer.
  if (v_stack[v_top].type == STR && v_stack[v_top].type==STR) {
    if(v_stack[v_top].value.str.ptr) { //not in string buffer yet
      strncpy((string_buffer+string_buffer_position), v_stack[v_top].value.str.ptr, v_stack[v_top].value.str.length);
      free(v_stack[v_top].value.str.ptr);
      v_stack[v_top].value.str.ptr = NULL;
    }
    v_top--;
    if(v_stack[v_top].value.str.ptr) {
      strncpy((string_buffer+string_buffer_position), v_stack[v_top].value.str.ptr, v_stack[v_top].value.str.length);
      free(v_stack[v_top].value.str.ptr);
      v_stack[v_top].value.str.ptr = NULL;
    }
    return;
  }

  else if(v_stack[v_top].type == STR) {
    //TODO ERROR, type mismatch
    free(v_stack[v_top].value.str.ptr);
    return;
  }

  if (v_stack[v_top].type == F)
    accum = v_stack[v_top].value.sing;
  else
    accum = v_stack[v_top].value.intg;
  if (v_stack[v_top-1].type == I){    
    accum2 = v_stack[v_top-1].value.intg;
  }
  else
    accum2 = v_stack[v_top-1].value.sing;
  v_stack[v_top-1].type = F;
  switch (t){
		
  case PLUS:
    v_stack[--v_top].value.sing = accum2 + accum;
    break;
  case MINUS:
    v_stack[--v_top].value.sing = accum2 - accum;
    break;
  case MULTIPLY:
    v_stack[--v_top].value.sing = accum2 * accum;
    break;
  case DIVIDE:
    v_stack[--v_top].value.sing = accum2 / accum;
    break;
  case POWER:
    v_stack[--v_top].value.sing = 7007;
    break;
  case AND:
    v_stack[--v_top].value.sing = (accum && accum2) ? 1: 0;
    break;
  case OR:
    v_stack[--v_top].value.sing = (accum || accum2) ? 1:0;
    break;
  case GT:
    v_stack[--v_top].value.sing = (accum < accum2) ? 1:0;
    break;
  case LT:
    v_stack[--v_top].value.sing = (accum > accum2) ? 1:0;
  case GTE:
    v_stack[--v_top].value.sing = (accum <= accum2) ? 1:0;
    break;
  case NOT_EQ:
    v_stack[--v_top].value.sing = (accum != accum2) ? 1:0;
    break;
  case EQ:
    v_stack[--v_top].value.sing = (accum==accum2) ? 1:0;
  default:
    break;
  }
  
  
}



//NOT token is a special case... what to do.

variable  evaluate(){
  v_top=-1;
  calc_top = -1;
  int comma_count = 0;
  for (int i = 0; i <= working_top; i++){

    //need to handle the case of an equals being the final operator. Comparison if IF is set, assignment otherwise.    
    //special case for IF - it is not equals, it is assignment. 
    // handle assignment specially
    //all commas must be in parens. When we get to a function or array lookup, we pop the arguments off in reverse order!  
    if(working_stack[i].type == OPERATOR && working_stack[i].value[0] == COMMA){
      comma_count++;
    }
      
    else if (working_stack[i].type == OPERATOR) {
      if (v_top >=1)
	do_operator(working_stack[i].value[0]);
    }      
      //calculates index with top N elements, and pushes result to top of stack
    else if(working_stack[i].type == ARRAY){
      //create or access?      
      if (GLOBAL_STATE==IN_DIM && i==working_top){
	//nothing else should follow this statement other than : or \n.
	//stack should contain N values that constitute the number of dimensions
	return v_stack[v_top];
      }
      int number_of_dimensions = get_array_dims(working_stack[i].value);
      printf("array %s, get_array_dims: %d\n", working_stack[i].value, number_of_dimensions);
      
      unsigned int accumulator = calculate_array_offset(number_of_dimensions, i);     
      //special code for array assignments. Just sets zero.
      if (GLOBAL_STATE==ASSIGNMENT){
        //will be an error, undimmed array.
	v_top -= number_of_dimensions-1;
	printf("dims calculated to %d\n", accumulator);
	v_stack[v_top].value.intg = accumulator;
	v_stack[v_top].type = i;
        return(v_stack[v_top]);
	
      }
      //else fetch
      v_top-=number_of_dimensions-1;
      get_value_from_array_into(working_stack[i].value ,accumulator, &v_stack[v_top]);
      printf("%s(%d)=%f retrieved\n", working_stack[i].value, accumulator, (v_stack[v_top].type == I)?(double)v_stack[v_top].value.intg:v_stack[v_top].value.sing);
      //continue
      
    }
    else if(working_stack[i].type == FUNCTION) {
      v_stack[v_top].type = F;
      v_stack[v_top].value.sing = 27.27;
      //get the number of arguments.
      //is the variable stack high enough
      //arguments are popped in reverse order
      
    }
    //fetch value
    else if (working_stack[i].type == SYMBOL) {
      //probably need a good bit more work here....
      v_stack[++v_top].type = F;
      //if this throws an error !!! 
      v_stack[v_top] = *find_variable(working_stack[i].value);
      //fetch value, place on variable stack
    }
    else if (working_stack[i].type == STRING){
      //Should be in the string buffer seperated by \0
      read_anonymous_variable(&v_stack[++v_top], &working_stack[i]);
    }
    
    else if (working_stack[i].type == INTEGER || working_stack[i].type == FLOAT || working_stack[i].type == DOUBLE){
      read_anonymous_variable(&v_stack[++v_top], &working_stack[i]);
    }
   //convert and put on the stack.      
  }
  return v_stack[v_top];
}
  
