#include <stdio.h>
#include <assert.h>
#define __XPG4
#define __UU
#define __OE_8
#include <stdlib.h>
#define strcasecmp _stricmp
#include <string.h>

#include "debug.h"
#include "grammar.h"
#include "lexer.h"
#include "rexxCom.h"
#include "helper.h"

extern token lookahead;

int main(int argc, char** argv){

    getArgsRexx();
    lookahead  = getNextToken();

    ast* data = NULL;

    if (varrer_pgm()){

       if(data = data_division()){
          scr_line* screen;
          screen = init_screen();
          debug_1("Desenhando (%d) (%d) \n",data,data->tag);
          assert(data->tag == DATA_DIV);
          show_node(data->node.data_div.ws_sect,screen);
          print_boxes(screen);
          screen = init_screen();
          show_node(data->node.data_div.file_sect,screen);
          print_boxes(screen);
          screen = init_screen();
          show_node(data->node.data_div.link_sect,screen);
          print_boxes(screen);
          free_node(data);
       }
    }
	
    return 0;
}

