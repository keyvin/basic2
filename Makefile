 all: basic2
 Debug: basic2
 basic2: expression.c tokens.c variables.c evaluate.c line.c main.c globals.c
	gcc -obasic  main.c expression.c tokens.c variables.c evaluate.c line.c globals.c --debug -I.
