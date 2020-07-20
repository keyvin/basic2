#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "tokens.h"
#include "expression.h"
#include "globals.h"
//#include "expressions.h"
//read token


//the : operator combines statements into a single line-num
//the ; tells print to omit a new line
uint8_t expression(char **);
variable evaluate();

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
  case ',':
    return COMMA;
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

//is this a function?
uint8_t is_function(char *value){
  for (int i = 0; i < NUM_FUNCTIONS; i++){
    if (strcmp(value, FUNCTION_MATCH[i])==0)
      return i+1;
  }
  return 0;

}

//reserved (multi char symbol?)
uint8_t reserved_symbol(token *sym){
  uint8_t match = 0;
  for (uint8_t a =0; a < STR_MATCH_NUM; a++){
    if (strcmp(STR_TO_TOKEN[a], sym->value)==0){

      if (a<3){
	sym->type = OPERATOR; 
	sym->value[0] = STR_MATCH[a];
      }
      else {
	sym->type = FLOW;
	sym->value[0] = STR_MATCH[a];
      }
      sym->value[1] = '\0';
    }
  }
  return 0;
}

//todo Check if int or float contains an E.

//modifies pointer passed in so there is no need to keep track
void read(char **string, token *token) {
  char *ptr = *string;
  uint8_t count = 0; 
  uint8_t isint = 0;
  uint8_t deciread = 0;
  uint8_t eread = 0;
  //Return an end of line 
  if (*ptr == '\0' || *ptr=='\n'){
    token->type = EOL;
    goto EXIT;
  }
  //chomp white space
  while (*ptr == ' ')
    ptr++;
  //read string
  if (*ptr == '"'){
    token->value[0] = '\0';
    token->type = STRING;
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
  while(count < MAX_TOKEN_LENGTH && *ptr != '\0' && *ptr !=' ' &&  !is_operator(*ptr)){
    
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
	if (token->type == SYMBOL){
	  reserved_symbol(token);
	  if(*ptr=='('){
	    token->type = ARRAY;

	    if (isint = is_function(token->value)){
	      token->type = FUNCTION;
	      token->value[0] = isint-1;
	    }
	      
	  }	      
	}
	if(token->type == STRING){
	  char *start = string_buffer;
	  ptr++;
	  while (*ptr != '"' &&  *ptr!= '\n' && *ptr != '\0')
	    *(start++)=*(ptr++);	  
	  *start='\0';
	  ptr++;
	}
	*string = ptr;
	return;
 ERROR:
	strcpy(token->value, TOKEN_ERROR_1);
	token->type = INVALID;	
	return;
}
    
void dump_token(token to_dump)
{

  switch (to_dump.type)   {
  case FLOAT:  
  case INTEGER:
  case SYMBOL:
    printf("SYMBOL:   %s\n", to_dump.value);
    break;
    
  case ERROR:
    printf("ERROR\n");
    break;

  case FLOW:
    // printf("FLOW Control: %s \n ", to_dump.value);
  case OPERATOR:
    printf("OPERATOR: %s, PRECEDENCE - %d\n", TOKEN_CHAR[to_dump.value[0]], TOKEN_PRECEDENCE[to_dump.value[0]]);
    break;
  case STRING:
    printf("STRING BUFFER %s\n", string_buffer);
    break;
  case INVALID:
    break;
  case ARRAY:
    printf("ARRAY OR FUNCTION %s\n", to_dump.value);
    break;
    
  default:
    break;
  }

}    



int main(int argv, char **argc)
{

  char *totoken = "0";
  //char *taktak = "IF X<>5 AND Y>4 THEN FUN1(FUN2(2+3, fun4(4+5), 2+3*4)*2+2)+SYM3: ELSE X=6+3*(3-4):x$=\"hello world\"";
  char *taktak = "5+4*(3*2)/2";
  char *sac = totoken;
  int count = 0;
  token a;
  read(&sac, &a);
  

  sac = totoken;
  expression( &sac );

  printf("stack contents:\n");
  for (int b=0; b<=working_top; b++){    
    dump_token(working_stack[b]);
  }
  sac = taktak;
  while (*sac!='\0' && *sac !='\n') {
    //printf("SAC%s\n", sac);
    expression(&sac);
    variable d = evaluate();
    printf("expression start\n");
    
    printf("FLOW CONTROL:\n");
    dump_token(control);

    
    	
    printf("stack contents:\n");
    for (int b=0; b<=working_top; b++){    
      dump_token(working_stack[b]);
    }
  }
  return 0;
    
}
  


