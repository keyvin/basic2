
#include "expression.h"
#include "tokens.h"
#include "globals.h"
#include <stdio.h>
//#include "variables.h"
#include <stdint.h>

token working_stack[MAX_T_STACK];
token operator_stack[MAX_T_STACK];
int operator_top;
int working_top;
token control;



// I may want to actually keep the ) and ( on the stack as they are delimiters.

//creates an evaluateable stack from an expression.
uint8_t expression(char **line)
{
  working_top = -1;
  operator_top = -1;
  char *position = *line;
  token next;
  token previous;
  //avoids extra logic
  previous.type = SYMBOL;
  //read first token. Verify not an operator.

  read(&position, &next);

  if (next.type == OPERATOR && next.value[0] != COLON)
    goto E_ERROR;
  if (next.type == FLOW)
    control = next;
  //while we read a valid token, that isn't an end of line.
  while (next.type != INVALID && next.type != EOL && working_top < MAX_T_STACK) {

    if (next.type == EOL || next.type == FLOW || (next.type == OPERATOR && next.value[0]==COLON)){
      control = next;
      // printf("%d\n", next.value[0]);
      break;
    }
    //assignment
    if(next.type == OPERATOR && next.value[0]==EQ && EQ_SWITCH==ASSIGNMENT){
      control = next;
      break;
    }

    //push array token to operator stack - this allows it to appear after any operands.
    if (next.type == ARRAY){
      operator_stack[++operator_top] = next;
    }
    //handle open parenthesis
    if (next.type == OPERATOR && next.value[0] == OPAREN){
      operator_top++;
      operator_stack[operator_top] = next;
    }
    else if(next.type == OPERATOR && next.value[0] == CPAREN) {
      while (operator_stack[operator_top].value[0] != OPAREN && operator_top >= 0){

	working_stack[++working_top] = operator_stack[operator_top--];

      }
      operator_top--;
      if (operator_top >=0 && operator_stack[operator_top].type==ARRAY){
	working_stack[++working_top] = operator_stack[operator_top--];
      }

      if (operator_top<-1)
	goto E_ERROR;

    }

/*    else if (next.type == OPERATOR && previous.type == OPERATOR && previous.value[0] ==OPAREN)
      goto E_ERROR;
    */
    //if we have a comma, we need to pop the operator stack, then push comma to the top of the stack
    //untill we get to an oparen. if no oparen, then it's an error

    else if (next.type == OPERATOR && next.value[0] == COMMA){
      while (operator_stack[operator_top].value[0] != OPAREN && operator_top > -1)
	working_stack[++working_top] = operator_stack[operator_top--];
      working_stack[++working_top] = next;
      if (operator_top < -1)
	goto E_ERROR;
    }

    else if (next.type == OPERATOR){

      if ( operator_top >= 0 && TOKEN_PRECEDENCE[operator_stack[operator_top].value[0]] > \
	   TOKEN_PRECEDENCE[next.value[0]])
	if (pop_operators(TOKEN_PRECEDENCE[next.value[0]])){
	  goto E_ERROR;
	}
      operator_top++;
      operator_stack[operator_top] = next;
    }

    else if (next.type == SYMBOL || next.type == INTEGER || next.type ==FLOAT || next.type == STRING){
      working_top++;
      working_stack[working_top] = next;
    }

    previous = next;
    if (*position=='\0')
      break;
    read(&position, &next);
  }
  while (operator_top >=0)
    working_stack[++working_top] = operator_stack[operator_top--];
  *line = position;
  return 0;

 E_ERROR:
  return 1;
}

uint8_t pop_operators(uint8_t precedence)
{

  // printf("incoming precedence %d, top precedence %d\n", precedence, TOKEN_PRECEDENCE[ operator_stack[operator_top].value[0]] );

  while  (operator_top >= 0 && operator_stack[operator_top].value[0] !=OPAREN && precedence < TOKEN_PRECEDENCE[ operator_stack[operator_top].value[0]] && operator_stack[operator_top].type == OPERATOR ) {
    //  printf("incoming precedence %d, top precedence %d\n", precedence, TOKEN_PRECEDENCE[ operator_stack[operator_top].value[0]] );
    //  printf("popping operator_top");

    working_stack[++working_top] = operator_stack[operator_top--];

  }

  //  printf("exit pop\n");
  return 0;
}

//works off of working stack.

