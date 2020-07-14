#include "tokens.h"

//#include "variables.h"
#include <stdint.h>

#define MAX_T_STACK 20

enum expression_result {SUCCESS, CONTINUE, FINISHED};
  
//variable working_v[3];
token working_stack[MAX_T_STACK];
token operator_stack[MAX_T_STACK];
int operator_top = -1;
int working_top = -1;
uint8_t pop_operators();

//shunting yard algorithm :)
//only need one - 


uint8_t expression(char **line)
{
  char *position = *line;
  token next;
  token previous;
  //avoids extra logic
  previous.type = SYMBOL;
  read(&position, &next);
  if (next.type == OPERATOR)
    goto E_ERROR;
  
  while (next.type != INVALID && working_top < MAX_T_STACK) {
    read(&position, &next);
    if (next.type == EOL)
      break;
    if (next.type == OPERATOR && next.value[0] == OPAREN){

      operator_top++;
      operator_stack[operator_top] = next;
      continue;
    }
    if(next.type == OPERATOR && next.value[0] == CPAREN)
      if(!pop_operators())
	goto E_ERROR;
    if (next.type == OPERATOR && previous.type == OPERATOR)
      goto E_ERROR;
    
    if (next.type == OPERATOR)
      {
	
	operator_top++;
	operator_stack[operator_top] = next; 	  
	
	if ( operator_top > 0 && TOKEN_PRECEDENCE[operator_stack[operator_top].value[0]] <= \
	     TOKEN_PRECEDENCE[operator_stack[operator_top-1].value[0]])
	  if (!pop_operators()){
	    goto E_ERROR;
	  }
	
      }

    if (next.type == SYMBOL){
      working_top++;
      working_stack[working_top] = next;
    }
   	  	   
    previous = next;
  }
  *line = position;
  return 0;
  
 E_ERROR:    
  return 1;
}

uint8_t pop_operators()
{
  uint8_t par=0;
  //pop closing parens. decided to do this here. 
  if (operator_stack[operator_top].value[0] == CPAREN){
    par=1;
    operator_top--;
  }
  while  (operator_top >= 0 && operator_stack[operator_top].value[0] !=OPAREN){   
    working_top++;
    working_stack[working_top] = operator_stack[operator_top];
    operator_top--;    
  }
  if (operator_stack[operator_top].value[0] == OPAREN){
    operator_top--;
  }
  
  //mismatched PARENS
  if (operator_top < 0 && par == 1)
    return 1;
  return 0;
  
}  
