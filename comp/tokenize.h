#ifndef TOKENIZE_H_INCLUDED
#define TOKENIZE_H_INCLUDED

#if defined(__IBMC__) || defined(__GNUC__)
#include <regex.h>
#elif defined(_MSC_VER)
#include <CRTDEFS.H> 
#include <regex.h>
#else
#endif

#define MAX_ATTR 20

typedef struct token{
    int   tkn_lnnum;
    int   tkn_colnum;
    int   tkn_len;

    char* tkn_val;
    char* tkn_type;
    char* tkn_attr[MAX_ATTR];
    int   tkn_attr_len;
} token;

typedef struct regex_npp{
    char*    name;
    char*    ptrn;
	regex_t* preg;

} regex_npp;

token      tokenizer();
int compil_regexp(regex_npp _regex_npp);
int erase_context();

#endif
