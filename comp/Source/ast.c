#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#define __UU
#define __OE_8
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#if defined(_MSC_VER)
#endif
#include "debug.h"
#include "ast.h"


/*
 * AST Building functions
 */

char* usageValues[] = {
	"UNKNOWN_USAGE",
	"BINARY",
	"COMPUTATIONAL",
	"COMP",
	"COMPUTATIONAL-1",
	"COMP-1",
	"COMPUTATIONAL-2",
	"COMP-2",
	"COMPUTATIONAL-3",
	"COMP-3",
	"COMPUTATIONAL-4",
	"COMP-4",
	"DISPLAY",
	"DISPLAY-1",
	"INDEX",
	"PACKED-DECIMAL",
	"POINTER",
	"PROCEDURE-POINTER"
};
char* tagValues[] = {
	"UNKNOWN_AST",
	"SENTENCE",
	"DECLARATION",
	"DATA_DIV",
	"FIELD",
	"OCCURS",
	"OCCURS_KEY",
	"PIC_CMPNT",
	"FIELD_SIGN",
	"FIELD_VALUE",
	"FIELD_SYNC",
	"USAGE",
	"RENAMES",
	"STATEMENT",
	"MOVE_STM", "DISPLAY_STM", "INITIALIZE_STM",
	"OPERAND",
	"LITERAL",
	"IDENTIFIER", "IDENT_NAME", "IDENT_NAME_QUALIF", "IDENT_QUALIF",
	"IDENT_SUBSCRIPT", "IDENT_REFMOD",
	"SPECIAL_REG",
	"ARITH_EXP",
	"BINARY_OP"
};

static int lvl=-1;

