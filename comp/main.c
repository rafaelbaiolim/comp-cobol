#include <stdio.h>
#include <assert.h>
//#include <string.h>
#define __XPG4 // itoa, strccase
#define __UU
#define __OE_8
#include <stdlib.h>

#include <string.h>

#include "lexer.h"
#include "debug.h"
#include "generate.h"
#include "tokenize.h"
#include "check.h"
#include "rexxCom.h"

extern token lookahead;
extern _srcLine   *curr_src_line; 

int main(int argc, char** argv){
    getArgsRexx("teste.cbl");
    lookahead = getNextToken();

    return 0;
}

