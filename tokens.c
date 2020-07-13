#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
//read token

#define TOKEN_ERROR_1 "INVALID TOKEN"
#define MAX_TOKEN_LENGTH 20
#define MAX_STRING 128


enum t_type {VARIABLE, INTEGER, FLOAT, ERROR, OPERATOR, STRING,EOL, INVALID};

typedef struct TOKEN {
  char value[MAX_TOKEN_LENGTH];
  enum t_type type;
} token;

token buffer;
char read_buffer[MAX_STRING];
//pas by value 
uint8_t is_operator(char val)
{
  if (val == '+' || val == '-' || val =='/' || val == '*' || val == '^' || val =='(' || val == ')' )
    return 1;
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
  //advance whitespace
  if (*ptr == '\0'){
    token->type = EOL;
  }
  while (*ptr == ' ')
    ptr++;
  if (*ptr == '"'){
    token->value[0] = '\0';
    token->type = STRING;
    ptr++;
    goto EXIT;
  }
  //operator
  if (is_operator(*ptr)){
    token->value[0]=*ptr;
    token->type = OPERATOR;
    ptr++;
    goto EXIT;
  }
  //starts with int?
  if ((*ptr >= '0' && *ptr <= '9') || *ptr == '.')
    isint = 1;
  //compare start to position to determine lenth until end of string or space
  while((ptr - *string) < MAX_TOKEN_LENGTH && *ptr != '\0' && *ptr !=' ' && !is_operator(*ptr)){
    
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
    //unnessecary?
    if (*ptr >='0' && *ptr <='9'){
      
    }
    if (*ptr == '.'){
      if (deciread)
	goto ERROR;
      deciread=1;
    }
    token->value[ptr-*string] = *ptr;
    ptr++;

  }
  goto LOOP_EXIT;
 ERROR:
	strcpy(token->value, TOKEN_ERROR_1);
	token->type = INVALID;	
	return;
 LOOP_EXIT:
	//point string to ptr for advancing to next token
	if (deciread && isint)
	  token->type = FLOAT;
	else if (isint)
	  token->type = INTEGER;
	else
	  token->type = VARIABLE;
 EXIT:
	token->value[ptr-*string] = '\0';
	*string = ptr;
	return;
}
    

      
int main(int argv, char **argc)
{

  char *totoken = "1234.45+64+TOTALLY/6(78)(-10)";
  char *sac = totoken;
  int count = 0;
  token a;
  read(&sac, &a);
  
  while (a.type != INVALID && a.type != EOL && count++<20 && *sac!='\0'){

    switch (a.type)
    {
      case OPERATOR:
	printf("Operator: %c\n", a.value[0]);
	break;
    default:
		    
      printf("a->value: %s\n", a.value);
    }
    read(&sac, &a);
  }

  return 0;
}
  