/*---   SENTENCE   ---*/
ast*
make_sentence    (int nb_statement, struct ast* list_statement){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.sentence.ast_name          = "SENTENCE";
  e->tag = SENTENCE;

  e->node.sentence.nb_statement = nb_statement;
  e->node.sentence.list_statement = NULL;

  /* if(nb_statement > 0 ) */
  e->node.sentence.list_statement = list_statement;

  e->next =NULL;

  debug_2("**********  SENTENCE   ***************\n");
  debug_2("* SENTENCE AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("**************************************\n");

  return e;

}

ast*
make_ast (){

  ast* e = (ast*) malloc(sizeof(ast));
  e->next =NULL;
  e->tag = UNKNOWN_AST;

  return e;

}

affich_sentence(ast* tree,scr_line* screen){


   ast* temp = NULL;

   lvl++;
   draw_box(screen,tree->node.sentence.ast_name,"",lvl);

   temp=tree->node.sentence.list_statement;

   while(temp != NULL){
      affich_statement(temp,screen);
      temp = temp->next;
   }

   lvl--;

}

affich_statement(ast* tree,scr_line* screen){

   debug_2("Switch for statement ... \n");
   ast* temp = NULL;

   debug_3("Statement (%s) \n",tagValues[tree->tag]);
   debug_2("Statement (%d) \n",tree->tag);

   lvl++;

   if      (tree->tag == MOVE_STM)

           affich_move_stm(tree,screen);

   else if (tree->tag == DISPLAY_STM)

           affich_display_stm(tree,screen);

   else if (tree->tag == INITIALIZE_STM)

           affich_initialize_stm(tree,screen);

   else {

        /* printf("Unknown Tag for statement (%s) \n",
                                tagValues[tree->tag]);
        */ printf("Unknown Tag for statement (%d) \n",
                                          tree->tag );
           exit(EXIT_FAILURE);
   }

   lvl--;
}

/*---   MOVE   ---*/
ast*
make_move        (int bool_corresp, int nb_operand_r,
                  struct ast* operand_g, struct ast* list_operand_r){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.move_stm.ast_name          = "MOVE";
  e->tag = MOVE_STM;

  e->node.move_stm.bool_corresp      = bool_corresp;
  e->node.move_stm.nb_operand_r      = nb_operand_r;
  e->node.move_stm.operand_g         = operand_g   ;
  e->node.move_stm.list_operand_r    = list_operand_r;

  e->next =NULL;
  debug_2("**********  MOVE       **********\n");
  debug_2("* MOVE    AST BUILT SUCCESFULLY *\n");
  debug_2("**********************************\n");

  return e;

}

affich_move_stm(ast* tree,scr_line* screen){

   ast* temp = NULL;

   char* value=malloc(125);

   if (tree->node.move_stm.bool_corresp)
      strcpy(value,"CORRESP ");
   else
      strcpy(value,"");

   draw_box(screen,tree->node.move_stm.ast_name,value,lvl);
   strcpy(value,"");

   lvl++;
   affich_node(tree->node.move_stm.operand_g,screen);
   lvl--;

   draw_box(screen,"TO","",lvl);
   strcpy(value,"");

   temp=tree->node.move_stm.list_operand_r;

   lvl++;
   while(temp != NULL){
      debug_3("Printing inner nodes (%s) \n",tagValues[temp->tag]);
      debug_2("Printing inner nodes (%d) \n",          temp->tag );
      affich_operand(temp,screen);
      temp=temp->next;
   }
   lvl--;

}
/*---   DISPLAY   ---*/
ast*
make_display (int bool_upon, int bool_no_adv, char* upon_operand,
                   int nb_operand, struct ast* list_operand){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.display_stm.ast_name          = "DISPLAY";
  e->tag = DISPLAY_STM;

  e->node.display_stm.bool_upon         = bool_upon;
  e->node.display_stm.bool_no_adv       = bool_no_adv;
  strcpy(e->node.display_stm.upon_operand,upon_operand);
  /*e->node.display_stm.upon_operand = upon_operand; */
  e->node.display_stm.nb_operand        = nb_operand;
  e->node.display_stm.list_operand   = NULL;

  if(nb_operand > 0 )
     e->node.display_stm.list_operand   = list_operand;

  e->next =NULL;

  debug_2("**********  DISPLAY    **********\n");
  debug_2("* DISPLAY AST BUILT SUCCESFULLY *\n");
  debug_2("**********************************\n");

  return e;

}

affich_display_stm(ast* tree,scr_line* screen){

   ast* temp = NULL;

   draw_box(screen,tree->node.display_stm.ast_name,"",lvl);

   temp=tree->node.display_stm.list_operand;
   lvl++;
   while(temp != NULL){
      debug_3("Printing inner nodes (%s) \n",tagValues[temp->tag]);
      debug_2("Printing inner nodes (%d) \n",          temp->tag );
      affich_operand(temp,screen);
      temp=temp->next;
   }
   lvl--;

}

ast*
make_initialize  (int bool_repl, int nb_operand,
                  struct ast* list_operand){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.initialize_stm.ast_name          = "INITIALIZE";
  e->tag = INITIALIZE_STM;

  e->node.initialize_stm.bool_repl         = bool_repl;
  e->node.initialize_stm.nb_operand        = nb_operand;
  e->node.initialize_stm.list_operand      = list_operand;

  e->next =NULL;
  debug_2("**********  INITIALIZE   **********\n");
  debug_2("* INITIALIZE AST BUILT SUCCESFULLY *\n");
  debug_2("************************************\n");

  return e;


}

affich_initialize_stm(ast* tree,scr_line* screen){

   ast* temp = NULL;

   char* value=malloc(125);

   if (tree->node.initialize_stm.bool_repl)
      strcpy(value,"REPLACING ");
   else
      strcpy(value,"");

   draw_box(screen,tree->node.initialize_stm.ast_name,value,lvl);
   strcpy(value,"");


   temp=tree->node.initialize_stm.list_operand;

   lvl++;
   while(temp != NULL){
      debug_3("Printing inner nodes (%s) \n",tagValues[temp->tag]);
      debug_2("Printing inner nodes (%d) \n",          temp->tag );
      affich_operand(temp,screen);
      temp=temp->next;
   }
   lvl--;
}


/*---   OPERAND   ---*/
ast*
make_operand (int bool_literal, int bool_identifier,
                  struct ast* operand){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.operand.ast_name          = "OPERAND";
  e->tag = OPERAND;


  e->next =NULL;
  debug_3("**********  OPERAND    **********\n");
  debug_3("* OPERAND AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_3("**********************************\n");

  return e;

}

affich_operand(ast* tree,scr_line* screen){

   ast* temp = NULL;

   if      (tree->tag == LITERAL)

           affich_literal(tree,screen);

   else if (tree->tag == IDENTIFIER)

           affich_identifier(tree,screen);

   else exit(EXIT_FAILURE);

}

/*---   LITERAL   ---*/
ast*
make_literal     (char* value,
                  int bool_all, int bool_numeric,
                  int bool_alphanumeric,
                  int bool_hexadecimal, int bool_positive,
                  int bool_negative, int bool_unsigned,
                  int bool_singl_q, int bool_doubl_q,
                  int bool_intgr,   int bool_decim,
                  int bool_figurative,
                  int char_length){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.literal.ast_name          = "LITERAL";
  e->tag = LITERAL;

  e->node.literal.bool_all          = bool_all;
  e->node.literal.bool_numeric      = bool_numeric;
  e->node.literal.bool_alphanumeric = bool_alphanumeric;
  e->node.literal.bool_hexadecimal  = bool_hexadecimal;
  e->node.literal.bool_positive     = bool_positive;
  e->node.literal.bool_negative     = bool_negative;
  e->node.literal.bool_unsigned     = bool_unsigned;
  e->node.literal.bool_singl_q      = bool_singl_q;
  e->node.literal.bool_doubl_q      = bool_doubl_q;
  e->node.literal.bool_intgr        = bool_intgr;
  e->node.literal.bool_decim        = bool_decim;
  e->node.literal.bool_figurative   = bool_figurative;
  e->node.literal.char_length       = char_length;


    strcpy(e->node.literal.value,value);
  /* e->node.literal.value=value; */
  /*sprintf(e->node.literal.value,"%.*s",strlen(value),value);*/

  e->next =NULL;

  debug_2("**********  LITERAL   **********\n");
  debug_2("* LITERAL AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("*********************************\n");

  return e;

}

affich_literal(ast* tree,scr_line* screen){

   /* comportement bizzare des strings */
   /* str continue toujours a garder sa valeur tout au long du pgm */
   char* value=malloc(125);
   strcpy(value,"ALL ");

   if (tree->node.literal.bool_all) {
      strcat(value,tree->node.literal.value); // modif , makefile
      draw_box(screen,tree->node.literal.ast_name,
               value,                       lvl);

   } else

   draw_box(screen,tree->node.literal.ast_name,
            tree->node.literal.value,lvl);

   strcpy(value,"");

}

ast*
make_identifier  (int bool_length, int bool_address,
                  int bool_special_reg,
                  struct ast* name, struct ast* qualif,
                  struct ast* subscript, struct ast* refmod){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.identifier.ast_name          = "IDENTIFIER";
  e->tag = IDENTIFIER;

  e->node.identifier.bool_length       = bool_length;
  e->node.identifier.bool_address      = bool_address;
  e->node.identifier.bool_special_reg  = bool_special_reg;
/*strcpy(e->node.identifier.special_reg_value,special_reg_value);*/
/*e->node.identifier.special_reg_value=special_reg_value;*/

  e->node.identifier.name              = name;
  e->node.identifier.qualif            = qualif;
  e->node.identifier.subscript         = subscript;
  e->node.identifier.refmod            = refmod;

  e->next =NULL;
  debug_2("**********  IDENTIFIER   **********\n");
  debug_2("* IDENTIFIER AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("************************************\n");

  return e;

}

affich_identifier(ast* tree,scr_line* screen){

   char* value_1=malloc(125);
   char* value_2=malloc(125);
   strcpy(value_1,"");
   strcpy(value_2,"");

   if(tree->node.identifier.bool_address)
      strcpy(value_1,"ADDRESS OF ");

   if(tree->node.identifier.bool_length)
      strcpy(value_1,"LENGTH OF ");

   if(tree->node.identifier.bool_special_reg)
      strcpy(value_2,"SPECIAL REG ");


   ast* name = tree->node.identifier.name;
   strcat(value_2,tree->node.identifier.ast_name); // modif , makefile
   draw_box(screen,value_2,
                          value_1,lvl);

   strcpy(value_1,"");
   strcpy(value_2,"");

   lvl++;

   debug_2("Identifier name  ? (%d)\n",tree->node.identifier.name);
   if(tree->node.identifier.name != NULL)
      affich_ident_name(tree->node.identifier.name,screen);

   debug_2("Identifier qualif? (%d)\n",tree->node.identifier.qualif);
   if(tree->node.identifier.qualif != NULL)
      affich_ident_qualif(tree->node.identifier.qualif,screen);

   debug_2("Identifier subscr  (%d)\n",tree->node.identifier.subscript);;
   if(tree->node.identifier.subscript != NULL)
      affich_ident_subscript(tree->node.identifier.subscript,screen);

   debug_2("Identifier refmod ? (%d)\n",tree->node.identifier.refmod);
   if(tree->node.identifier.refmod != NULL)
      affich_ident_refmod(tree->node.identifier.refmod,screen);
   lvl--;
}


ast*
make_ident_name_qualif
                 (struct ast* name, struct ast* qualif){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.ident_name_qualif.ast_name          = "IDENT_NAME_QUALIF";
  e->tag = IDENT_NAME_QUALIF;

  e->node.ident_name_qualif.name       = name;
  e->node.ident_name_qualif.qualif     = qualif;


  e->next =NULL;
  debug_2("*********** ID NAME QUALIF  ***********\n");
  debug_2("* ID NAME QUALIF AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("****************************************\n");

  return e;

}

affich_ident_name_qualif(ast* tree,scr_line* screen){


}

ast*
make_ident_name  (int length, char* value){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.ident_name.ast_name          = "IDENT_NAME";
  e->tag = IDENT_NAME;

  e->node.ident_name.length            = length;
  strcpy(e->node.ident_name.value,value);

  e->next =NULL;
  debug_2("*********** ID NAME  ***********\n");
  debug_2("* ID NAME AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("*********************************\n");
  return e;


}

affich_ident_name(ast* tree,scr_line* screen){

   draw_box(screen,tree->node.ident_name.ast_name,
                   tree->node.ident_name.value,lvl);
}

ast*
make_ident_qualif(struct ast* list_qualif){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.ident_qualif.ast_name          = "IDENT_QUALIF";
  e->tag = IDENT_QUALIF;

  e->node.ident_qualif.list_qualif     = list_qualif;


  e->next =NULL;
  debug_2("*********** ID QUALIF  ***********\n");
  debug_2("* ID QUALIF AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("***********************************\n");
  return e;

}

affich_ident_qualif(ast* tree,scr_line* screen){

   ast* temp = NULL;


   temp=tree->node.ident_qualif.list_qualif;

   lvl++;
   while(temp != NULL){
      draw_box(screen,tree->node.ident_qualif.ast_name,
                   temp->node.ident_name.value,lvl);

      temp=temp->next;
   }
   lvl--;

}


ast*
make_ident_subscript
                 (struct ast* list_subscript){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.ident_subscript.ast_name          = "IDENT_SUBSCRIPT";
  e->tag = IDENT_SUBSCRIPT;

  e->node.ident_subscript.list_subscript  = list_subscript;

  debug_2("*********** ID SUBSCR  ***********\n");
  debug_2("* ID SUBSCR AST BUILT SUCCESFULLY *\n");
  debug_2("***********************************\n");


  e->next =NULL;
  return e;


}

affich_ident_subscript(ast* tree,scr_line* screen){

   ast* temp = NULL;

   draw_box(screen,tree->node.ident_subscript.ast_name,"",lvl);

   temp=tree->node.ident_subscript.list_subscript;

   lvl++;
   while(temp != NULL){
      debug_3("Printing (%s) (%d)     \n",tagValues[temp->tag],temp);
      debug_2("Printing (%d) (%d)     \n",          temp->tag ,temp);
      affich_arith_exp(temp,screen);
      /*
      draw_box(screen,tree->node.ident_subscript.ast_name,
                      temp->node.arith_exp.value,lvl);
      */
      temp=temp->next;
   }
   lvl--;
}


ast*
make_ident_refmod(struct ast* charleft, struct ast* length){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.ident_refmod.ast_name          = "IDENT_REFMOD";
  e->tag = IDENT_REFMOD;

  e->node.ident_refmod.charleft        = charleft;
  e->node.ident_refmod.length          = length;

  debug_2("*********** ID REFMOD  ***********\n");
  debug_2("* ID REFMOD AST BUILT SUCCESFULLY *\n");
  debug_2("***********************************\n");


  e->next =NULL;
  return e;


}

affich_ident_refmod(ast* tree,scr_line* screen){


   draw_box(screen,tree->node.ident_refmod.ast_name,"",lvl);

   lvl++;

   draw_box(screen,"OFFSET","",lvl);
   affich_arith_exp(tree->node.ident_refmod.charleft,screen);

   if(tree->node.ident_refmod.length){
     draw_box(screen,"LENGTH","",lvl);
     affich_arith_exp(tree->node.ident_refmod.length,screen);
   }
   lvl--;

}

ast*
make_arith_exp   (char* value, char* oper, char* sign,
                  struct ast* left, struct ast* right){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.arith_exp.ast_name          = "ARITH_EXP";
  e->tag = ARITH_EXP;

  strcpy(e->node.arith_exp.value,value);
  strcpy(e->node.arith_exp.oper ,oper );
  strcpy(e->node.arith_exp.sign ,sign );

  e->node.arith_exp.left              = left;
  e->node.arith_exp.right             = right;

  e->next =NULL;
  return e;

}

affich_arith_exp(ast* tree,scr_line* screen){

   /*draw_box(screen,tree->node.arith_exp.ast_name,"",lvl);*/

   if (strlen(tree->node.arith_exp.sign)>0)
      draw_box(screen,"SIGN",tree->node.arith_exp.sign,lvl);

   if (tree->node.arith_exp.left)
      affich_node(tree->node.arith_exp.left,screen);

   if (strlen(tree->node.arith_exp.oper)>0)
      draw_box(screen,"OPERATION",tree->node.arith_exp.oper,lvl);

   if (tree->node.arith_exp.right)
      affich_node(tree->node.arith_exp.right,screen);

}

/*--- DATA_DIV ---*/
ast*
make_data_div    (struct ast* file_sect, struct ast* ws_sect,
                  struct ast* link_sect){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.data_div.ast_name          = "DATA_DIV";
  e->tag = DATA_DIV;

  e->node.data_div.file_sect         = file_sect;
  e->node.data_div.ws_sect           = ws_sect;
  e->node.data_div.link_sect         = link_sect;

  debug_2("*********** DATA DIV   ***********\n");
  debug_2("* DATA DIV  AST BUILT SUCCESFULLY *\n");
  debug_2("***********************************\n");

  e->next =NULL;
  return e;

}

/*---   FIELD    ---*/
ast*
make_field (struct ast*  name,
            struct ast*  copy,
            field_name   field_name,
            field_lvl    field_lvl,
            int          lvl,
            int          name_length,
            int          size,
            int          offset,
            int          nb_indexes,
            int          nb_parents,
            int          bool_blank_zero,
            int          bool_justified,
            int          bool_global,
            int          bool_external,
            int          bool_bouchon,
            int          nb_bouchon,
            struct ast*  occurs,
            struct ast*  usage,
            struct ast*  field_value,
            struct ast*  cond_88,
            struct ast*  parent,
            struct ast*  children,
            struct ast*  redefines,
            struct ast*  renames,
            struct ast*  search,
            struct ast*  picture,
            struct ast*  field_sign,
            struct ast*  field_sync,
            section_t    section
           ){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.field.ast_name                = "FIELD";
  e->tag = FIELD;

  e->node.field.name                    = name;
  e->node.field.copy                    = copy;
  e->node.field.field_name              = field_name;
  e->node.field.field_lvl               = field_lvl;
  e->node.field.lvl                     = lvl;
  e->node.field.name_length             = name_length;
  e->node.field.size                    = size;
  e->node.field.offset                  = offset;
  e->node.field.nb_indexes              = nb_indexes;
  e->node.field.nb_parents              = nb_parents;
  e->node.field.bool_blank_zero         = bool_blank_zero;
  e->node.field.bool_justified          = bool_justified;
  e->node.field.bool_global             = bool_global;
  e->node.field.bool_external           = bool_external;
  e->node.field.bool_bouchon            = bool_bouchon;
  e->node.field.nb_bouchon              = nb_bouchon;
  e->node.field.occurs                  = occurs;
  e->node.field.usage                   = usage;
  e->node.field.field_value             = field_value;
  e->node.field.cond_88                 = cond_88;
  e->node.field.parent                  = parent;
  e->node.field.children                = children;
  e->node.field.redefines               = redefines;
  e->node.field.renames                 = renames;
  e->node.field.search                  = search;
  e->node.field.picture                 = picture;
  e->node.field.field_sign              = field_sign;
  e->node.field.field_sync              = field_sync;
  e->node.field.section                 = section;

  e->next =NULL;

  debug_2("**********  FIELD   **********\n");
  debug_2("* FIELD AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("*******************************\n");

  return e;

}

ast*
update_field_init(struct ast* field, struct ast* name,
                  field_name field_name, field_lvl field_lvl, int lvl){

  field->node.field.name                    = name;
  field->node.field.field_name              = field_name;
  field->node.field.field_lvl               = field_lvl;
  field->node.field.lvl                     = lvl;

  return field;

}

ast*
update_field     (struct ast* field, struct ast* clause, tag_t tag){

  switch(tag){

     case CL_EXTERNAL  :
        field->node.field.bool_external      = 1;
        return field;

     case CL_BLANK_WHEN_ZERO :
        field->node.field.bool_blank_zero    = 1;
        return field;

     case CL_GLOBAL    :
        field->node.field.bool_global        = 1;
        return field;

     case CL_JUSTIFIED :
        field->node.field.bool_justified     = 1;
        return field;

     case CL_OCCURS    :
        field->node.field.occurs             = clause;
        return field;

     case CL_PICTURE   :
        field->node.field.picture            = clause;
        return field;

     case CL_SIGN      :
        field->node.field.field_sign         = clause;
        return field;

     case CL_VALUE     :
        field->node.field.field_value        = clause;
        return field;

     case CL_SYNCHRONIZED :
        field->node.field.field_sync         = clause;
        return field;

     case CL_USAGE        :
        field->node.field.usage              = clause;
        return field;

     default :
        printf("Update_field : Unknown clause (%d)\n",tag);
        exit(EXIT_FAILURE);

  }

}

ast*
make_occurs (int times, int to_times, ast* depend_on, ast* indexes,
                                                      ast* keys ){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.occurs.ast_name               = "OCCURS";
  e->tag = OCCURS;

  e->node.occurs.times                   = times;
  e->node.occurs.to_times                = to_times;
  e->node.occurs.depend_on               = depend_on;
  e->node.occurs.keys                    = keys;
  e->node.occurs.indexes                 = indexes;

  e->next =NULL;

  debug_2("*********** OCCURS   **********\n");
  debug_2("* OCCURS AST BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("********************************\n");

  return e;
}

ast*
make_occurs_key (int bool_asc, int bool_des, ast* key){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.occurs_key.ast_name               = "OCCURS_KEY";
  e->tag = OCCURS_KEY;

  e->node.occurs_key.bool_asc            = bool_asc;
  e->node.occurs_key.bool_des            = bool_des;
  e->node.occurs_key.key                 = key;

  e->next =NULL;

  debug_2("*********** OCCURS_KEY *********\n");
  debug_2("* OCCURS_KEY BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("********************************\n");

  return e;
}

ast*
make_pic_cmpnt(pic_cmpnt_t pic_cmpnt,int repeat, char* chars,
                                                 char* currency){
  ast* e = (ast*) malloc(sizeof(ast));
  e->node.pic_cmpnt.ast_name               = "PIC_CMPNT";
  e->tag = PIC_CMPNT;

  e->node.pic_cmpnt.pic_cmpnt            = pic_cmpnt;

  switch (pic_cmpnt)   {
     case PIC_REPEAT   :
          e->node.pic_cmpnt.pic_val.repeat       = repeat;
          break;
     case PIC_CHARS    :
          strcpy(e->node.pic_cmpnt.pic_val.chars,chars);
          break;
     case PIC_CURRENCY :
          strcpy(e->node.pic_cmpnt.pic_val.currency,currency);
          break;

     default           :
          printf("pic_cmpnt unknown \n");
          exit(EXIT_FAILURE);
  }

  e->next =NULL;

  debug_2("*********** PIC_CMPNT *********\n");
  debug_2("* PIC_CMPNT BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("*******************************\n");

  return e;
}

ast*
make_field_sign(int bool_leading, int bool_trailing, int bool_separate,
                int bool_character){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.field_sign.ast_name               = "FIELD_SIGN";
  e->tag = FIELD_SIGN;

  e->node.field_sign.bool_leading         = bool_leading;
  e->node.field_sign.bool_trailing        = bool_trailing;
  e->node.field_sign.bool_separate        = bool_separate;
  e->node.field_sign.bool_character       = bool_character;

  e->next =NULL;

  debug_2("*********** FIELD_SIGN *********\n");
  debug_2("* FIELD_SIGN BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("*******************************\n");

  return e;
}

ast*
make_field_value(ast* value, ast* thru_value){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.field_value.ast_name            = "FIELD_VALUE";
  e->tag = FIELD_VALUE;

  e->node.field_value.value               = value;
  e->node.field_value.thru_value          = thru_value;

  e->next =NULL;

  debug_2("*********** FIELD_VALUE *********\n");
  debug_2("* FIELD_VALUE BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("*********************************\n");

  return e;
}

ast*
make_usage(usage_t usage){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.usage.ast_name                  = "USAGE";
  e->tag = USAGE;

  e->node.usage.usage                     = usage;

  e->next =NULL;

  debug_2("*********** USAGE *********\n");
  debug_2("* USAGE BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("***************************\n");

  return e;
}

ast*
make_renames(ast* id_qualif,ast* thru_id_qualif){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.renames.ast_name                  = "RENAMES";
  e->tag = RENAMES;

  e->node.renames.id_qualif                 = id_qualif;
  e->node.renames.thru_id_qualif            = thru_id_qualif;

  e->next =NULL;

  debug_2("*********** RENAMES *********\n");
  debug_2("* RENAMES BUILT SUCCESFULLY (%d)*\n",e);
  debug_2("*****************************\n");

  return e;
}

ast*
make_field_sync(int bool_right, int bool_left){

  ast* e = (ast*) malloc(sizeof(ast));
  e->node.field_sync.ast_name            = "FIELD_SYNC";
  e->tag = FIELD_SYNC;

  e->node.field_sync.bool_right          = bool_right;
  e->node.field_sync.bool_left           = bool_left;

  e->next =NULL;

  debug_2("*********** FIELD_SYNC *********\n");
  debug_2("* FIELD_SYNC BUILT SUCCESFULLY *\n");
  debug_2("********************************\n");

  return e;
}

affich_field (ast* tree,scr_line* screen){

   assert(tree->tag==FIELD);
   assert((!tree->next)||tree->next->tag==FIELD);
   static int nb_call=0;
   nb_call++;
   debug_3("repere nb call (%d)\n",nb_call);


   ast*  temp    =NULL;
   debug_3("tree->tag (%s) value (%s) \n",tagValues[tree->tag]
                                         ,tree->node.field.name->
                                          node.ident_name.value);
   char* buffer_1=malloc(125);
   char* buffer_2=malloc(125);
   char* buffer  =malloc(125);


   /* buffer 1 */

   snprintf(tree->node.field.lvl,buffer_1,10); /* lvl */
   strcat(buffer_1," ");

   snprintf(tree->node.field.nb_parents,buffer,10); /* nb_parents */
   strcat(buffer_1,buffer);
   strcat(buffer_1," ");
   strcpy(buffer,"");

 //snprintf(tree->node.field.nb_bouchon,buffer,10); /* nb_bouchon */
 /*strcat(buffer_1,buffer);
   strcat(buffer_1," ");
   strcpy(buffer,"");
   */
   strcat(buffer_1,FLD_NAME(tree));


   if(tree->node.field.occurs       ) {
      strcat(buffer_1," OCCURS ");
      snprintf(tree->node.field.occurs->node.occurs.times,buffer,10);
      strcat(buffer_1,buffer);                        /* times */
      strcpy(buffer,"");
      if(tree->node.field.occurs->node.occurs.to_times>0){
         strcat(buffer_1," TO ");
         snprintf(tree->node.field.occurs->node.occurs.to_times,buffer,10);
         strcat(buffer_1,buffer);                     /* to_times */
         strcpy(buffer,"");
      }
   }


   if(tree->node.field.redefines    ) {
      strcat(buffer_1," REDEF ");
      strcat(buffer_1,tree->node.field.redefines
                          ->node.ident_name.value);
   }


   if(tree->node.field.picture      ) {
      strcat(buffer_1," PIC ");
      temp=tree->node.field.picture;
      while(temp){
         switch (temp->node.pic_cmpnt.pic_cmpnt) {
            case PIC_REPEAT :
                 snprintf(temp->node.pic_cmpnt.pic_val.repeat,buffer,10);
                 strcat(buffer_1,"(");
                 strcat(buffer_1,buffer);
                 strcat(buffer_1,")");
                 break;
            case PIC_CHARS  :
                 strcat(buffer_1,temp->node.pic_cmpnt.pic_val.chars);
                 break;
            case PIC_CURRENCY :
                 strcat(buffer_1,temp->node.pic_cmpnt.pic_val.currency);
                 break;
            default         :
                 printf("pic_cmpnt unknown !\n");
                 exit(EXIT_FAILURE);
         }
         temp=temp->next;
      }

   }


   temp=tree->node.field.field_value;
   if(temp)strcat(buffer_1," VALUE ");

   while(temp){
      strcpy(buffer," ");
      strcat(buffer,
      temp->node.field_value.value->node.literal.value);
      if (temp->node.field_value.thru_value){
          strcat(buffer," THRU ");
          strcat(buffer,
          temp->node.field_value.thru_value->node.literal.value);
      }
      strcat(buffer_1,buffer);
      strcpy(buffer,"");
      temp=temp->next;
   }


   if(tree->node.field.usage        ) {
      strcat(buffer_1," USAGE ");
      strcat(buffer_1,usageValues[tree->node.field.usage
                                      ->node.usage.usage]);
   }


   if(tree->node.field.renames          ) {
      strcat(buffer_1," RENAMES ");
      strcat(buffer_1,
      tree->node.field.renames->node.renames.id_qualif
                              ->node.identifier.name
                              ->node.ident_name.value);
      if (tree->node.field.renames->node.renames.thru_id_qualif){
          strcat(buffer_1," THRU ");
          strcat(buffer_1,
          tree->node.field.renames->node.renames.thru_id_qualif
                                  ->node.identifier.name
                                  ->node.ident_name.value);
      }
   }


   /* buffer 2 */

   if(tree->node.field.bool_blank_zero) {
      strcat(buffer_2," BLANK WHEN ZERO -");
   }
   if(tree->node.field.bool_justified ) {
      strcat(buffer_2," JUSTIFIED -");
   }
   if(tree->node.field.field_sign       ) {
      strcat(buffer_2," SIGN ");
      if(tree->node.field.field_sign->node.field_sign.bool_leading) {
         strcat(buffer_2,"LEAD ");
      }
      if(tree->node.field.field_sign->node.field_sign.bool_trailing) {
         strcat(buffer_2,"TRAIL ");
      }
      if(tree->node.field.field_sign->node.field_sign.bool_separate) {
         strcat(buffer_2,"SEPARATE ");
      }
      if(tree->node.field.field_sign->node.field_sign.bool_character) {
         strcat(buffer_2,"CHAR ");
      }
      strcat(buffer_2,"-");
   }


   if(tree->node.field.field_sync) {
      strcat(buffer_2," SYNC ");
      if(tree->node.field.field_sync->node.field_sync.bool_right){
         strcat(buffer_2,"RIGHT -");
      } else if(tree->node.field.field_sync->node.field_sync.bool_left){
         strcat(buffer_2,"LEFT -");
      } else {
         strcat(buffer_2,"-");
      }
   }
   if(tree->node.field.bool_global) {
      strcat(buffer_2," GLOBAL -");
   }
   if(tree->node.field.bool_external) {
      strcat(buffer_2," EXTERNAL -");
   }


   lvl = tree->node.field.nb_parents;// + tree->node.field.nb_bouchon;
   debug_3("nb_parents  (%d) nb_bouchon(%d) draw lvl  (%d)\n",
   tree->node.field.nb_parents,tree->node.field.nb_bouchon,lvl);

   draw_box(screen,buffer_1,buffer_2,lvl);

   strcpy(buffer_1,"");
   strcpy(buffer_2,"");
   strcpy(buffer,"");
   free(buffer_1);
   free(buffer);


   temp =tree->node.field.children;

   if(temp) {
  /*  lvl++; */
      affich_field(temp,screen);
  /*  lvl--; */
   }

   temp =tree->next;

   if(temp) {
      debug_2("afficher next field : addr (%d) \n",temp);

      affich_field(temp,screen);
   }

}

affich_binary_op(ast* tree,scr_line* screen){


}

affich_data_div(ast* tree,scr_line* screen){

   debug_1("afficher file_sect\n");
   affich_node(tree->node.data_div.file_sect, screen);
   debug_1("afficher ws_sect\n");
   affich_node(tree->node.data_div.ws_sect, screen);
   debug_1("afficher link_sect\n");
   affich_node(tree->node.data_div.link_sect, screen);

}

affich_declaration(ast* tree,scr_line* screen){


}

affich_special_reg(ast* tree,scr_line* screen){


}

int
chain_length(ast* tree){

   ast* p=NULL;
   int  n=0;

   for(p=tree;p;p=p->next){
      n++;
      debug_3("p%d : %d (%d) (%s) \n",n,p,p->tag,
                        p->node.field.name->node.ident_name.value);
   }
      debug_3("Chain  length (%d) \n",n);
   return n;
}

                     /***                 ***/
ast*
append_list (struct ast* list, struct ast* toadd){

  debug_3("\n*******************************************");
  debug_3("\n");
  debug_3("append_list element(%s) to list (%s) start ...\n",
            tagValues[toadd->tag]    ,tagValues[list->tag]);
  debug_3("list (%d) list->next (%d) toadd (%d) toadd->next (%d) \n",
          list,list->next,toadd,toadd->next);
  debug_3("\n*******************************************\n");

  ast* head;

  if(list == NULL){


     debug_3("list (%d) list->next (%d)\n",toadd,toadd->next);

     return toadd;
  }

  head = list;

  while(list->next != NULL){
     debug_3("noeud visit{ non non null passer au suivant ... \n");
     list = list->next;
  }

  list->next=toadd;
  /*list->next=NULL;*/

  debug_3("append succesful            \n");
  debug_3("\n*****************************************\n");
  debug_3("head (%d) head->next (%d) list (%d) list->next (%d) \n",
          head,head->next,list,list->next);
  debug_3("\n*******************************************");
  debug_3("\n");

  return head;

}

void
append_child(struct ast* dad, struct ast* child){

  //ast* head;
  ast* temp=NULL;

  if(FLD_CHILD(dad)) debug_3("dad first child (%s)\n",
  FLD_NAME(FLD_CHILD(dad)));

  if(FLD_CHILD(dad) == NULL){

     debug_3("Dad has no children \n");
     FLD_CHILD(dad) = child;
     return;
  }

  temp = FLD_CHILD(dad);

  while(temp->next){
     debug_3("Hi bro (%s)\n",FLD_NAME(temp->next));
     temp = temp->next;
  }

  debug_3("dad put your child (%s) next bro (%s) \n",
           FLD_NAME(child),FLD_NAME(temp));

  temp->next=child;


}

/*              Affichage des AST                       */
void
affich_node (ast* tree,scr_line* screen){

   if (tree==NULL) return;

   debug_1("  afficher node (%d) \n",tree->tag);

   if      (tree->tag == SENTENCE)

           affich_sentence(tree,screen);

   else if (tree->tag == DECLARATION)

           affich_declaration(tree,screen);

   else if (tree->tag == DATA_DIV   )

           affich_data_div(tree,screen);

   else if (tree->tag == STATEMENT)

           affich_statement(tree,screen);

   else if (tree->tag == MOVE_STM)

           affich_move_stm(tree,screen);

   else if (tree->tag == DISPLAY_STM)

           affich_display_stm(tree,screen);

   else if (tree->tag == INITIALIZE_STM)

           affich_initialize_stm(tree,screen);

   else if (tree->tag == OPERAND)

           affich_operand(tree,screen);

   else if (tree->tag == LITERAL)

           affich_literal(tree,screen);

   else if (tree->tag == IDENTIFIER)

           affich_identifier(tree,screen);

   else if (tree->tag == IDENT_NAME)

           affich_ident_name(tree,screen);

   else if (tree->tag == IDENT_NAME_QUALIF)

           affich_ident_name_qualif(tree,screen);

   else if (tree->tag == IDENT_QUALIF)

           affich_ident_qualif(tree,screen);

   else if (tree->tag == IDENT_SUBSCRIPT)

           affich_ident_subscript(tree,screen);

   else if (tree->tag == IDENT_REFMOD)

           affich_ident_refmod(tree,screen);

   else if (tree->tag == SPECIAL_REG)

           affich_special_reg(tree,screen);

   else if (tree->tag == ARITH_EXP)

           affich_arith_exp(tree,screen);

   else if (tree->tag == BINARY_OP)

           affich_binary_op(tree,screen);

   else if (tree->tag == FIELD)

           affich_field(tree,screen);

   else {
          /* printf("node to print not allowed (%s) \n",
                     tagValues[tree->tag]);
          */ printf("node to print not allowed (%d) (%d)\n",
                               tree,tree->tag );
             exit(EXIT_FAILURE);
   }

}

/*              Free AST               */
void
free_node   (ast* tree){

   /* si ast n'est pas cr{{e */
   if (tree==NULL) return;

   /* Cas sp{cial ast : free children first */
   if ((tree->tag == FIELD) && (tree->node.field.children)) {
      debug_2("Child to free found (%d)\n",tree->node.field.children);
      free_node(tree->node.field.children);
   }

   if(tree->next){
      debug_2("Next to free found (%d)(%d)\n",
                                           tree->next->tag,tree->next);
      free_node(tree->next);
   }

   debug_3("Real free start switch (%d)(%d)\n",tree->tag,tree);

   switch (tree->tag) {

      case SENTENCE          : free_sentence(tree);tree=NULL;break;
      case DECLARATION       : free_declaration(tree);tree=NULL;break;
      case DATA_DIV          : free_data_div(tree);tree=NULL;break;
      case STATEMENT         : free_statement(tree);tree=NULL;break;
      case MOVE_STM          : free_move_stm(tree);tree=NULL;break;
      case DISPLAY_STM       : free_display_stm(tree);tree=NULL;break;
      case INITIALIZE_STM    : free_initialize_stm(tree);               ;
                               tree=NULL;break;
      case OPERAND           : free_operand(tree);tree=NULL;break;
      case LITERAL           : free_literal(tree);tree=NULL;break;
      case IDENTIFIER        : free_identifier(tree);tree=NULL;break;
      case IDENT_NAME        : free_ident_name(tree);tree=NULL;break;
      case IDENT_NAME_QUALIF : free_ident_name_qualif(tree);
                               tree=NULL;break;
      case IDENT_QUALIF      : free_ident_qualif(tree);tree=NULL;break;
      case IDENT_SUBSCRIPT   : free_ident_subscript(tree);              
                               tree=NULL;break;
      case IDENT_REFMOD      : free_ident_refmod(tree);tree=NULL;break;
      case SPECIAL_REG       : free_special_reg(tree);tree=NULL;break;
      case ARITH_EXP         : free_arith_exp(tree);tree=NULL;break;
      case BINARY_OP         : free_binary_op(tree);tree=NULL;break;
      case FIELD             : free_field(tree);tree=NULL;break;
      case OCCURS            : free_occurs(tree);tree=NULL;break;
      case OCCURS_KEY        : free_occurs_key(tree);tree=NULL;break;
      case PIC_CMPNT         : free_pic_cmpnt(tree);tree=NULL;break;
      case FIELD_SIGN        : free_field_sign(tree);tree=NULL;break;
      case FIELD_VALUE       : free_field_value(tree);tree=NULL;break;
      case FIELD_SYNC        : free_field_sync(tree);tree=NULL;break;
      case USAGE             : free_usage(tree);tree=NULL;break;
      case RENAMES           : free_renames(tree);tree=NULL;break;
      default:
        /* printf("Unknown Tag (%d)(%s) \n", tree->tag,
           tagValues[tree->tag]); */
           printf("Unknown Tag (%d)(%d) \n", tree->tag,tree);
           exit(EXIT_FAILURE);
   }

   debug_3("Real free end for (%d)(%d)\n",tree->tag,tree);
}



free_sentence(ast* tree){

   free_node(tree->node.sentence.list_statement);

   free(tree);

}

free_declaration(ast* tree){

   free(tree);

}

free_statement(ast* tree){ /* to_del */

   free(tree);

}

free_move_stm(ast* tree){

   free_node(tree->node.move_stm.operand_g);
   free_node(tree->node.move_stm.list_operand_r);
   free(tree);

}

free_display_stm(ast* tree){

   free_node(tree->node.display_stm.list_operand);
   free(tree);

}

free_initialize_stm(ast* tree){

   free_node(tree->node.initialize_stm.list_operand);
   free(tree);

}

free_operand(ast* tree){

   free_node(tree->node.operand.operand);
   free(tree);
}

free_literal(ast* tree){

   free(tree);
}

free_identifier(ast* tree){

   free_node(tree->node.identifier.name);
   free_node(tree->node.identifier.qualif);
   free_node(tree->node.identifier.subscript);
   free_node(tree->node.identifier.refmod);

   free(tree);

}

free_ident_name(ast* tree){

   free(tree);

}

free_ident_name_qualif(ast* tree){ /* to_del */

   free_node(tree->node.ident_name_qualif.name);
   free_node(tree->node.ident_name_qualif.qualif);

   free(tree);

}

free_ident_qualif(ast* tree){

   free_node(tree->node.ident_qualif.list_qualif);

   free(tree);

}

free_ident_refmod(ast* tree){

   free_node(tree->node.ident_refmod.charleft);
   free_node(tree->node.ident_refmod.length);

   free(tree);

}

free_ident_subscript(ast* tree){

   free_node(tree->node.ident_subscript.list_subscript);

   free(tree);

}

free_special_reg(ast* tree){

   free(tree);

}

free_arith_exp(ast* tree){

   free_node(tree->node.arith_exp.left);
   free_node(tree->node.arith_exp.right);

   free(tree);
}

free_data_div(ast* tree){

   free_node(tree->node.data_div.file_sect);
   free_node(tree->node.data_div.ws_sect);
   free_node(tree->node.data_div.link_sect);

   free(tree);
}

free_field(ast* tree){

   free_node(tree->node.field.name);
   free_node(tree->node.field.copy);
   free_node(tree->node.field.occurs);
   free_node(tree->node.field.usage);
   free_node(tree->node.field.field_value);
   free_node(tree->node.field.cond_88);
 /*free_node(tree->node.field.parent);*/
 /*free(tree->node.field.parent);tree->node.field.parent =NULL;*/
 /*free_node(tree->node.field.children);*/
   free_node(tree->node.field.redefines);
   free_node(tree->node.field.renames);
   free_node(tree->node.field.search);
   free_node(tree->node.field.picture);
   free_node(tree->node.field.field_sign);
   free_node(tree->node.field.field_sync);
   free(tree);

}

free_occurs(ast* tree){

   free_node(tree->node.occurs.depend_on);
   free_node(tree->node.occurs.keys);
   free_node(tree->node.occurs.indexes);

   free(tree);

}

free_occurs_key(ast* tree){

   free_node(tree->node.occurs_key.key);

   free(tree);

}

free_pic_cmpnt(ast* tree){

   free(tree);

}

free_field_sign(ast* tree){

   free(tree);

}

free_field_value(ast* tree){

   free_node(tree->node.field_value.value);
   free_node(tree->node.field_value.thru_value);
   free(tree);

}

free_field_sync(ast* tree){

   free(tree);
}

free_usage(ast* tree){

   free(tree);

}

free_renames(ast* tree){

   free_node(tree->node.renames.id_qualif);
   free_node(tree->node.renames.thru_id_qualif);
   free(tree);

}

free_binary_op(ast* tree){ /* to_del */

   free(tree);

}

/* Maj de nombre de bouchon pour tous les childs */
/* en entr{e : premier child */
void
maj_nb_bouchon(ast* tree){

   assert(tree->tag==FIELD);

   tree->node.field.nb_bouchon++;

   ast* p =NULL;

   for(p=tree->node.field.children;p;p=p->node.field.children){

      maj_nb_bouchon(p);

   }

   for(p=tree->next;p;p=p->next){

      maj_nb_bouchon(p);

   }

}

