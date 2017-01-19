#ifndef TOKENIZE_H_INCLUDED
#define TOKENIZE_H_INCLUDED

#if defined(__IBMC__) || defined(__GNUC__)
#include <regex.h>
#elif defined(_MSC_VER)
#include <CRTDEFS.H> 
#include <regex.h>
//#include <pcre.h>
#else
// error : compiler not supported
#endif

#define MAX_ATTR 20

/* token used by tokenizer and parser */
typedef struct token{
    int   tkn_lnnum;
    int   tkn_colnum;
    int   tkn_len;

    char* tkn_val;
    char* tkn_type;
    char* tkn_attr[MAX_ATTR];
    int   tkn_attr_len;
} token;

/* struct for regex name pattern and pregex */
typedef struct regex_npp{
    char*    name;
    char*    ptrn;
	regex_t* preg;

} regex_npp;

token      tokenizer();
int compil_regexp(regex_npp _regex_npp);
int erase_context();

typedef struct context{
          char*  division;             /* Division                   */
          char*  section;              /* Section                    */
          char*  sentence;             /* Sentence                   */
          char*  clause;               /* Clause                     */
          char*  keyword;              /* Keyword                    */
          char*  other;                /* Other                      */
} context;

#endif
/* TOKENIZE_H_INCLUDED */


/* token used by tokenizer and parser */
typedef struct token{
    int   tkn_lnnum;
    int   tkn_colnum;
    int   tkn_len;

    char* tkn_val;
    char* tkn_type;
    char* tkn_attr[MAX_ATTR];
    int   tkn_attr_len;
} token;

/* struct for regex name pattern and pregex */
typedef struct regex_npp{
    char*    name;
    char*    ptrn;
	regex_t* preg;

} regex_npp;

token      tokenizer();
int compil_regexp(regex_npp _regex_npp);
int erase_context();

typedef struct context{
          char*  division;             /* Division                   */
          char*  section;              /* Section                    */
          char*  sentence;             /* Sentence                   */
          char*  clause;               /* Clause                     */
          char*  keyword;              /* Keyword                    */
          char*  other;                /* Other                      */
} context;

