#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include "tokenize.h"
#include "ast.h"

/* ln_str_num used by String function  */
typedef struct ln_str_num{
    char* ln_str;
    int   ln_num;
} ln_str_num;

/* ln_obj used by preprocessor     */
typedef struct ln_obj{
    char  ln_1_6[7];
    char  ln_7[2];
    char  ln_8_72[66];
    char  ln_73_80[9];
    int   ln_num;
    int   ln_indent;
} ln_obj;

token      getNextToken();
// ln_str_num getLineString(int);
ln_str_num getLineString();
ln_obj     preprocessor(ln_str_num);

ast* copy_replacing();


#endif
/* LEXER_H_INCLUDED */
