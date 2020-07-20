#include <stdio.h>
#include "globals.h"
#include "tokens.h"
#include "variables.h"
#include "expression.h"

variable evaluate();

//If I were to allow nesting ifs, have if counter
//returns line_no to execute next
unsigned int execute_line(char *line_text)
{
  //we sometimes double read. Oh well
  token t1;
  char *current;
  char *previous;
  EQ_SWITCH = ASSIGNMENT;
  current=previous=line_text;
  //will set control to next....
  expression(&current);
  while(control.type != EOL && control.type != ERROR && *current!='\0'){    
    if (control.type == FLOW && control.value[0]==IF){
      EQ_SWITCH=REGULAR;
    }
      
    //seek until end of line or else
    if (control.type == FLOW && control.value[0]==THEN) { }
    //seek until EOL, or just continue execution...
    if (control.type == FLOW && control.value[0]==ELSE){ }
    //treat first = as a statement break in read (assignment)
    if (control.type == FLOW && control.value[0] == FOR) { }
    //assignment. Set up stack. Get PTR to read/write address
    if (control.type == FLOW && control.value[0] == DIM) {
      GLOBAL_STATE=IN_DIM;
      expression(&current);
      dump_stack();
      evaluate();
      printf("ARRAY DIMMED %s, %d\n", working_stack[working_top].value, v_stack[v_top].value.intg);
      printf("NUMBER OF ELEMENTS: %f\n", v_stack[v_top-1].value.sing);      
      set_variable(working_stack[working_top].value, &v_stack[v_top]);     
    }
  
  
  
  if (control.type == OPERATOR && control.value[0]==EQ && EQ_SWITCH==ASSIGNMENT) {      
      //single symbol.
    GLOBAL_STATE = ASSIGNMENT;
    if (working_top == 0){

	token t1 = working_stack[0];
	
	EQ_SWITCH = REGULAR;
	GLOBAL_STATE = REGULAR;
	expression(&current);
	dump_stack();
	evaluate();
	printf("VARIABLE ASSIGNMENT: %s - %f", t1.value, v_stack[v_top].value.sing);
	set_variable(t1.value, &v_stack[v_top]);
      }
      else {
	token t1 = working_stack[working_top];	
	dump_stack();
	evaluate();
	EQ_SWITCH = REGULAR;
	GLOBAL_STATE=REGULAR;
	printf("ARRAY ASSIGNMENT %s\n %d", t1.value, v_stack[v_top].value.intg);	
	expression(&current);
	dump_stack();
	evaluate();
	
	//top of v_stack has name, offset
      }
      
	
      //else it's an array (or an error)
      
      //working stack top should have array name.
      
      //variable stack should have parameters
      //working_top should point to array_name.
      //if symbol -
      //else if array -
      //else error
      //name working_stack[working_top] 
      //if working_top is array, caluculate offset (var variable)

      
      //v_top will have integer offset and name set for assignment as variable. 
     
      //do assignment
    }
    if(control.type == OPERATOR && control.value[0]==COLON){

    }
    GLOBAL_STATE=REGULAR;
    EQ_SWITCH=ASSIGNMENT;
    expression(&current);
  }  
}

