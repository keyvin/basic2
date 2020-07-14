#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdint.h>

#define VARNAME_MAX 20

enum var_types { I, S, D, STR, IA, SA, DA, STRA };

typedef union VALUE {
  int intg;
  float sing;
  double dubl;
  char *str;
  void *array;
} value;

//First char of varname is type (i, s, 
typedef struct VARIABLE {
  char name [VARNAME_MAX];
  value val;
  var_types type;
} variable;

#endif
