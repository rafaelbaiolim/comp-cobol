#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// isspace
#if defined(_MSC_VER)
#include <ctype.h>
#endif
#include "debug.h"
#include "lexer.h"
#include "tokenize.h"
#include "grammar.h"
#include "rexxCom.h"

int i = 0;
_srcLine* src_list;
_srcLine* curr_src_line;

/* curr_ln_obj stores the actual string to tokenize */
static ln_obj curr_ln_obj;

extern token lookahead;

token getNextToken()
{

    static token _ret;

    debug_3("%s", curr_src_line);

    _ret = tokenizer(curr_src_line);
    _ret.tkn_lnnum = curr_src_line->ln_num;

    curr_src_line = curr_src_line->next;
    lookahead = _ret;

    if(copy_replacing()) {
        ;
    }

    debug_2("*----------------------------------------------*\n");
    debug_2("*--------------   TOKENIZER    ----------------*\n");
    debug_2("*----------------------------------------------*\n");
    debug_2("*   Token Value : %s \n", _ret.tkn_val);
    debug_2("*   Token Type  : %s \n", _ret.tkn_type);
    debug_2("*   Token Line  : %d \n", _ret.tkn_lnnum);
    debug_2("*   Token Coln  : %d \n", _ret.tkn_colnum);
    debug_2("*   Token Length: %d \n", _ret.tkn_len);
    debug_2("*   Token attrs : %d \n", _ret.tkn_attr_len);
    debug_2("*----------------------------------------------*\n");

    return _ret;
}

ln_str_num getLineString()
{

    ln_str_num _ret;

    debug_1("curr_src_line (%d)\n", curr_src_line);
    debug_1("curr_src_line next (%d)\n", curr_src_line->next);
    debug_1("value         (%s)\n", curr_src_line->value);
    debug_1("num_all       (%d)\n", curr_src_line->num_all);
    debug_1("num_file      (%d)\n", curr_src_line->num_file);
    debug_1("file          (%s)\n", curr_src_line->file);
    if(!curr_src_line) {
        _ret.ln_num = 0;
    } else {

        /* else return line from rawcode and line number   */
        _ret.ln_str = curr_src_line->value;
        _ret.ln_num = curr_src_line->num_all;
        curr_src_line = curr_src_line->next;
    }
    debug_1("num(%d)str<%s>\n", _ret.ln_num, _ret.ln_str);
    debug_1("strlen(%d)    \n", strlen(_ret.ln_str));
    debug_1("AFTER !!! curr_src_line (%d)\n", curr_src_line);

    return _ret;
}

ln_obj preprocessor(ln_str_num lnstrnum)
{

    ln_obj _ret = { "", "", "", "", 0, 0 };
    int i = 0;

    if(strlen(lnstrnum.ln_str) == 80) {

        strncpy(_ret.ln_1_6, lnstrnum.ln_str, 6);
        *(_ret.ln_1_6 + 6) = '\0';
        strncpy(_ret.ln_7, lnstrnum.ln_str + 6, 1);
        *(_ret.ln_7 + 1) = '\0';
        strncpy(_ret.ln_8_72, lnstrnum.ln_str + 7, 65);
        *(_ret.ln_8_72 + 65) = '\0';
        strncpy(_ret.ln_73_80, lnstrnum.ln_str + 72, 8);
        *(_ret.ln_73_80 + 8) = '\0';

        while(isspace(*(_ret.ln_8_72 + i)))
            i++;

        _ret.ln_indent = i + 1;
        _ret.ln_num = lnstrnum.ln_num;
    }

    /* '*' = 92 */
    /* '-' = 96 */
    /* '/' = 97 */

    if(((*(_ret.ln_7)) == 92) || ((*(_ret.ln_7)) == 97)) {
        debug_2(" Comment line (%d)\n", _ret.ln_num);
        debug_2(" _ret.ln_8_72 <%s>\n", _ret.ln_8_72);
        /*  sprintf(_ret.ln_8_72,"%-65s"); */
        *(_ret.ln_8_72) = '\0';
        debug_2(" _ret.ln_8_72 <%s>\n", _ret.ln_8_72);
        debug_2("  len ln_8_72 (%d)\n", strlen(_ret.ln_8_72));
        return _ret;
    }

    if((*(_ret.ln_7)) == 96) {
        printf(" Continuation not supported line (%d)\n", _ret.ln_num);
        *(_ret.ln_8_72) = '$';
        *(_ret.ln_8_72 + 1) = '\0';
        exit(EXIT_FAILURE);
    }

    if(_ret.ln_indent == 66) {
        debug_2(" Empty Line   (%d)\n", _ret.ln_num);
        *(_ret.ln_8_72) = '\0';
        return _ret;
    }
    return _ret;
}
