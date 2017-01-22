#include <stdio.h>
#include <assert.h>
//#include <string.h>
#define __XPG4 // itoa, strccase
#define __UU
#define __OE_8
#include <stdlib.h>
#if defined(__IBMC__) || defined(__GNUC__)
#include <strings.h>
#elif defined(_MSC_VER)
#include <string.h>
#define strcasecmp _stricmp
#else
// error : compiler not supported
#endif
#include <string.h>

#include "debug.h"
#include "grammar.h"
#include "lexer.h"
#include "rexxCom.h"
#include "helper.h"
#include "generate.h"
#include "check.h"

extern token lookahead;

int main(int argc, char** argv){

    getArgsRexx();
    lookahead  = getNextToken();

    ast* data=NULL;

    if (balayeur_pgm()){

       if(data=data_division()){
          scr_line* screen;
          screen=init_screen();
          debug_1("Drawing (%d) (%d) \n",data,data->tag);
          assert(data->tag == DATA_DIV);
      //  affich_node(data,screen);
          affich_node(data->node.data_div.ws_sect,screen);
          print_boxes(screen);
          screen=init_screen();
          affich_node(data->node.data_div.file_sect,screen);
          print_boxes(screen);
          screen=init_screen();
          affich_node(data->node.data_div.link_sect,screen);
          print_boxes(screen);
          free_node(data);
       }
    }
    debug_1(" END MAIN. \n");
    return 0;
}

