#include <stdio.h>

#define TOK_NUM   0
#define TOK_OP    1
#define TOK_PONT  2

#define PARESQ 0
#define PARDIR 1

#define SOMA 0
#define SUB  1
#define MULT 2
#define DIV  3


FILE* file;
int crrFilePointer;

typedef struct
{
    int tipo;
    int valor;
    int* coluna;
    int* linha;
}Token;

char *codigo;
int tamanho;
int pos;

