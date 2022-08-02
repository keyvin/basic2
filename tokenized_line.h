#ifndef TOKENIZED_LINE_H_INCLUDED
#define TOKENIZED_LINE_H_INCLUDED
#define MAX_LINE_LENGTH 1000

typedef struct TOKENIZED_LINE {
    uint16_t length;
    char *line;
} tokenized_line;

char *tlw_buffer[MAX_LINE_LENGTH];
int tokenize_line(char *);
void zero_buffer();

#endif // TOKENIZED_LINE_H_INCLUDED
