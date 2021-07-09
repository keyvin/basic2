//superseded by program.h

#ifndef LINE_H
#define LINE_H 1

#define MAX_LINE_LENGTH

//I could define my own string type still

typedef struct LINE {
  unsigned int line_no;
  char line[256];
}

char *execute_line(char * );


#endif
