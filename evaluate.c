#include <stdio.h>
#include <string.h>
#include "variables.h"
#include "tokens.h"
#include "expression.h"
#include "globals.h"
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


//what to do about not. write to stack with low or high precedence?
void do_operator(uint8_t t)
{
  double accum = 0;
  double accum2 = 0;
  //will need another operator table for strings.

  // assume integer if not float (fix later for double and string...)
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
    v_stack[v_top-1].value.sing = accum2 + accum;
    break;
  case MINUS:
    v_stack[v_top-1].value.sing = accum2 - accum;
    break;
  case MULTIPLY:
    v_stack[v_top-1].value.sing = accum2 * accum;
    break;
  case DIVIDE:
    v_stack[v_top-1].value.sing = accum2 / accum;
    break;
  case POWER:
    v_stack[v_top-1].value.sing = 7007;
    break;
  case AND:
    v_stack[v_top-1].value.sing = (accum && accum2) ? 1: 0;
    break;
  case OR:
    v_stack[v_top-1].value.sing = (accum || accum2) ? 1:0;
    break;
  case GT:
    v_stack[v_top-1].value.sing = (accum < accum2) ? 1:0;
    break;
  case LT:
    v_stack[v_top-1].value.sing = (accum > accum2) ? 1:0;
  case GTE:
    v_stack[v_top-1].value.sing = (accum <= accum2) ? 1:0;
    break;
  case NOT_EQ:
    v_stack[v_top-1].value.sing = (accum != accum2) ? 1:0;
    break;
  case EQ:
    v_stack[v_top-1].value.sing = (accum==accum2) ? 1:0;
  default:
    break;
  }
  v_top--;
  
}


//we use working and operator stack....
//could do pointer arithmatic. Would dramatically reduce caluclations.
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
      
      if (GLOBAL_STATE==IN_DIM){
	//top of the stack is number of dimensions. just under will be total elements
	//dim anywhere in an expression other than start is an error.
	//calculate total size,
	
	for (int ic = 0; ic < comma_count; ic++)
	  do_operator(MULTIPLY);
	v_top++;
	v_stack[v_top].value.intg = comma_count+1;
	v_stack[v_top].name[0] = '\0';	
	return v_stack[v_top];
      }

      //special code for array assignments. Just sets zero.
      else if (GLOBAL_STATE==ASSIGNMENT){
	//will be an error, undimmed array.
	int num = get_array_dims(working_stack[i].value);
	//	if (num !=comma_count+1)
	//printf("Error: Undimmed or assignment to invalid subscript\n");
	for (int ic = 0; ic < num-1; ic++)
	  do_operator(MULTIPLY);
	if (v_stack[v_top].type !=I)
	  v_stack[v_top].value.intg = (int) v_stack[v_top].value.sing;
	strcpy(v_stack[v_top].name, working_stack[i].value);
	return(v_stack[v_top]);
      }      
      int num = get_array_dims(working_stack[i].value);
      //verfiy comma count
      
      //might want to verify integer type...
      //if (comma_count != num-1)
	//printf("INVALID SUBSCRIPT ERROR\n");
      for (int ic = 0; ic < num-1; ic++)
	do_operator(MULTIPLY);
      //      if (v_stack[v_top].type !=I)
      //	v_stack[v_top].value.intg = (int) v_stack[v_top].value.sing;
      //strcpy(v_stack[v_top].name, working_stack[working_top].value);
      if (working_stack[i].type == STRING) {
	//TODO put string array in working buffer at offset
	
      }
      else{
	printf("Array value %f calculated\n", v_stack[v_top].value.sing);
	//put_array_value_in_var(&v_stack[v_top], working_stack[working_top].value) ;
      }
      //comma_count=0;
      
	  
      //assume single dimension for now.
      //v_top--;
      //v_stack[v_top].type = F;
      //v_stack[v_top].value.intg =22.22;
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
    
    else if (working_stack[i].type == INTEGER || working_stack[i].type == FLOAT || working_stack[i].type == STRING || working_stack[i].type == DOUBLE){
      read_anonymous_variable(&v_stack[++v_top], &working_stack[i]);
    }
   //convert and put on the stack.      
  }
  return v_stack[v_top];
}
  
