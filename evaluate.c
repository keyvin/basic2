#include "variables.h"
#include "tokens.h"
#include "expression.h"

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

int is_operand(token *t){
  if (t->type  == INTEGER || t->type == FLOAT || t->type == STRING || t->type == DOUBLE)
    return 1;
  return 0;
}

void to_var(variable *v,token *t)
{
  


}

variable v_stack[25];


//we use working and operator stack....
variable  evaluate(){
  int v_top=-1;
  calc_top = -1;
  
  for (int i = 0; i <= working_top; i++){
    if (working_stack[i].type == operator){
      
      //if we can perform an operation, do so.      
      if ( > 1 && is_operand(&(operator_stack[calc_top])) && is_operand(&operator_stack[calc_top-1])) {
	calc_top = calc_top - 2;
	create_anon_var(&v_stack[++v_top], &operator_stack[calc_top]);
	create_anon_var(&v_stack[++v_top], &operator_stack[calc_top-1]);
	v_stack[v_top--] = operation(token->value[0], v_stack[v_top], v_stack[v_top-1]);	  
      }
    }
      
    }
  }
  
