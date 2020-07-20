#include "variables.h"
#include "tokens.h"
#include "expression.h"

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
token calc_stack[25];
int calc_top=1;
int v_top=-1;
int is_operand(token *t){
  if (t->type  == INTEGER || t->type == FLOAT || t->type == STRING || t->type == DOUBLE)
    return 1;
  return 0;
}



variable v_stack[10];
int v_top;



void do_operator(token *t)
{
  double accum = 0;
  double accum2 = 0;
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
  switch (t->value[0]){
		
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
  default:
    break;
  }
  v_top--;
  
}


//we use working and operator stack....
//could do pointer arithmatic. Would dramatically reduce caluclations.
variable  evaluate(){
  v_top=-1;
  calc_top = -1;
  
  for (int i = 0; i <= working_top; i++){

    //need to handle the case of an equals being the final operator. Comparison if IF is set, assignment otherwise.
  
      //all commas must be in parens. When we get to a function or array lookup, we pop the arguments off in reverse order!
      if(working_stack[i].type == OPERATOR && working_stack[i].value[0] == COMMA){
	v_top++;
      }
      
      else if (working_stack[i].type == OPERATOR) {
	if (v_top >=1)
	  do_operator(&working_stack[i]);
      }
      
      //calculates index with top N elements, and pushes result to top of stack
      else if(working_stack[i].type == ARRAY){
	//assume single dimension for now.
	//v_top--;
	v_stack[v_top].type = F;
	v_stack[v_top].value.intg =22.22;
      }
      else if(working_stack[i].type == FUNCTION) {
	v_stack[v_top].type = F;
	v_stack[v_top].value.sing = 27.27;
	//get the number of arguments.
	//is the variable stack high enough
	//arguments are popped in reverse order
	
      }
      else if (working_stack[i].type == SYMBOL) {
	v_stack[v_top].type = F;	
	//fetch value, place on variable stack
      }
      
      else if (working_stack[i].type == INTEGER || working_stack[i].type == FLOAT || working_stack[i].type == STRING || working_stack[i].type == DOUBLE){
	read_anonymous_variable(&v_stack[++v_top], &working_stack[i]);
      }
      //convert and put on the stack.
      
  }
  return v_stack[v_top];
}
  
