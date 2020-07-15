#include "expression.h"
#include "tokens.h"

#include <stdio.h>
//#include "variables.h"
#include <stdint.h>


uint8_t expression(char **line)
{
  working_top = -1;
  operator_top = -1;
  char *position = *line;
  token next;
  token previous;
  //avoids extra logic
  previous.type = SYMBOL;
  read(&position, &next);
  if (next.type == OPERATOR)
    goto E_ERROR;

  while (next.type != INVALID && next.type != EOL && working_top < MAX_T_STACK  && *position!='\0') {

       
    if (next.type == EOL)
      break;
    if (next.type == OPERATOR && next.value[0] == OPAREN){
      operator_top++;
      operator_stack[operator_top] = next;      
    }
    else if(next.type == OPERATOR && next.value[0] == CPAREN) {
      while (operator_stack[operator_top].value[0] != OPAREN && operator_top >= 0){
	working_top++;
	working_stack[working_top] = operator_stack[operator_top];
	operator_top--;	
      }
      operator_top--;
      if (operator_top<-1)
	goto E_ERROR;
	     
    }
    
/*    else if (next.type == OPERATOR && previous.type == OPERATOR && previous.value[0] ==OPAREN)
      goto E_ERROR;
    */
    else if (next.type == OPERATOR){
	
      if ( operator_top >= 0 && TOKEN_PRECEDENCE[operator_stack[operator_top].value[0]] > \
	   TOKEN_PRECEDENCE[next.value[0]])
	if (pop_operators(TOKEN_PRECEDENCE[next.value[0]])){
	  goto E_ERROR;
	}
      operator_top++;
      operator_stack[operator_top] = next;
    }
    
    else if (next.type == SYMBOL || next.type == INTEGER || next.type ==FLOAT){
      working_top++;
      working_stack[working_top] = next;
    }
   	  	   
    previous = next;
    read(&position, &next);
  }
  pop_operators(0);
  *line = position;
  return 0;
  
 E_ERROR:    
  return 1;
}

uint8_t pop_operators(uint8_t precedence)
{
  
  while  (operator_top >= 0 && operator_stack[operator_top].value[0] !=OPAREN && precedence < TOKEN_PRECEDENCE[ operator_stack[operator_top].value[0] ] ) {   
    working_top++;
    working_stack[working_top] = operator_stack[operator_top];
    operator_top--;
  }
  
  return 0;
  
}  
