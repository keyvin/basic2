#include "tokens.h"

#ifndef EXPRESSION_H
#define EXPRESSION_H 1


#define MAX_T_STACK 20

enum expression_result {SUCCESS, CONTINUE, FINISHED};
  
//variable working_v[3];
extern token working_stack[MAX_T_STACK];
extern token operator_stack[MAX_T_STACK];
extern int operator_top;
extern int working_top;
extern token control;
uint8_t pop_operators(uint8_t);
uint8_t expression(char **);

//shunting yard algorithm :)
//only need one - 
#endif
