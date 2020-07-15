#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "tokens.h"
#include "expression.h"
//#include "expressions.h"
//read token


//the : operator combines statements into a single line-num
//the ; tells print to omit a new line
uint8_t expression(char **);

uint8_t is_operator(char val)
{

  switch (val) {
  case '+':
    return PLUS;
  case '-':
    return MINUS;
  case '/':
    return DIVIDE;    
  case '*':
    return MULTIPLY;
  case '^':
    return POWER;
  case '(':
    return OPAREN;
  case ')':
    return CPAREN;
  case '<':
    return LT;
  case '>':
    return GT;
  case '=':
    return EQ;
  case ':':
    return COLON;
  case ';':
    return SEMI;
  default:
    return 0;
  }
  return 0;
    
}

    
  
/*
  if (val == '+' || val == '-' || val =='/' || val == '*' || val == '^' || val =='(' || val == ')' || val == '<' || val == '>'|| val == '='|| val ==':' ||val ==';')
    return 1;
  return 0;
}
*/

//todo Check if int or float contains an E.

//modifies pointer passed in so there is no need to keep track
void read(char **string, token *token) {
  char *ptr = *string;
  uint8_t count = 0; 
  uint8_t isint = 0;
  uint8_t deciread = 0;
  uint8_t eread = 0;
  //advance whitespace
  if (*ptr == '\0'){
    token->type = EOL;
    goto EXIT;
  }
  while (*ptr == ' ')
    ptr++;
  if (*ptr == '"'){
    token->value[0] = '\0';
    token->type = STRING;
    ptr++;
    goto EXIT;
  }

  //operator - uses isint to avoid stack use...
  if (isint = is_operator(*ptr)){
    token->value[0]= isint;
    token->type = OPERATOR;
    //needed at the bottom of the loop
    count++; 
    ptr++;
    if (isint == LT){
      if (*ptr == '>'){
	token->value[0] = NOT_EQ;
	ptr++;
      }
      else if (*ptr=='='){
	token->value[0] = LTE;
	ptr++;
      }
    }
    else if (isint == GT) {
      if (*ptr=='='){
	token->value[0] = GTE;
	ptr++;
      }
    }        
    goto EXIT;
  }
  //starts with int?
  if ((*ptr >= '0' && *ptr <= '9') || *ptr == '.')
    isint = 1;
  //compare start to position to determine lenth until end of string or space
  while(count < MAX_TOKEN_LENGTH && *ptr != '\0' && *ptr !=' ' && !is_operator(*ptr)){
    
    if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z')){
      //error - token started with numeral
      if (isint){
	if (*ptr=='E')
	  if (eread)
	    goto ERROR;
	  else
	    eread=1;
	else
	  goto ERROR;           
      }
    }
    if (*ptr == '.'){
      if (deciread)
	goto ERROR;
      deciread=1;
    }
    token->value[count] = *ptr;
    count++;
    ptr++;

  }

 LOOP_EXIT:
	//point string to ptr for advancing to next token
	if (deciread && isint)
	  token->type = FLOAT;
	else if (isint)
	  token->type = INTEGER;
	else
	  token->type = SYMBOL;
 EXIT:
	token->value[count] = '\0';
	*string = ptr;
	return;
 ERROR:
	strcpy(token->value, TOKEN_ERROR_1);
	token->type = INVALID;	
	return;
}
    

      
int main(int argv, char **argc)
{

  char *totoken = "1234.45+64 <> TOTALLY/678-10";
  char *sac = totoken;
  int count = 0;
  token a;
  read(&sac, &a);
  
  while (a.type != INVALID && a.type != EOL && count++<20 && *sac!='\0'){

    switch (a.type)
    {
      case OPERATOR:
	printf("Operator: %d\n", (int) a.value[0]);
	break;
    default:
		    
      printf("a->value: %s\n", a.value);
    }
    read(&sac, &a);
  }
  sac = totoken;
  expression( &sac);
  printf("stack contents:\n");
  for (int b =0; b <= working_top; b++){
    a = working_stack[b];
    switch (a.type)
      {
      case OPERATOR:
	printf("Operator: %d\n", (int) a.value[0]);
	break;
    default:
		    
      printf("a->value: %s\n", a.value);
    }
  };
    return 0;
    
}
  


