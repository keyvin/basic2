#include "tokens.h"

#ifndef EXPRESSION_H
#define EXPRESSION_H 1


#define MAX_T_STACK 20

enum expression_result {SUCCESS, CONTINUE, FINISHED};
  
//variable working_v[3];
token working_stack[MAX_T_STACK];
token operator_stack[MAX_T_STACK];
int operator_top;
int working_top;
uint8_t pop_operators(uint8_t);

//shunting yard algorithm :)
//only need one - 
#endif
