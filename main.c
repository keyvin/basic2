#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include "tokens.h"
#include "globals.h"
#include "variables.h"
#include "expression.h"



int main(int argv, char **argc)
{

  //  char *totoken = "0";
  //char *taktak = "IF X<>5 AND Y>4 THEN FUN1(FUN2(2+3, fun4(4+5), 2+3*4)*2+2)+SYM3: ELSE X=6+3*(3-4):x$=\"hello world\"";
  char *taktak = "DIM X(3,4,5):DIM Z(2, 2) : V=22+1: X(1*4,2,3)=5+4*(3*2)/2:Y=5+X(X(3,3,3),X(2,2*Z(1,V),2),1)";
  char *sac ;
  // int count = 0;
  // token a;
  //  read(&sac, &a);


  //  sac = totoken;
  // expression( &sac );

  //  printf("stack contents:\n");
  //  for (int b=0; b<=working_top; b++){
  //    dump_token(working_stack[b]);
  //  }
  sac = taktak;
  execute_line(sac);
  /*  while (*sac!='\0' && *sac !='\n') {
    //printf("SAC%s\n", sac);
    expression(&sac);
    variable d = evaluate();
    printf("expression start\n");

    printf("FLOW CONTROL:\n");
    dump_token(control);



  }
  */
  return 0;

}
