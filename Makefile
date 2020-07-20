basic2: expression.c tokens.c
	gcc -obasic2  expression.c tokens.c variables.c evaluate.c line.c --debug -I.
