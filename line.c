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
  int if_counter=0;
  EQ_SWITCH = ASSIGNMENT;
  current=previous=line_text;
  //will set control to next....
  expression(&current);
  while(control.type != EOL && control.type != ERROR && *current!='\0'){    

    if (control.type == FLOW && control.value[0]==IF){
      EQ_SWITCH=REGULAR;
      expression(&current);
      //      dump_stack();
      evaluate();
      printf("Value of if expression: %f", v_stack[v_top].value.sing);
      if (control.type == FLOW && control.value[0]==THEN) {
	//logical true - we don't need to do anything.
	if (v_stack[v_top].value.sing != 0){
	  
	}
	else {
	  // read tokens until we get to newline, or else.
	  int if_counter=0;
	  token find_if;

	  //we break when we find a matching else, or we fall of the line
	  while (1){
	    read(&current, &find_if);
	    if (find_if.type == FLOW && find_if.value[0]==IF)
	      if_counter++;
	    if (find_if.type == FLOW && find_if.value[0]==ELSE){
	      if (if_counter >0) if_counter--;
	      else {
		break;
	      }	     	      
	    }
	    if(control.type == EOL)
	      break;	    	   	 	  	    
	  }
	  
	}
	
      }
    }
    if (control.type == FLOW && control.value[0] == ELSE)
      return 0;
            
    if (control.type == FLOW && control.value[0] == FOR) { }
    //assignment. Set up stack. Get PTR to read/write address
    if (control.type == FLOW && control.value[0] == DIM) {
      GLOBAL_STATE=IN_DIM;
      expression(&current);
      // dump_stack();
      evaluate();
      //set_variable(working_stack[working_top].value, &v_stack[v_top]);
      convert_stack_to_int_below_n(v_top);
      //check error
      unsigned int array_size = calculate_array_size();
      dim_array(&working_stack[working_top], array_size, v_top+1);
      printf("ARRAY DIMMED %s, %d dimensions\n", working_stack[working_top].value, v_top+1);
      printf("NUMBER OF ELEMENTS: %d\n", array_size);      
      printf("Dimension values: ");
      for (int a = v_top; a >= 0; a--){
	set_dimension_n(working_stack[working_top].value, a, v_stack[a].value.intg);
	printf("%d   ", v_stack[a].value.intg);
      }
      printf("\n");
    }
  
  
    
    if (control.type == OPERATOR && control.value[0]==EQ && EQ_SWITCH==ASSIGNMENT) {      
      //single symbol.
    GLOBAL_STATE = ASSIGNMENT;
    if (working_top == 0){
	token t1 = working_stack[0];	
	EQ_SWITCH = REGULAR;
	GLOBAL_STATE = REGULAR;
	expression(&current);
	//	dump_stack();
	evaluate();
	printf("VARIABLE ASSIGNMENT: %s - %f", t1.value, v_stack[v_top].type == F?v_stack[v_top].value.sing:(float)v_stack[v_top].value.intg);
	printf("v_top:%d\n, v_type:%d\n", v_top, t1.type);
	set_variable(t1.value, &v_stack[v_top]);
      }
      else {
	token t1 = working_stack[working_top];	
	//	dump_stack();
	evaluate();
	EQ_SWITCH = REGULAR;
	GLOBAL_STATE=REGULAR;
	unsigned int offset = (v_stack[v_top].type==F)? (int) v_stack[v_top].value.sing:v_stack[v_top].value.intg;
	expression(&current);
	//	dump_stack();
	evaluate();
	if (v_stack[v_top].type==STR)
	  printf("ARRAY ASSIGNMENT %s(%d)=%s\n", t1.value, offset, string_buffer);
	else
	  printf("ARRAY ASSIGNMENT %s(%d)=%f\n", t1.value, offset, (v_stack[v_top].type==I)? (float)v_stack[v_top].value.intg:v_stack[v_top].value.sing);
	set_value_in_array_from(t1.value, offset, &v_stack[v_top]);
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
      //ends statements. Not appearing before a then for instance, is an error.
    }
    GLOBAL_STATE=REGULAR;
    EQ_SWITCH=ASSIGNMENT;
    string_buffer_position=0;
    expression(&current);
  }  
}

