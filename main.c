#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include "tokens.h"
#include "globals.h"
#include "variables.h"
#include "expression.h"
#include "program.h"
/*****************************************************************************
 *****************************************************************************
 *****************************************************************************
                           Phrasing
			   ***************************************************
*/

int main(int argv, char **argc)
{

  //  char *totoken = "0";
  //char *taktak = "IF X<>5 AND Y>4 THEN FUN1(FUN2(2+3, fun4(4+5), 2+3*4)*2+2)+SYM3: ELSE X=6+3*(3-4):x$=\"hello world\"";
  //  char *taktak = "F=3:DIM X(F,4+1,5):DIM Z(2, 2):Z(0,0)=1: Z(1,1)=1: V=Z(0,0): J=Z(1,1): X(1*4,2,3)=5+4*(3*2)/2:Y=5+X(X(3,3,3),X(2,2*Z(1,V),2),1)";
  //add_line("10 DIM f$(10): a$ =\"hello world\": PRINT a$: f$(0)=a$: c$=f$(0): f$(0)=\"again\":PRINT f$(0)+\"Hello World\"");
  add_line("20 HI=1:HO=1+HI: DIM FO(10): FO(1)=HO: FO(0)=FO(1):PRINT FO(0): FO(3)=3: FO(FO(3))=3");

  //add_line("30 PRINT f$(0)+\" \"");
 //char *taktak = "DIM X(5, 5, 5): X(1*4,2,4)=6+2";
  //  char *taktak = "IF 1=2 THEN IF 1=1 THEN V=1 :ELSE V=2 :ELSE V=3";
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
  //sac = taktak;
  //execute_line(sac);
  /*  while (*sac!='\0' && *sac !='\n') {
    //printf("SAC%s\n", sac);
    expression(&sac);
    variable d = evaluate();
    printf("expression start\n");

    printf("FLOW CONTROL:\n");
    dump_token(control)
  }
  */
  execute();
  char input_buffer[200];
  fgets(input_buffer,20,stdin);
  return 0;

}
