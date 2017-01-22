#include <stdio.h>
#define __XPG4 // itoa, strccase
#define __UU
#define __OE_8
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#if defined(__IBMC__) || defined(__GNUC__)
#include <strings.h>
#elif defined(_MSC_VER)
#include <string.h>
#define strcasecmp _stricmp
#else
// error : compiler not supported
#endif

#include "debug.h"
#include "grammar.h"
#include "lexer.h"
#include "ast.h"
#include "rexxCom.h"
#include "tokenize.h"
#include "rexxCom.h"


#define MAX_FP_ARR   12
#define COUNT_OF( arr) (sizeof(arr)/sizeof(0[arr]))

context _context={"","","","","",""};
context save;
ast* build_literal(int);
token lookahead = { 0, 0, 0, "", "" };

                     /****************************/

ast*
get_sentence()
{
    ast* ret=NULL;

    if (ret=sntce()){
      ;
    }
    else return NULL;

 /* if(!equal_type("PROGRAM$")){
       printf("unexpected token (%s)\n",get_token_val());
       return NULL;
    } */

    return ret;
}

ast*
get_data()
{
    ast* ret=NULL;
    ast* sub_ret=NULL;

    if(ret=data_fields()){
      ;
    }
    else return NULL;

 /* if(!equal_type("PROGRAM$")){
       printf("unexpected token (%s)\n",get_token_val());
       return NULL;
    } */

    return ret;
}

/*---------------------------------  Sweeper for DATA      ---------*/

int
balayeur_pgm(){

    // commencer le parcours de pgm à partir du début
    //setCurrLnStart();
    // get first token
    consume();

    debug_3("token value (%s) token type (%s) \n",get_token_val()
             ,get_token_type());

    // lire jusqua trouver DATA
    while((!equal_val("DATA")) && (!equal_type("PROGRAM$"))
                               && (!equal_type("ERROR"))){
       consume();
    }

    if(equal_val("DATA")){
       return 1;
    } else {
       return 0;
    }

}

/*---------------------------------  file storage        ---------*/
ast*
data_division(){

    ast* ret=NULL;
    ast* file_ret=NULL;
    ast* ws_ret=NULL;
    ast* link_ret=NULL;


    debug_2("Trying to match rule : %s \n",__FUNCTION__);

    if (match_val("DATA")){
       ;
    }
    else return 0;

    if (match_val("DIVISION")){
       ;
    }
    else return 0;

    if (match_val(".")){
       ;
    }
    else return 0;

    if (file_ret=file_section()){
       ;
    }

    if (ws_ret=working_storage_section()){
       ;
    }

    if (link_ret=linkage_section()){
       ;
    }

    if (!equal_val("PROCEDURE")){
       printf("expected procedure but found (%s)type(%s)l(%d)c(%d)\n",
              get_token_val(),get_token_type(),get_token_line(),
              get_token_col());
       exit(EXIT_FAILURE);
    }

    ret=make_data_div(file_ret,ws_ret,link_ret);

    return ret;

}
/*---------------------------------  file storage        ---------*/
ast*
file_section(){


    ast* ret=NULL;

    debug_2("Trying to match rule : %s \n",__FUNCTION__);

    if(match_val("FILE")){
       ;
    }
    else return 0;

    if(match_val("SECTION")){
       ;
    }
    else return 0;

    if(match_val(".")){
       ;
    }
    else return 0;

    // lire jusqua sortir de la file section
    while( !equal_val("WORKING-STORAGE")
       &&  !equal_val("LINKAGE")
       &&  !equal_val("PROCEDURE")
       &&  !equal_type("PROGRAM$")
       &&  !equal_type("ERROR")){
       consume();
    }

    if (equal_type("PROGRAM$") || equal_type("ERROR")){
       printf("expected working-storage, linkage or procedure \n");
       exit(EXIT_FAILURE);
    }

    ret = make_field(NULL, NULL, UNKNOWN_FIELD_NAME, UNKNOWN_FIELD_LVL,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     NULL, NULL, NULL, NULL, NULL, NULL,
                     NULL, NULL, NULL, NULL, NULL, NULL,
                     UNKNOWN_SECTION);

    debug_2("Rule recognized: %s \n",__FUNCTION__);

    return ret;
}

/*---------------------------------  working storage        ---------*/
ast*
working_storage_section(){

    ast* ret= NULL;

    debug_2("Trying to match rule : %s \n",__FUNCTION__);

    if(match_val("WORKING-STORAGE")){
       ;
    }
    else return ret;

    if(match_val("SECTION")){
       ;
    }
    else return ret;

    if(match_val(".")){
       ;
    }
    else return ret;

    /* 1-49, 66, 77 et 88 */
    while  (equal_attr("LEVEL NUMBER")){

           if(ret=data_fields()){
              ;
           }
           else return ret;
    }

    if (equal_type("PROGRAM$") || equal_type("ERROR")){
       printf("expected linkage or procedure \n");
       exit(EXIT_FAILURE);
    }

    debug_2("Rule recognized: %s \n",__FUNCTION__);

    return ret;
}

/*---------------------------------  linkage                ---------*/
ast*
linkage_section(){


    ast* ret=NULL;
    debug_2("Trying to match rule : %s \n",__FUNCTION__);

    if(match_val("LINKAGE")){
       ;
    }
    else return ret;

    if(match_val("SECTION")){
       ;
    }
    else return ret;

    if(match_val(".")){
       ;
    }
    else return ret;

    /* 1-49, 66, 77 et 88 */
    while  (equal_attr("LEVEL NUMBER")){

           if(ret=data_fields()){
              ;
           }
           else return ret;
    }

    if (equal_type("PROGRAM$") || equal_type("ERROR")){
       printf("expected  procedure \n");
       exit(EXIT_FAILURE);
    }


    debug_2("Rule recognized: %s \n",__FUNCTION__);

    return ret;
}

/*---------------------------------  Sentence and Statement ---------*/

ast*
sntce()
{
    ast* ret=NULL;
    ast* stmnt_ret=NULL;

    int nb_statement =  0;
    ast* list_statement = NULL;

    debug_2("Trying to match rule : sntce \n");

    while((equal_val("MOVE"))         ||
          (equal_val("DISPLAY"))      ||
          (equal_val("INITIALIZE"))){

      if(stmnt_ret=stmnt()){

         nb_statement++;
         list_statement = append_list(list_statement,stmnt_ret);

    /*  (ret->node.sentence.nb_statement)++; */
    /* bug tres bizzare: list_statement->next nest pas null mai egale
       au list_statement, le patch est provisoire eet peux causer
       perte de noeud si append de plusieur noeud a la fois */
    /*  ret->node.sentence.list_statement=append_list
                        (ret->node.sentence.list_statement,stmnt_ret);
    */
      }
      else return NULL;

    }



    /* optional */
    if (equal_val("."))
        consume();

    if (list_statement) {
       ret = make_sentence(nb_statement,list_statement);
       debug_2("Rule recognized: sntce \n");
       return ret;
    }
    else return NULL;

}

ast*
stmnt(){
    debug_2("Trying to match rule : stmnt \n");

    ast* ret=NULL;

    if (equal_val("MOVE")){

       if (ret=move()){
           ;
       }
       else return NULL;

    }
    else if (equal_val("INITIALIZE")){

       if (ret=intlz()){
           ;
       }
       else return NULL;
    }
    else if(equal_val("DISPLAY")){

       if (ret=dsply()){
           ;
       }
       else return NULL;
    }
    else return NULL;


    debug_2("Rule recognized: stmnt \n.");

    return ret;
}

/*--------------------  move, display and initialize Statement ------*/

ast*
move()
{
    debug_2("Trying to match rule : move \n");
/*
    if (move_2()) {
        ;
    }
    else if (move_1()){
        ;
    }
    else return 0;
*/
    ast* ret=NULL;


    if(match_val("MOVE")){
        ;
    }
    else return 0;

    if (ret=move_oprnd()){
        ;
    }
    else return 0;

    debug_2("Rule recognized: move \n");
    return ret;
}

ast*
dsply()
{
    debug_2("Trying to match rule : dsply \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;

/*  BUILD AST : DISPLAY_STM */
    ret = make_display(0,0,"",0,NULL);


    if(match_val("DISPLAY")){
        ;
    }
    else return NULL;

    if(sub_ret=ids_litrs()){

        ret->node.display_stm.list_operand=
        append_list(ret->node.display_stm.list_operand,sub_ret);
        /* attention un append peut ajouter plus qu'un seul operand */
        (ret->node.display_stm.nb_operand)++;
    }
    else return NULL;

    /* optional */
    if (equal_val("UPON")){
       if(dsply_upon()){
           ret->node.display_stm.bool_upon =1;
       }
       else return NULL;
    }

    /* optional */
    if ((equal_val("WITH"))||(equal_val("NO"))){
       if(dsply_noadv()){
           ret->node.display_stm.bool_no_adv =1;
       }
       else return NULL;
    }

    debug_2("Rule recognized: dsply \n");

    return ret;
}

ast*
intlz()
{
    debug_2("Trying to match rule : intlz \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;
    int  bool_rplc=0;


    if(match_val("INITIALIZE")){
        ;
    }
    else return NULL;

    if(sub_ret=ids()){
        ;
    }
    else return NULL;

    /* optional */
    if (equal_val("REPLACING")){

       if(intlz_rplc()){
           bool_rplc =1;
       }
       else return NULL;

    }

    ret = make_initialize(bool_rplc,0,sub_ret);

    debug_2("Rule recognized: intlz \n");

    return ret;
}

/*------------------ initialize body --------------------------------*/

int
intlz_rplc ()
{
    debug_2("Trying to match rule : intlz_rplc \n");

    if(match_val("REPLACING")){
        ;
    }
    else return 0;

    if(intlz_rplc_oprnds()){
        ;
    }
    else return 0;

    debug_2("Rule recognized: intlz_rplc \n");

    return 1;
}

int
intlz_rplc_oprnds()
{
    debug_2("Trying to match rule : intlz_rplc_oprnds \n");

    int ret = 0;

    while((equal_val("ALPHABETIC"))   ||
          (equal_val("ALPHANUMERIC")) ||
          (equal_val("NUMERIC")))    {

       if(intlz_rplc_oprnd()){
           ;
       }
       else return 0;
       ret = 1;
    }

    if (ret == 1)   debug_2("Rule recognized: intlz_rplc_oprnds \n");
    return ret;
}

int
intlz_rplc_oprnd(){

    debug_2("Trying to match rule : intlz_rplc_oprnd \n");

    if(chartype()){
        ;
    }
    else return 0;

    if(match_val("BY")){
        ;
    }
    else return 0;

    if (id_litr()){
        ;
    }
    else return 0;

    debug_2("Rule recognized: intlz_rplc_oprnd \n");
    return 1;
}

int
chartype()
{
    debug_2("Trying to match rule : chartype \n");

    if(match_val("ALPHABETIC")){
        ;
    }
    else if (match_val("ALPHANUMERIC")){
        ;
    }
    else if (match_val("NUMERIC")){
        ;
    }
    else return 0;

    debug_2("Rule recognized: chartype \n");
    return 1;
}

/*-------------------------- move body ------------------------------*/
/*
int move_1(){

    debug_2("Trying to match rule : move_1 \n");

    if(match("MOVE")){
        ;
    }
    else return  0;

    if(id_litr()){
        ;
    }
    else return  0;

    if(match("TO")){
        ;
    }
    else return  0;

    if(ids()){
        ;
    }
    else return  0;

    debug_2("Rule recognized: move_1 \n");
    return 1;

}
*/
/* backtracking ? */
/*
int move_2()
{
    debug_2("Trying to match rule : move_2 \n");

    if(match("MOVE")){
        ;
    }
    else return  0;

    if(corspnd()){
        ;
    }
    else return  0;

    debug_2("Rule recognized: move_2 \n");

    return 1;
}
*/
ast*
move_oprnd(){

    debug_2("Trying to match rule : move_oprnd \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;
    ast* g_oprnd=NULL;
    ast* r_oprnd=NULL;
    int  bool_corresp = 0;


    if((equal_val("CORR")) ||
       (equal_val("CORRESPONDING"))) {
       if(sub_ret=corspnd()){
          bool_corresp = 1;
       }
       else return NULL;

       if(g_oprnd=id()){
          ;
       }
       else return NULL;

       if(match_val("TO")){
          ;
       }
       else return NULL;

       if(r_oprnd=id()){
          ;
       }
       else return NULL;

    }
    else {

       if(g_oprnd=id_litr()){
          ;
       }
       else return NULL;

       if(match_val("TO")){
          ;
       }
       else return NULL;

       if(r_oprnd=ids()){
          ;
       }
       else return NULL;
    }

    debug_3("g_oprnd (%s) r_oprnd (%s) \n",tagValues[g_oprnd->tag],
                                          tagValues[r_oprnd->tag]);
    ret=make_move(bool_corresp,0,g_oprnd,r_oprnd);


    /*
    if(id_litr()){
        if(match("TO")){
            if(ids()){
                ;
            }
            else return  0;
        }
        else return  0;
    }
    else {
        if(corspnd()){
            if(id()){
                if(match("TO")){
                    if(id()){
                        ;
                    }
                    else return  0;
                }
                else return  0;
            }
            else return  0;
        }
        else return  0;
    }
    */
    debug_2("Rule recognized: move_oprnd \n");
    return ret;

}

ast*
corspnd()
{
    debug_2("Trying to match rule : corspnd \n");

    if(match_val("CORRESPONDING")){
        ;
    }
    else if (match_val("CORR")){
        ;
    }
    else return  NULL;

    ast* ret = make_ast();

    debug_2("Rule recognized : corspnd \n");
    return ret;
}

/*--------------------------- display body --------------------------*/

int
dsply_upon()
{
    debug_2("Trying to match rule : dsply_upon \n");

    if(match_val("UPON")){
        ;
    }
    else return  0;

    if(dsply_upon_oprnd()){
        ;
    }
    else return  0;

    debug_2("Rule recognized : dsply_upon \n");

    return 1;
}

int
dsply_noadv()
{
    debug_2("Trying to match rule : dsply_noadv \n");

    /* optional */
    if (match_val("WITH")){
        ;
    }

    if (match_val("NO")){
        ;
    }
    else return  0;

    if (match_val("ADVANCING")){
        ;
    }
    else return  0;

    debug_2("Rule recognized : dsply_noadv \n");

    return 1;
}

int
dsply_upon_oprnd()
{
    debug_2("Trying to match rule : dsply_upon_oprnd \n");

    if(mnemo_or_envir()){
        ;
    }
    else return  0;

    debug_2("Rule recognized : dsply_upon_oprnd \n");
    return 1;
}

int
mnemo_or_envir()
{
    debug_2("Trying to match rule : mnemo_or_envir \n");

    if(id_name()){
        ;
    }
    else return  0;

    debug_2("Rule recognized : mnemo_or_envir \n");
    return 1;
}


/*----------------------  Identifiers and Literals ------------------*/

ast*
ids_litrs()
{
    debug_2("Trying to match rule : ids_litrs \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    while(sub_ret=id_litr()){

        ret=append_list(ret,sub_ret);
        ;
    }

    if (ret)     debug_2("Rule recognized: ids_litrs (%d) \n",ret);

    return ret;
}

ast*
id_litr() {

    debug_2("Trying to match rule : id_litr \n");

    ast* ret=NULL;

    if(equal_type("IDENTIFIER") || equal_attr("SPECIAL REGISTER")){

       if (ret=id()){
           ;
       }
       else return NULL;

    }

    else {

       if (ret=litr()){
           ;
       }
       else return NULL;

    }

    debug_2("Rule recognized: id_litr \n");
    return ret;
}

ast*
ids() {

    debug_2("Trying to match rule : ids \n");

    ast* ret     =NULL;
    ast* sub_ret =NULL;

    while(equal_type("IDENTIFIER") || equal_attr("SPECIAL REGISTER")){
       if(sub_ret=id()){

          ret = append_list(ret,sub_ret);

       }
       else return NULL;
    }

    if (ret)     debug_2("Rule recognized: ids \n");
    return ret;
}

/*-----------------------  Identifier and Literal -------------------*/

ast*
litr() {

    debug_2("Trying to match rule : litr \n");

    ast* ret=NULL;


    /* figurative constants */
    if (equal_attr("FIGURATIVE CONST")){

        if (ret=figurative_constants()){
           ;
        }
        else return NULL;
    }
    else

    /* nonnumeric and numeric literals  */
    if (equal_type("LITERAL")){

    /*  BUILD AST : LITERAL */
        ret=build_literal(0);
        consume();
    }
    else return  NULL;

    debug_2("Rule recognized: litr \n");
/*  printf("ret tag (%s)(%d) \n",tagValues[ret->tag],ret->tag);
*/  return ret;
}

ast*
figurative_constants() {

    ast* ret=NULL;
    ast* figurative_ret=NULL;

    int bool_all=0;

    debug_2("Trying to match rule : figurative_constants \n");

    if(equal_val("ALL")){

       bool_all = 1;
       consume();

    }
       /* QUOTED  est pr{sent ici dans le cas ou ALL est consomm{ */
    if (equal_attr("QUOTED")       ||
        equal_val ("ZERO")         ||
        equal_val ("ZEROS")        ||
        equal_val ("ZEROES")       ||
        equal_val ("SPACE")        ||
        equal_val ("SPACES")       ||
        equal_val ("HIGH-VALUE")   ||
        equal_val ("HIGH-VALUES")  ||
        equal_val ("LOW-VALUE")    ||
        equal_val ("LOW-VALUES")   ||
        equal_val ("QUOTE")        ||
        equal_val ("QUOTES")){

       /*  BUILD AST : LITERAL */
       figurative_ret = build_literal(bool_all);
       ret=append_list(ret,figurative_ret);

       consume();

    }
    else return NULL;

    debug_2("Rule recognized: figurative_constants \n");
    return ret;
}

ast*
id() {

    debug_2("Trying to match rule : id \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;

    ast* name_ret=NULL;
    ast* qualif_ret=NULL;
    ast* subscr_ret=NULL;
    ast* refmod_ret=NULL;

    if (equal_attr("SPECIAL REGISTER")){
       if (sub_ret=special_register()){
          ret=append_list(ret,sub_ret);
       }
    }
    else {

       if (sub_ret=id_name()){
           name_ret=append_list(name_ret,sub_ret);
           ;
       }
       else return NULL;

       /* optional */
       if (equal_val("OF")){
          if (sub_ret=id_qualif()){
              qualif_ret=append_list(qualif_ret,sub_ret);
              ;
          }
          else return  NULL;
       }

       /* optional subscript and refmodifier */
       if (equal_val("(")){

           /* une fois rentr{, ca devient obligatoire */
           if (sub_ret=id_subs_refm()){
            debug_3("id_subs_refm ret->tag (%s) ret->next (%d) \n",
            tagValues[sub_ret->tag],sub_ret->next);

           /* separer les noeuds subs de refm */
              if(sub_ret->tag==IDENT_SUBSCRIPT){
                 subscr_ret = sub_ret;
                 if (sub_ret->next->tag==IDENT_REFMOD){
                    refmod_ret = sub_ret->next;
                    subscr_ret->next = NULL;
                 }
              }
              else {
                 if(sub_ret->tag==IDENT_REFMOD){
                    refmod_ret = sub_ret;
                    if(sub_ret->next) {
                       debug_3("refmod has a next !\n");
                       exit(EXIT_FAILURE);
                    }
                 }
              }
           }
           else return  NULL;
       }



       ret=make_identifier(0,0,0,name_ret,qualif_ret,subscr_ret,
                                                     refmod_ret);
    }

    debug_2("Rule recognized: id \n");
    return ret;
}


ast*
special_register() {

    debug_2("Trying to match rule : special_register \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    int bool_length=0;
    int bool_address=0;

    /* gestion de special register avec OF */
    if ((equal_val("LENGTH")) || (equal_val("ADDRESS"))){

       if(equal_val("LENGTH"))  bool_length  =1;
       if(equal_val("ADDRESS")) bool_address =1;

       consume();

       if(match_val("OF")){
          ;
       }
       else return NULL;

       if (sub_ret=id()){

           ret=append_list(ret,sub_ret);
           ret->node.identifier.bool_length =bool_length;
           ret->node.identifier.bool_address=bool_address;

       }
       else return NULL;

    }
    else

       if(equal_attr("SPECIAL REGISTER")){
          ret=make_identifier(bool_length,bool_address,1,
              make_ident_name(get_token_len(),get_token_val()),
                                              NULL,NULL,NULL);
          consume();
       }
       else return NULL;


    debug_2("Rule recognized: special_register \n");
    return ret;
}

ast*
id_name_qualif()
{
    ast* ret    =NULL;
    ast* sub_ret=NULL;
    ast* name_ret=NULL;
    ast* qualif_ret=NULL;

/*  ret = make_ident_name_qualif(NULL,NULL); */


    if (name_ret=id_name()){
/*      ret->node.ident_name_qualif.name=sub_ret; */
    }
    else return NULL;

    /* optional */
    if (equal_val("OF")){
       if (qualif_ret=id_qualif()){
/*        ret->node.ident_name_qualif.qualif=sub_ret; */
          ;
       }
       else return NULL;
    }

    ret=make_identifier(0,0,0,name_ret,qualif_ret,NULL,NULL);

    return ret;

}

ast*
id_names()
{
    ast* ret    =NULL;
    ast* sub_ret=NULL;

    debug_2("Trying to match rule : id_names \n");

    while(equal_type("IDENTIFIER")){

        if (sub_ret=id_name()){

           ret=append_list(ret,sub_ret);
        }
        else return NULL;

    }

    if(ret) debug_2("Rule recognized: id_names \n");

    return ret;
}

ast*
id_name()
{
    debug_2("Trying to match rule : id_name \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    if (equal_type("IDENTIFIER")){
        ret=make_ident_name(get_token_len(),get_token_val());
        consume();
    }
    else return NULL;

    debug_2("Rule recognized: id_name \n");
    return ret;
}

ast*
id_qualif()
{
    debug_2("Trying to match rule : id_qualif \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;
    ret=make_ident_qualif(NULL);

    while (equal_val("OF")){
       if(sub_ret=id_qualif_oprnd()){
           ret->node.ident_qualif.list_qualif=
               append_list(ret->node.ident_qualif.list_qualif,sub_ret);
       }
       else return NULL;

    }

    if (ret)     debug_2("Rule recognized: id_qualif \n");
    return ret;

}

ast*
id_qualif_oprnd (){

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    debug_2("Trying to match rule : id_qualif_oprnd \n");

    if (match_val("OF")){
        ;
    }
    else return NULL;

    if(sub_ret=id_name()){
        ret=append_list(ret,sub_ret);
    }
    else return NULL;

    debug_2("Rule recognized: id_qualif_oprnd \n");
    return ret;
}
/* continue implementing ast here ... */
ast*
id_subs_refm(){

    ast* ret    =NULL;
    ast* sub_ret=NULL;
    ast* oprnd1 =NULL;

    debug_2("Trying to match rule : id_subs_refm \n");

    if (match_val("(")){
        ;
    }
    else return NULL;
    /* concatener oprnd 1 avec le reste de subs et refm */
    if(oprnd1=id_oprnd_1_subs_refm()){
        ;
    }
    else return NULL;

    if(sub_ret=id_subs_refm_rest()){
        ;
    }
    else return NULL;

    debug_3("expecting IDENT_SUBSCRIPT OR REFMOD (%s)\n",
               tagValues[sub_ret->tag]);
    if(sub_ret->tag==IDENT_SUBSCRIPT){

       ast* head = sub_ret->node.ident_subscript.list_subscript;
       oprnd1->next = head;
       sub_ret->node.ident_subscript.list_subscript = oprnd1;
    }
    else if (sub_ret->tag==IDENT_REFMOD){

       sub_ret->node.ident_refmod.charleft = oprnd1;

    }
    else {

       printf("expected IDENT_SUBSCRIPT OR REFMOD but found (%s)\n",
               tagValues[sub_ret->tag]);
       exit(EXIT_FAILURE);
    }

    ret=sub_ret;

    /* patch pour enlever length vide */

    if (ret->tag==IDENT_REFMOD){
       if(ret->node.ident_refmod.length->node.arith_exp.left == NULL)
         ret->node.ident_refmod.length=NULL;
    }

    debug_3("id_subs_refm ret->tag (%s) ret->next (%d) \n",
            tagValues[ret->tag],ret->next);

    debug_2("Rule recognized : id_subs_refm \n");
    return ret;

}

ast*
id_oprnd_1_subs_refm(){


    ast* ret    =NULL;
    ast* sub_ret=NULL;

    debug_2("Trying to match rule : id_oprnd_1_subs_refm \n");

    if (sub_ret=arith_expr()){
        ret=append_list(ret,sub_ret);
    }
    else return NULL;

    debug_2("Rule recognized : id_oprnd_1_subs_refm \n");
    return ret;

}

ast*
id_subs_refm_rest(){

    debug_2("Trying to match rule : id_subs_refm_rest \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    ast* refmod_ret=NULL;
    ast* subscript_ret=NULL;

    if (equal_val(":")){
        if (sub_ret=id_refm_rest()){
           /* null pour charleft a remplacer par oprnd 1 */
           refmod_ret=make_ident_refmod(NULL,sub_ret);
           ret=append_list(ret,refmod_ret);
           ;
        }
        else return NULL;
    }
    else if (sub_ret=id_subs_rest()){
             /* 1er subscript absent/coquille: remplacer par oprnd 1 */
             subscript_ret=make_ident_subscript(sub_ret);
             ret=append_list(ret,subscript_ret);

             if (equal_val("(")){
                if (sub_ret=id_refmodif()){
                    /* refmod ast est cree par id_refmodif */
                    ret=append_list(ret,sub_ret);
                    ;
                }
                else return  NULL;
             }

    }

    else return  NULL;

    /* ret = soit refmod sans charleft, soit subscripts avec premier
             subscript coquille si seul ou absent si plusieurs
             puis refmod ou non mais complet (car subscript avant) */

    debug_2("Rule recognized : id_subs_refm_rest \n");
    return  ret;

}

ast*
id_refm_rest()
{
    debug_2("Trying to match rule : id_refm_rest \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;


    if (match_val(":")){
        ;
    }
    else return  NULL;
    /* length peut etre une arith_exp vide */
    if(sub_ret=refmodif_length()){
        /* arith_exp */
        ret=append_list(ret,sub_ret);
    }
    else return  NULL;

    if (match_val(")")){
        ;
    }
    else return  NULL;

    debug_2("Rule recognized : id_refm_rest \n");
    return  ret;

}

ast*
id_subs_rest()
{
    debug_2("Trying to match rule : id_subs_rest \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    if (match_val(")")){
        /* creation d'une coquille AST pour le premier subscript */
        ret=make_arith_exp("","","",NULL,NULL);
        ;
    }
    else
    {
        if(sub_ret=subscripts()){

            ret=append_list(ret,sub_ret);

            if (match_val(")")){
                ;
            }
            else return  NULL;
        }
        else return  NULL;
    }

    debug_2("Rule recognized : id_subs_rest \n");
    return  ret;

}

ast*
id_subscript()
{
    debug_2("Trying to match rule : id_subscript \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;


    if (match_val("(")){
        ;
    }
    else return  NULL;

    if(sub_ret=subscripts()){
        ret=append_list(ret,sub_ret);
    }
    else return  NULL;

    if (match_val(")")){
        ;
    }
    else return  NULL;

    debug_2("Rule recognized: id_subscript \n");
    return ret;
}

ast*
id_refmodif()
{
    debug_2("Trying to match rule : id_refmodif \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;


    if (match_val("(")){
        ;
    }
    else return  NULL;

    if(sub_ret=refmodif()){
        ret=append_list(ret,sub_ret);
    }
    else return  NULL;

    if (match_val(")")){
        ;
    }
    else return  NULL;

    debug_2("Rule recognized: id_refmodif \n");
    return ret;
}


/*---------------------------  Identifier body ----------------------*/

ast*
subscripts()
{
    debug_2("Trying to match rule : subscripts \n");


    ast* ret    =NULL;
    ast* sub_ret=NULL;

    while(sub_ret=subscript()){
        ret=append_list(ret,sub_ret);
    }

    if (ret)     debug_2("Rule recognized: subscripts \n");
    return ret;
}

ast*
subscript()
{
    debug_2("Trying to match rule : subscript \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    if (sub_ret=arith_expr2()){
        ret=append_list(ret,sub_ret);
    }
    else return  NULL;

    debug_2("Rule recognized: subscript \n");
    return ret;
}

ast*
refmodif()
{

    ast* ret    =NULL;
    ast* charleftpos_ret=NULL;
    ast* refmodif_length_ret=NULL;

    debug_2("Trying to match rule : refmodif \n");

    if(charleftpos_ret=charleftpos()){
        ;
    }
    else return  NULL;

    if(match_val(":")){
        ;
    }
    else return  NULL;

    /* it's optional || */
    if(refmodif_length_ret=refmodif_length()){
        ;
    }
    else return  NULL;

    ret=make_ident_refmod(charleftpos_ret,refmodif_length_ret);

    debug_2("Rule recognized: refmodif \n");
    return ret;
}

ast*
charleftpos()
{

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    debug_2("Trying to match rule : charleftpos \n");

    if (sub_ret=arith_expr()){
        ret=append_list(ret,sub_ret);
    }
    else return  NULL;

    debug_2("Rule recognized: charleftpos \n");
    return ret;
}

ast*
refmodif_length()
{
    debug_2("Trying to match rule : refmodif_length \n");

    /*optional */

    ast* ret    =NULL;


    if(!equal_val(")")){
       if (ret=arith_expr()){
           ;
       }
       else return NULL;
    }
    else {
       ret=make_arith_exp("","","",NULL,NULL);

    }
    debug_2("Rule recognized: refmodif_length \n");
    return ret;
}


/*-------------------------  Arithmetic expression ------------------*/
/*
int arith_expr_old()
{
    debug_2("Trying to match rule : arith_expr_old \n");

    if(arith_expr_oprnd()){
        ;
    }
    else return  0;

       optional
    if(arith_expr_oprtn()){
        if(arith_expr()){
            ;
        }
        else return  0;
    }

    debug_2("Rule recognized: arith_expr_old \n");
    return 1;
}
*/
ast*
arith_expr()
{
    debug_2("Trying to match rule : arith_expr \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    ast* left   =NULL;
    ast* right  =NULL;
    char oper[3]="";

    if(left=times_div()){
        /* attention aux variables string */
        ret=make_arith_exp("","","",left,NULL);
        ;
    }
    else return  NULL;

    while((equal_val("+")) || (equal_val("-"))){

       strcpy(oper,get_token_val());
       strcpy(ret->node.arith_exp.oper,oper);
       strcpy(oper,"");
       consume();

       if(right=times_div()){
          ret->node.arith_exp.right = right;
       }
       else return NULL;

       ret=make_arith_exp("","","",ret,NULL);
    }

    debug_2("Rule recognized: arith_expr \n");
    return ret;
}

ast*
times_div()
{
    debug_2("Trying to match rule : times_div \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    ast* left   =NULL;
    ast* right  =NULL;
    char oper[3]="";


    if(left=power()){
        /* attention aux variables string */
        ret=make_arith_exp("","","",left,NULL);
        ;
    }
    else return  NULL;

    while((equal_val("*")) || (equal_val("/"))){

       strcpy(oper,get_token_val());
       strcpy(ret->node.arith_exp.oper,oper);
       strcpy(oper,"");
       consume();

       if(right=power()){
          ret->node.arith_exp.right = right;
       }
       else return NULL;

       ret=make_arith_exp("","","",ret,NULL);

    }

    debug_2("Rule recognized: times_div \n");
    return ret;
}

ast*
power()
{
    debug_2("Trying to match rule : power \n");

    ast* ret    =NULL;
    ast* sub_ret=NULL;

    ast* left   =NULL;
    ast* right  =NULL;
    char oper[3]="";
    char sign[3]="";

    if ((equal_val("+"))){

       strcpy(sign,get_token_val());
       consume();
    }
    else if((equal_val("-"))){

       strcpy(sign,get_token_val());
       consume();
    }

    if(left=basis()){
        /* attention aux variables string */
        ret=make_arith_exp("","",sign,left,NULL);
        strcpy(sign,"");
    }
    else return  NULL;

    while((equal_val("**"))){

       strcpy(oper,get_token_val());
       strcpy(ret->node.arith_exp.oper,oper);
       strcpy(oper,"");

       consume();

       if(right=basis()){
          ret->node.arith_exp.right = right;
       }
       else return NULL;

       ret=make_arith_exp("","","",ret,NULL);

    }

    /* ret est unaire (left seulement) */

    debug_2("Rule recognized: power \n");
    return ret;
}

ast*
basis()
{
    debug_2("Trying to match rule : basis \n");


    ast* ret    =NULL;
    ast* sub_ret=NULL;

    if ((equal_type("IDENTIFIER"))||(equal_attr("SPECIAL REGISTER"))){

       if (sub_ret=id()){
          ret=append_list(ret,sub_ret);
       }
       else return NULL;
    }
    else if((equal_val("("))){

            /* bug de caca ! ne pas oublier de consommer le ( */
            consume();

            if (sub_ret=arith_expr()){
               ret=append_list(ret,sub_ret);
            }
            else return NULL;

            if (match_val(")")){
               ;
            }
            else return NULL;
         }
            /* ZERO/ZEROS/ZEROS inclut */
    else if (equal_attr("NUMERIC")){

            if(equal_type("LITERAL")){

               if (sub_ret=litr()){
                  ret=append_list(ret,sub_ret);
               }
               else return NULL;
            }
            else /* KEYWORD (ZERO/ZEROS/ZEROS) */
            {
               sub_ret=build_literal(0);
               consume();
            }

         }

    else return NULL;

    debug_2("Rule recognized: basis \n");
    return ret;
}

ast*
arith_expr2()
{
    debug_2("Trying to match rule : arith_expr2 \n");

    ast*  ret    =NULL;
    ast*  sub_ret=NULL;

    ast*  left   =NULL;
    ast*  right  =NULL;
    char* oper   =NULL;

    if((equal_attr("INTEGER"))&&(equal_attr("UNSIGNED"))){

        left=build_literal(0);
        consume();

    }
    else if(sub_ret=id_name_qualif()){
            /* id_name_qualif retourn d{sormais un identifier */
            left=make_identifier(0,0,0,
                 sub_ret->node.ident_name_qualif.name,
                 sub_ret->node.ident_name_qualif.qualif,NULL,NULL);


            /* optional */
            if((equal_val("+"))||(equal_val("-"))) {

               strcpy(oper,get_token_val());
               strcpy(ret->node.arith_exp.oper,oper);
               strcpy(oper,"");
               consume();

               if((equal_attr("INTEGER"))&&(equal_attr("UNSIGNED"))){

                  right=build_literal(0);
                  consume();

               }
               else return  NULL;
            }

         }
    else return NULL;

    ret = make_arith_exp("",oper,"",left,right);

    debug_2("Rule recognized: arith_expr2 \n");
    return ret;
}
/*
int arith_expr2_old()
{
    debug_2("Trying to match rule : arith_expr2_old \n");

    if(arith_expr_oprnd()){
        ;
    }
    else return  0;

       optional
    if(arith_expr_oprtn()){
        if(arith_expr_oprnd()){
            ;
        }
        else return  0;
    }

    debug_2("Rule recognized: arith_expr2_old \n");
    return 1;
}

int arith_expr_oprnd()
{
    debug_2("Trying to match rule : arith_expr_oprnd \n");

    if (match("INTEGER")){
        ;
    }
    else if (id_name_qualif()){
        ;
    }
    else return  0;

    debug_2("Rule recognized: arith_expr_oprnd \n");
    return 1;
}

int arith_expr_oprtn()
{
    debug_2("Trying to match rule : arith_expr_oprtn \n");

    if (match("+")){
        ;
    }
    else if (match("-")){
        ;
    }
    else if (match("*")){
        ;
    }
    else return  0;

    debug_2("Rule recognized: arith_expr_oprtn \n");
    return 1;
}
*/
/*---------------------- Data declaration    ------------------------*/
ast*
data_fields(){

    debug_2("Trying to match rule : data_fields \n");

    ast* ret=NULL;
    ast* driver=NULL;
    ast* sub_ret=NULL;
    int i;

    static ast* lvl_arr [89];

    while(sub_ret=data_field()) {

       debug_2("lvl_arr(%d)=(%d)AV\n",FLD_LVL(sub_ret)
                                     ,lvl_arr[FLD_LVL(sub_ret)]);

       debug_2("sub_ret(%d)name(%s)lvl(%d)\n",sub_ret,FLD_NAME(sub_ret),
                                                 FLD_LVL(sub_ret));
       if(driver) {
          debug_2("driver(%d)lvl(%d)\n",driver,FLD_LVL(driver));
       }

       if(!ret) {
          if(FLD_LVL(sub_ret)==1 || FLD_LVL(sub_ret)==77){
             ret=append_list(ret,sub_ret);
             driver=ret;
          } else {
             printf("Section must begin with data level 01 or 77\n");
       printf("expected procedure but found (%s)type(%s)l(%d)c(%d)\n",
              get_token_val(),get_token_type(),get_token_line(),
              get_token_col());
             exit(EXIT_FAILURE);
          }
       } else {
          // gerer field lu lvl 77
          if       (FLD_LVL(sub_ret)==77){
             debug_2("(%d-%s) == 77 Lonely field \n",
                      FLD_LVL(sub_ret),FLD_NAME(sub_ret));
             if(FLD_LVL(driver)==77){
                append_list(driver,sub_ret) ; // sister
                // Erasing ...
                for(i=1;i<89;i++) {
                    debug_3(" Erasing lvl arr (%d) \n",i);
                    lvl_arr[i]=NULL;
                }

             } else if (lvl_arr[1]) {
                append_list(lvl_arr[1],sub_ret) ; // sister
                for(i=1;i<89;i++) {
                    debug_3(" Erasing lvl arr (%d) \n",i);
                    lvl_arr[i]=NULL;
                }

             } else {
                printf("Section must begin with data level 01 or 77\n");
       printf("expected procedure but found (%s)type(%s)l(%d)c(%d)\n",
              get_token_val(),get_token_type(),get_token_line(),
              get_token_col());
                exit(EXIT_FAILURE);
             }
             driver=sub_ret;

          // gerer field existant lvl 77
          } else if(FLD_LVL(driver) == 77){
             if(FLD_LVL(sub_ret)==1 || FLD_LVL(sub_ret)==77){
                debug_2("(%d-%s) New start after 77\n",
                         FLD_LVL(sub_ret),FLD_NAME(sub_ret));
                append_list(driver,sub_ret) ; // sister
                driver=sub_ret;
                for(i=1;i<89;i++) {
                    debug_3(" Erasing lvl arr (%d) \n",i);
                    lvl_arr[i]=NULL;
                }
             } else {
                printf("Level 77 must be followed by level 01 or 77\n");
                exit(EXIT_FAILURE);
             }

          } else if(FLD_LVL(driver) == FLD_LVL(sub_ret)){
             FLD_PARENT   (sub_ret) =  FLD_PARENT   (driver);
             FLD_NB_PARENT(sub_ret) =  FLD_NB_PARENT(driver);
             FLD_NB_BCHON (sub_ret) =  FLD_NB_BCHON (driver);
             debug_2("(%d-%s) == (%d-%s) Append to sister\n",
                      FLD_LVL(driver) ,FLD_NAME(driver),
                      FLD_LVL(sub_ret),FLD_NAME(sub_ret));
             append_list(driver,sub_ret) ; // sister
             driver=sub_ret;

          } else if(FLD_LVL(driver) < FLD_LVL(sub_ret)){
             FLD_PARENT   (sub_ret) =  driver;
             FLD_NB_PARENT(sub_ret) =  FLD_NB_PARENT(driver)+1;
             FLD_NB_BCHON (sub_ret) =  FLD_NB_BCHON (driver);
         //  FLD_CHILD    (driver ) =  sub_ret; // children
             debug_2("(%d-%s) <  (%d-%s) Append to child \n",
                      FLD_LVL(driver) ,FLD_NAME(driver),
                      FLD_LVL(sub_ret),FLD_NAME(sub_ret));

             append_child(driver,sub_ret) ;
             driver=sub_ret;

          } else if(FLD_LVL(driver) > FLD_LVL(sub_ret)){

             debug_2("(%d-%s) >  (%d-%s) ",
                      FLD_LVL(driver) ,FLD_NAME(driver),
                      FLD_LVL(sub_ret),FLD_NAME(sub_ret));

             /* parcourir array pour trouver lvl <= lvl du sub_ret */
             for(i=FLD_LVL(sub_ret);i>0;i--) {
                 debug_3("loop lvl array (%d) ",i);
                 if(lvl_arr[i]) {
                    /* nouveau driver est celui trouve */
                    driver=lvl_arr[i];
                    debug_2(" lvl found(%d-%s)",
                    FLD_LVL(driver) ,FLD_NAME(driver));

                    /* si meme lvl non trouve */
                    if(FLD_LVL(sub_ret)>i) {
                       // append to child
                       // ca rend sub_ret frere des child de driver
                       // du coup deux lvl different peuvent etre freres
                       FLD_PARENT (sub_ret) = driver;
                       FLD_NB_PARENT(sub_ret) = FLD_NB_PARENT(driver)+1;
                       FLD_NB_BCHON (sub_ret) = FLD_NB_BCHON (driver);
                       append_child(driver,sub_ret) ;
                       debug_2("append to child \n");

                    } else { /*  i = lvl de sub_ret  */
                       /* append to sister */
                       FLD_PARENT   (sub_ret) = FLD_PARENT   (driver);
                       FLD_NB_PARENT(sub_ret) = FLD_NB_PARENT(driver);
                       FLD_NB_BCHON (sub_ret) = FLD_NB_BCHON (driver);
                       append_list(driver,sub_ret) ;
                       debug_2("append to sister \n");

                    }
                    // un field superior trouve quitter
                    break;
                 } else {
                    debug_3("not found\n");
                 }
             }

             if(i==0) {

                 printf("field parent not found \n");
                 exit(EXIT_FAILURE);
             }

             driver=sub_ret;

             debug_2(" driver lvl > subret lvl \n");
             debug_2(" driver (%d) \n",driver);
             debug_2(" dr-nxt (%d) \n",driver->next);
             debug_2(" subret (%d) \n",sub_ret);
             debug_2(" su-nxt (%d) \n",sub_ret->next);
         //  append_list(driver,sub_ret) ; /* sister */
             debug_2(" driver (%d) \n",driver);
             debug_2(" dr-nxt (%d) \n",driver->next);

             debug_3(" i (%d) \n",i);
             for(++i;i<60;i++) {
                 debug_3(" Erasing lvl arr (%d) \n",i);
                 lvl_arr[i]=NULL;
             }

          }
       }

       debug_3("lvl_arr(%d)= (%d)AP",FLD_LVL(sub_ret),
                lvl_arr[FLD_LVL(sub_ret)]);
       lvl_arr[FLD_LVL(sub_ret)] = sub_ret;
       debug_3("--> (%d)\n ",sub_ret);

      if (ret) debug_3("(%d-%s) next (%d) child (%s) \n",
             FLD_LVL(ret) ,FLD_NAME(ret),
             ret->next, FLD_NAME(FLD_CHILD(ret)));

    }

    if(ret) debug_2("Rule recognized: data_fields \n");


    return ret;

}

ast*
data_field(){

    debug_2("Trying to match rule : data_field \n");

    ast* ret=NULL;
    ast* name_ret=NULL;
    ast* sub_ret=NULL;

    field_name field_name;
    field_lvl  field_lvl;
    int  lvl=0;
    char** ptr=NULL;

    if  (equal_attr("LEVEL NUMBER")&&
        !equal_val ("66")          &&
        !equal_val ("88")          ){

         if (get_token_val()=="77") {
            field_lvl=LVL_77;
         } else {
            field_lvl=LVL_49;
         }
         lvl=strtol(get_token_val(),ptr,10);
         consume();

        if (equal_type("IDENTIFIER")){

           if (name_ret=id_name()){
              ;
           }
           else return NULL;
        }
        else if (equal_val("FILLER")){

                name_ret=make_ident_name(get_token_len(),
                                         get_token_val());
                field_name=FILLER;
                consume();
             }
             else {
                name_ret=make_ident_name(0,"");
                field_name=SPACE;
             }

        if (ret=data_desc()){
           ;
        }
        else return NULL;

        if (match_val(".")){
           ;
        }
        else return NULL;

    }
    else if (equal_attr("LEVEL NUMBER")&&
             equal_val ("66")          ){

             field_lvl =LVL_66;
             lvl=strtol(get_token_val(),ptr,10);
             consume();

        if (name_ret=id_name()){
           ;
        }
        else return NULL;

        if (ret=renames_cl()){
           ;
        }
        else return NULL;

        if (match_val(".")){
           ;
        }
        else return NULL;

    }
    else if (equal_attr("LEVEL NUMBER")&&
             equal_val ("88")          ){

             field_lvl =LVL_88;
             lvl=strtol(get_token_val(),ptr,10);
             consume();

        if (name_ret=cond_name()){
           ;
        }
        else return NULL;

        if (ret=cond_val_cl()){
           ;
        }
        else return NULL;

        if (match_val(".")){
           ;
        }
        else return NULL;

    }
    else return NULL;

    ret=update_field_init(ret,name_ret,field_name,field_lvl,lvl);


    debug_2("Rule recognized: data_field \n");
    return ret;

}
/*---------------------- Declaration  body   ------------------------*/

ast*
cond_name()
{
    debug_2("Trying to match rule : cond_name \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;
    /*
    if (match_type("IDENTIFIER")){
        ;
    }
    else return NULL;
    */

    if(ret=id_name()){

    }
    else return NULL;

    debug_2("Rule recognized: cond_name \n");
    return ret;
}

/*------*/

ast*
data_desc()
{
    debug_2("Trying to match rule : data_desc \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;
    ast* redefines_ret=NULL;


    /* Not all clauses are compatible with each other */
    /* Semantic for further analysis and elimination */


    /* add permutation */
    /* Union for all the clause to permute */
    /* an array of function pointer        */
    /* a while loop                        */
    /* if a clause is found                */
    /* take it off from the array          */

    /* Known bug : normalement global peut venir avant redefines */
    /* Mais pour nous redefines doit obligatoirement etre en premier */

    if (equal_val("REDEFINES")){

       if (redefines_ret=data_redefines_cl()){
          ;
       }
       else return NULL;
    }

    /* Array of clauses to permute      */
    fp_t_nod *clauses;
    clauses = alim_clauses();
    /* print_clauses(clauses); */

    fp_t_nod *iterator= clauses;

    debug_3("Permutation loop start ... \n list head (%d)\n",iterator);

    ret = make_field(NULL, NULL, UNKNOWN_FIELD_NAME, UNKNOWN_FIELD_LVL,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     NULL, NULL, NULL, NULL, NULL, NULL,
                     redefines_ret, NULL, NULL, NULL, NULL, NULL,
                     UNKNOWN_SECTION);
    while (iterator != NULL){

         debug_3("Permutation loop .iterator (%d)\n",iterator);
       /* Known bug : 'IS' is accepted before each clause || */
       /* if should be for external et gloabal only          */
       /* A deleguer a l'analyse semantique                  */
       if (equal_val("IS")){

          consume();

       }

       /* if lookahead match    */
       if (tkn_in_array(iterator->val.cond_1)){
          /* invoquer la function pointer */
          if (sub_ret=iterator->val.fp()){
             debug_2("Function pointer (%d) invoked after cond \n",
                                                iterator->val.id);
             update_field(ret,sub_ret,iterator->val.tag);
          }
          else return NULL;

          /* Reduire la ligne de la liste chaine */
          clauses = delete_clauses(clauses,iterator);
          /* Repointer vers la tete de la chaine */
          iterator = clauses;
       }
       else
       {
          /* pointer vers le noeud suivant */
          iterator = iterator->next;

       }
    }

    /* Not implemented clause : Like */
    /* Not implemented clause : Typedef */

    /* Not implemented clause : Type    */

    debug_2("Rule recognized: data_desc \n");
    return ret;
}

/*--------------------- Data declaration functions ------------------*/

ast*
data_redefines_cl(){

    debug_2("Trying to match rule : data_redefines_cl \n");

    ast* ret=NULL;

    if (match_val("REDEFINES")){
       ;
    }
    else return NULL;

    if (ret=id_name()){
       ;
    }
    else return NULL;

    debug_2("Rule recognized: data_redefines_cl \n");
    return ret;
}

/*------*/

ast*
data_external_cl(){

    debug_2("Trying to match rule : data_external_cl \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;

    if (match_val("EXTERNAL")){
       ret=make_ast();
    }
    else return NULL;


    debug_2("Rule recognized: data_external_cl \n");
    return ret;
}

/*------*/

ast*
data_blankzero_cl(){

    debug_2("Trying to match rule : data_blankzero_cl \n");

    ast* ret=NULL;


    if (match_val("BLANK")){
       ;
    }
    else return NULL;

    if (equal_val("WHEN")){

       consume();

    }

    if (match_val("ZERO")){
       ;
    }
    else return NULL;

    ret=make_ast();

    debug_2("Rule recognized: data_blankzero_cl \n");
    return ret;
}

/*------*/

ast*
data_global_cl(){

    debug_2("Trying to match rule : data_global_cl \n");

    ast* ret=NULL;

    if (match_val("GLOBAL")){
       ;
    }
    else return NULL;

    ret=make_ast();

    debug_2("Rule recognized: data_global_cl \n");
    return ret;
}

/*------*/

ast*
data_just_cl(){

    debug_2("Trying to match rule : data_just_cl \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;


    if ((equal_val("JUST"))||(equal_val("JUSTIFIED"))){

       consume();

    } else return NULL; /* ajout{ au cours de ast */

    if ((equal_val("RIGHT"))){

       consume();

    }

    ret = make_ast();

    debug_2("Rule recognized: data_just_cl \n");
    return ret;
}

/*------*/

ast*
data_occurs_cl(){

    debug_2("Trying to match rule : data_occurs_cl \n");

    ast* ret       =NULL;
    ast* sub_ret   =NULL;

    int  times     =0;
    char** ptr     =NULL;
    int  to_times  =0;
    ast* depend_on =NULL;
    ast* keys      =NULL;
    ast* indexes   =NULL;


    if (match_val("OCCURS")){
       ;
    }
    else return NULL;

    if ((equal_attr("INTEGER"))||(equal_attr("UNSIGNED"))){

       times = strtol(get_token_val(),ptr,10);
       consume();
    }
    else return NULL;

    /* Format 2 */
    if ((equal_val("TO"))){

        consume();

        if ((equal_attr("INTEGER"))||(equal_attr("UNSIGNED"))){

           to_times = strtol(get_token_val(),ptr,10);
           consume();
        }
        else return NULL;

        if ((equal_val("TIMES"))){

           consume();
        }

        if (match_val("DEPENDING")){
           ;
        }
        else return NULL;

        if ((equal_val("ON"))){

           consume();
        }

        if (depend_on=id_name()){
           ;
        }
        else return NULL;

        /* Key-indexed-by phrase */

        if ((equal_val("ASCENDING")) ||
            (equal_val("DESCENDING")) ){

            if (keys=data_index_key_cls()){
               ;
            }
            else return NULL;
        }

        if ((equal_val("INDEX"))){

            if (indexes=data_index_by_cl()){
               ;
            }
            else return NULL;
        }

    }

    /* Format 1 */

    else {

        if ((equal_val("TIMES"))){

           consume();
        }

        /* Key-indexed-by phrase */

        if ((equal_val("ASCENDING")) ||
            (equal_val("DESCENDING")) ){

            if (keys=data_index_key_cls()){
               ;
            }
            else return NULL;
        }

        if ((equal_val("INDEX"))){

            if (indexes=data_index_by_cl()){
               ;
            }
            else return NULL;
        }

    }

    ret=make_occurs(times,to_times,depend_on,keys,indexes);

    debug_2("Rule recognized: data_occurs_cl \n");
    return ret;
}

ast* data_index_key_cls(){

    debug_2("Trying to match rule : data_index_key_cls \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;
    int  bool_asc =0;
    int  bool_des =0;

    while ((equal_val("ASCENDING")) ||
           (equal_val("DESCENDING"))){

        if (equal_val("ASCENDING"))  bool_asc = 1;
        else                         bool_des = 1;

        if (sub_ret=data_index_key_cl()){ /* names */
           ret=append_list(ret,
                           make_occurs_key(bool_asc,bool_des,sub_ret));
           bool_asc =0;
           bool_des =0;
        }
        else return NULL;

    }

    if(ret) debug_2("Rule recognized: data_index_key_cls \n");

    return ret; /* occurs_key */
}

ast*
data_index_key_cl(){

    debug_2("Trying to match rule : data_index_key_cl \n");

    ast* ret=NULL;

    if ((equal_val("ASCENDING")) ||
        (equal_val("DESCENDING")) ){

       consume();

       if ((equal_val("KEY"))){

          consume();
       }

       if ((equal_val("IS"))){

          consume();
       }

       if (ret=id_names()){
          ;
       }
       else return NULL;

    }
    else return NULL;

    debug_2("Rule recognized: data_index_key_cl \n");
    return ret;
}

ast*
data_index_by_cl(){

    debug_2("Trying to match rule : data_index_by_cl \n");

    ast* ret=NULL;

    if (match_val("INDEX")){
       ;
    }
    else return NULL;

    if ((equal_val("BY"))){

       consume();
    }

    if (ret=id_names()){
       ;
    }
    else return NULL;


    debug_2("Rule recognized: data_index_by_cl \n");
    return ret;
}

/*------*/

ast*
data_pic_cl(){

    debug_2("Trying to match rule : data_pic_cl \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;


    if(equal_val("PIC")||equal_val("PICTURE")){
       save = set_context("data_pic_chars");
       consume();
       ;
    }
    else return NULL;

    if (ret=data_pic_str()){
       ;
    }
    else return NULL;

    /* restore_context(save);*/


    if (equal_val("SIZE")){ /* ?? */
       if (sub_ret=data_pic_size_local()){
          ;
       }
       else return NULL;
    }


    /* consommer Space qui vient apres la fin de pic str  (??) */
    if (equal_type("SPACE")){

       consume();
    }


    debug_2("Rule recognized: data_pic_cl \n");
    return ret;
}


ast*
data_pic_str(){

    debug_2("Trying to match rule : data_pic_str \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;

    if (equal_val("IS")){

       consume();
    }

    /* Lexer/Parser Hack */
    /* pour ne pas accepter espace apres repeat : */
    /* PIC 9(9) V                                 */
    /* on consomme le premier espace apres pic et IS */
    /* et puis enlever les espaces de token picchar  */

    /* Space */

    if (equal_type("SPACE")){
       consume();
    }
    else return NULL;

    // bug : on ajoute une boucle de consommation space
    //         08 I-EMTITRE-FININFO REDEFINES I-EMTITRE-NSIGFT   PIC
    //   9(7).
    while (equal_type("SPACE")){
       consume();
    }

    /* Currency */
    if (equal_val("$")){
       sub_ret=make_pic_cmpnt(PIC_CURRENCY,0,"",get_token_val());
       ret=append_list(ret,sub_ret);
       consume();

    }

    while (equal_type("PICCHARS")){

   /*  set_context("data_pic_chars");
   */
       /* PicChars */
       if (sub_ret=data_pic_chars()){
          ret=append_list(ret,sub_ret);
       }
       else return NULL;

   /*  restore_context(save);
   */
       /* Repeat */
       if (equal_val("(")){

          if (sub_ret=data_pic_repeat()){
             ret=append_list(ret,sub_ret);
          }
          else return NULL;

       }
    }


    debug_2("Rule recognized: data_pic_str \n");
    return ret;
}

/****/

ast*
data_pic_chars(){

    debug_2("Trying to match rule : data_pic_chars \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;
    char chars[120];


    if (equal_type("PICCHARS")){
       /* si dernier char de picchar est un point alors */
       /* exiger un point avant la fin de la ligne */
       strcpy(chars,get_token_val());
       consume();
    }
    else
    {
       return NULL;
    }

    ret=make_pic_cmpnt(PIC_CHARS,0,chars,"");
    strcpy(chars,"");

    debug_2("Rule recognized: data_pic_chars \n");

    return ret;
}

ast*
data_pic_repeat(){

    debug_2("Trying to match rule : data_pic_repeat \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;
    int  repeat=0;
    char** ptr=NULL;


    if (match_val("(")){
       ;
    }
    else return NULL;

    if ((equal_attr("UNSIGNED"))&&(equal_attr("INTEGER"))){
       repeat=strtol(get_token_val(),ptr,10);
       consume();
    }
    else return NULL;

    if (equal_val(")")){
       set_context("data_pic_chars");
       consume();
    }
    else return NULL;

    /* picchar hack */
    if (!equal_type("PICCHARS")){
       erase_context();
       if(equal_type("SPACE")){
          consume();
       }
    }

    ret=make_pic_cmpnt(PIC_REPEAT,repeat,"","");

    debug_2("Rule recognized: data_pic_repeat \n");
    return ret;
}


ast*
data_pic_size_local(){

    debug_2("Trying to match rule : data_pic_size_local \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;


    if (match_val("SIZE")){
       ;
    }
    else return NULL;

    if (equal_val("IS")){

       consume();

    }

    if (match_attr("INTEGER")){
       ;
    }
    else return NULL;

    if (match_val("LOCALE")){
       ;
    }
    else return NULL;

    if (equal_val("IS")){

       consume();

       if (mnemo_or_envir()){
          ;
       }
       else return NULL;
    }

    else {
       if (equal_type("IDENTIFIER")){
          if (mnemo_or_envir()){
             ;
          }
          else return NULL;
       }
    }
    /* clause size and local n'est pas exmploit{ */
    ret=make_ast();

    debug_2("Rule recognized: data_pic_size_local \n");
    return ret;
}

/*------*/

ast*
data_sign_cl(){

    debug_2("Trying to match rule : data_sign_cl \n");

    ast* ret=NULL;
    int  bool_leading=0;
    int  bool_trailing=0;
    int  bool_separate=0;
    int  bool_character=0;

    if (equal_val("SIGN")){
       consume();

       if (equal_val("IS")){

          consume();
       }
    }

    if (equal_val("LEADING")){
       bool_leading=1;
       consume();

    }
    else if(equal_val("TRAILING")){
             bool_trailing=1;
             consume();
         }

    else return NULL;

    if (equal_val("SEPARATE")){
       bool_separate=1;
       consume();

       if (equal_val("CHARACTER")){
          bool_character=1;
          consume();
       }
    }

    ret=make_field_sign(bool_leading,bool_trailing,bool_separate,
                                                   bool_character);
    debug_2("Rule recognized: data_sign_cl \n");
    return ret;
}

/*------*/

ast*
data_value_cl(){

    debug_2("Trying to match rule : data_value_cl \n");

    ast* ret=NULL;
    ast* sub_ret=NULL;


    if (match_val("VALUE")){
       ;
    }
    else return NULL;

    if (equal_val("IS")){

       consume();

    }

    if (sub_ret=litr()){
       ;
    }
    else return NULL;

    ret=make_field_value(sub_ret,NULL);

    debug_2("Rule recognized: data_value_cl \n");
    return ret;
}

/*------*/

ast*
data_sync_cl(){

    debug_2("Trying to match rule : data_sync_cl \n");

    ast* ret=NULL;
    int  bool_right=0;
    int  bool_left=0;

    if ((equal_val("SYNC"))) {

       consume();

    }
    else if (equal_val("SYNCHRONIZED")){

       consume();
    }
    else return NULL;

    if ((equal_val("LEFT"))) {
       bool_left=1;
       consume();
    }
    else if (equal_val("RIGHT")) {
       bool_right=1;
       consume();

    }

    ret=make_field_sync(bool_right,bool_left);

    debug_2("Rule recognized: data_sync_cl \n");
    return ret;
}

/*------*/

ast*
data_usage_cl(){

    debug_2("Trying to match rule : data_usage_cl \n");

    ast* ret=NULL;
    usage_t usage;

    if (equal_val("USAGE")){

       consume();

       if (equal_val("IS")){

          consume();

       }

    }

    if (equal_val("BINARY")){
        usage=BINARY;
        consume();


    } else if (equal_val("COMPUTATIONAL")){
        usage=COMPUTATIONAL;
        consume();

    } else if (equal_val("COMP")){
        usage=COMP;
        consume();

    } else if (equal_val("COMPUTATIONAL-1")){
        usage=COMPUTATIONAL_1;
        consume();

    } else if (equal_val("COMP-1")){
        usage=COMP_1;
        consume();

    } else if (equal_val("COMPUTATIONAL-2")){
        usage=COMPUTATIONAL_2;
        consume();

    } else if (equal_val("COMP-2")){
        usage=COMP_2;
        consume();

    } else if (equal_val("COMPUTATIONAL-3")){
        usage=COMPUTATIONAL_3;
        consume();

    } else if (equal_val("COMP-3")){
        usage=COMP_3;
        consume();

    } else if (equal_val("COMPUTATIONAL-4")){
        usage=COMPUTATIONAL_4;
        consume();

    } else if (equal_val("COMP-4")){
        usage=COMP_4;
        consume();

    } else if (equal_val("DISPLAY")){
        usage=DISPLAY;
        consume();

    } else if (equal_val("DISPLAY-1")){
        usage=DISPLAY_1;
        consume();

    } else if (equal_val("INDEX")){
        usage=INDEX;
        consume();

    } else if (equal_val("PACKED-DECIMAL")){
        usage=PACKED_DECIMAL;
        consume();

    } else if (equal_val("POINTER")){
        usage=POINTER;
        consume();

    } else if (equal_val("PROCEDURE-POINTER")){
        usage=PROCEDURE_POINTER;
        consume();

    } else return NULL;

    ret=make_usage(usage);

    debug_2("Rule recognized: data_usage_cl \n");
    return ret;
}
/*---------------------- renames and conditional function -----------*/

ast*
renames_cl()
{
    debug_2("Trying to match rule : renames_cl \n");

    ast* ret=NULL;
    ast* renames_ret=NULL;
    ast* id_qualif_ret=NULL;
    ast* thru_id_qualif_ret=NULL;

    if (match_val("RENAMES")){
        ;
    }
    else return NULL;

    if (id_qualif_ret=id_name_qualif()){
        ;
    }
    else return NULL;

    if ((equal_val("THROUGH")) ||
        (equal_val("THRU")) ){

       consume();

       if (thru_id_qualif_ret=id_name_qualif()){
           ;
       }
       else return NULL;

    }

    renames_ret=make_renames(id_qualif_ret,thru_id_qualif_ret);
    ret=make_field(NULL, NULL, UNKNOWN_FIELD_NAME, UNKNOWN_FIELD_LVL,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   NULL, NULL, NULL, NULL, NULL, NULL,
                   NULL, renames_ret , NULL,NULL, NULL, NULL,
                   UNKNOWN_SECTION);
    debug_2("Rule recognized: renames_cl \n");
    return ret;
}

ast*
cond_val_cl()
{
    debug_2("Trying to match rule : cond_val_cl \n");

    ast* ret=NULL;
    ast* field_value_ret=NULL;
    ast* sub_ret=NULL;
    ast* value_ret=NULL;
    ast* thru_value_ret=NULL;

    if      (match_val("VALUE")){
         if (match_val("IS")){
            ;
         }
    }
    else if (match_val("VALUES")){
         if (match_val("ARE")){
            ;
         }
    }
    else return NULL;

    while (value_ret=litr()){

       if ((equal_val("THROUGH")) ||
           (equal_val("THRU")) ){

           consume();

          if (thru_value_ret=litr()){
              ;
          }
          else return NULL;
       }
       sub_ret=make_field_value(value_ret,thru_value_ret);
   /*  value_ret=NULL;
       thru_value_ret=NULL; */
       field_value_ret=append_list(field_value_ret,sub_ret);
    }

    ret=make_field(NULL, NULL, UNKNOWN_FIELD_NAME, UNKNOWN_FIELD_LVL,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   NULL, NULL, field_value_ret, NULL, NULL, NULL,
                   NULL, NULL, NULL,NULL, NULL, NULL,UNKNOWN_SECTION);


    debug_2("Rule recognized: cond_val_cl \n");
    return ret;
}

//
//   PREPROCESSING OF COPY
//

ast*
copy_replacing(){

    debug_3("Trying to match rule : %s \n",__FUNCTION__);

    ast* ret=NULL;
    ast* copy_ret=NULL;
    ast* lib_ret=NULL;
    int  bool_rplc=0;


    if(match_val("COPY")){
        ;
    }
    else return NULL;

    // text-name
    if(copy_ret=id_name()){
        ;

    // or literal
    } else if(equal_attr("ALPHANUMERIC")){

        copy_ret=litr();

    } else return NULL;

    // optional qualifier
    if (equal_val("OF") || equal_val("IN")){
       consume();

       // text-name
       if(lib_ret=id_name()){
           ;

       // or literal
       } else if(equal_attr("ALPHANUMERIC")){

           lib_ret=litr();

       } else return NULL;

    }

    // optional suppress
    if (equal_val("SUPPRESS")){
       consume();
    }

    /*
    if (equal_val("REPLACING")){

       if(intlz_rplc()){
           bool_rplc =1;
       }
       else return NULL;

    }

    ret = make_initialize(bool_rplc,0,sub_ret);
    */
    debug_2("Rule recognized: %s \n",__FUNCTION__);

    return ret;
}


/*---------------------- Context function    ------------------------*/

int
affich_context(){

   debug_2("context.division : <%s>\n", _context.division);
   debug_2("context.section  : <%s>\n", _context.section );
   debug_2("context.sentence : <%s>\n", _context.sentence);
   debug_2("context.clause   : <%s>\n", _context.clause  );
   debug_2("context.keyword  : <%s>\n", _context.keyword );
   debug_2("context.other    : <%s>\n", _context.other   );

   return 1;
}

context
set_context(char* clause){

    context save;

    if(strcmp("data_pic_chars",clause)==0){

       save = _context;
       _context.division ="DATA";
       _context.section  ="";
       _context.clause   ="data_pic_chars";
       _context.keyword  ="PIC";
       _context.other    ="";

    }

    affich_context();

    return  save;

}

int
restore_context(context save){

    _context = save;

    affich_context();

    return  1;

}

/*---------------------- Functions for permutation -----------------*/
fp_t_nod*
alim_clauses(){

    /*printf("alim_clauses : Start \n"); */

    fp_t clauses_arr[MAX_FP_ARR];
    fp_t_nod* head;

    int i=0;

    fp_t_nod* node;
    node  = malloc (sizeof (fp_t_nod));

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 1;
    clauses_arr[i].tag= CL_EXTERNAL;
    clauses_arr[i].fp = data_external_cl;
    clauses_arr[i].cond_1.arr[0] = "EXTERNAL";
  /*printf("alim_clauses : alimentation of cond_1 \n");
  */clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : Succesful insertion of clause  1 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    head           = node;
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 2;
    clauses_arr[i].tag= CL_BLANK_WHEN_ZERO;
    clauses_arr[i].fp = data_blankzero_cl;
    clauses_arr[i].cond_1.arr[0] = "BLANK";
  /*printf("alim_clauses : alimentation of cond_1 \n");
  */clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : Succesful insertion of clause  2 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 3;
    clauses_arr[i].tag= CL_GLOBAL;
    clauses_arr[i].fp = data_global_cl;
    clauses_arr[i].cond_1.arr[0] = "GLOBAL";
  /*printf("alim_clauses : alimentation of cond_1 \n");
  */clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : Succesful insertion of clause  3 \n");
    /**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 4;
    clauses_arr[i].tag= CL_JUSTIFIED;
    clauses_arr[i].fp = data_just_cl;
    clauses_arr[i].cond_1.arr[0] = "JUST";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[1] = "JUSTIFIED";
    clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : alimentation of cond_1 \n");
    printf("alim_clauses : Succesful insertion of clause  4 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 5;
    clauses_arr[i].tag= CL_OCCURS;
    clauses_arr[i].fp = data_occurs_cl;
    clauses_arr[i].cond_1.arr[0] = "OCCURS";
  /*printf("alim_clauses : alimentation of cond_1 \n");
  */clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : Succesful insertion of clause  5 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;


    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 6;
    clauses_arr[i].tag= CL_PICTURE;
    clauses_arr[i].fp = data_pic_cl;
    clauses_arr[i].cond_1.arr[0] = "PIC";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[1] = "PICTURE";
    clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : alimentation of cond_1 \n");
    printf("alim_clauses : Succesful insertion of clause  6 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 7;
    clauses_arr[i].tag= CL_SIGN;
    clauses_arr[i].fp = data_sign_cl;
    clauses_arr[i].cond_1.arr[0] = "SIGN";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[1] = "LEADING";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[2] = "TRAILING";
    clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : alimentation of cond_1 \n");
    printf("alim_clauses : Succesful insertion of clause  7 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 8;
    clauses_arr[i].tag= CL_VALUE;
    clauses_arr[i].fp = data_value_cl;
    clauses_arr[i].cond_1.arr[0] = "VALUE";
    clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : alimentation of cond_1 \n");
    printf("alim_clauses : Succesful insertion of clause  8 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 9;
    clauses_arr[i].tag= CL_SYNCHRONIZED;
    clauses_arr[i].fp = data_sync_cl;
    clauses_arr[i].cond_1.arr[0] = "SYNC";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[1] = "SYNCHRONIZED";
    clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : alimentation of cond_1 \n");
    printf("alim_clauses : Succesful insertion of clause  9 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = malloc (sizeof (fp_t_nod));
    node           = node->next;
    /**/
    i++;

    clauses_arr[i].cond_1.len =0;
    clauses_arr[i].cond_2.len =0;
    clauses_arr[i].cond_1.arr[0] = "";
    clauses_arr[i].cond_2.arr[0] = "";

    clauses_arr[i].id = 10;
    clauses_arr[i].tag= CL_USAGE;
    clauses_arr[i].fp = data_usage_cl;
    clauses_arr[i].cond_1.arr[0]  = "USAGE";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[1]  = "BINARY";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[2]  = "COMPUTATIONAL";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[3]  = "COMP";;
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[4]  = "COMPUTATIONAL-1";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[5]  = "COMP-1";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[6]  = "COMPUTATIONAL-2";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[7]  = "COMP-2";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[8]  = "COMPUTATIONAL-3";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[9]  = "COMP-3";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[10] = "COMPUTATIONAL-4";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[11] = "COMP-4";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[12] = "DISPLAY";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[13] = "DISPLAY-1";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[14] = "INDEX";;
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[15] = "PACKED-DECIMAL";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[16] = "POINTER";
    clauses_arr[i].cond_1.len++;
    clauses_arr[i].cond_1.arr[17] = "PROCEDURE-POINTER";
    clauses_arr[i].cond_1.len++;
  /*printf("alim_clauses : alimentation of cond_1 \n");
    printf("alim_clauses : Succesful insertion of clause 10 \n");
  *//**/
    node->val      = clauses_arr[i];
    node->next     = NULL;
    /**/
    i++;

    /*--------------------------------------------------*/
    /*                                                  */
    /*--------------------------------------------------*/
    /*
    printf("Start for linked list building from array ...(%d)\n",i);

    fp_t_nod* temp;

    fp_t_nod* node = malloc (sizeof (fp_t_nod));
    node->val      = clauses_arr[0];
    clauses = node;

    temp = clauses->next;

    for(j=1;j<i;j++){

       printf("iteration (%d) \n",j+1);
       if (temp == NULL) {
          temp = malloc (sizeof (fp_t_nod));
          printf("malloc done \n");
       }

       temp->val  = clauses_arr[j];
       temp->next = NULL;

       clause_print(temp->val,"temp->val");


       clause_nod_print(temp,"temp->val");
       temp = temp->next;

       printf("Creation done \n");

    }
    */
    return head;

}
/*
int alim_clause_arr(fp_t_arr clauses){

    printf("alim_clause_arr : Start \n");

    int i=0;
    int j=0;

    if (clause_arr == NULL) {
       clause_arr = malloc(MAX_FUNCTIONS*sizeof(fp_t));
       printf("alim_clause_arr : allocation of MAX_FUNCTIONS size \n");
       for(i=0;i<MAX_FUNCTIONS;i++){
          (clause_arr+i)->cond_1 = malloc(MAX_COND*sizeof(char*));
          (clause_arr+i)->cond_2 = malloc(MAX_COND*sizeof(char*));
       }
    }
    printf("alim_clause_arr : allocation of MAX_COND size \n");
    printf("sizeof(clause_arr) malloc : (%d) \n",sizeof(*clause_arr));

    i = 0;
    j = 0;

    (clause_arr+i)->id = 1;
    (clause_arr+i)->fp = data_external_cl;
    *(((clause_arr+i)->cond_1)+j) = "IS"; j++;
    printf("alim_clause_arr : alimentation of cond_1 \n");
    realloc((clause_arr+i)->cond_1,j*sizeof(char*)); j=0;
    printf("alim_clause_arr : allocation of j size for cond_1 \n");
    *(((clause_arr+i)->cond_2)+j) = "EXTERNAL"; j++;
    printf("alim_clause_arr : alimentation of cond_2 \n");
    realloc((clause_arr+i)->cond_2,j*sizeof(char*)); j=0;
    printf("alim_clause_arr : allocation of j size for cond_2 \n");
    i++;
    printf("alim_clause_arr : Succesful insertion of clause  1 \n");

    (clause_arr+i)->id = 2;
    (clause_arr+i)->fp = data_external_cl;
    *(((clause_arr+i)->cond_1)+j) = "BLANK"; j=0;
    realloc((clause_arr+i)->cond_1,j*sizeof(char*)); j=0;
    realloc((clause_arr+i)->cond_2,j*sizeof(char*)); j=0;
    i++;
    printf("alim_clause_arr : Succesful insertion of clause  2 \n");

    (clause_arr+i)->id = 3;
    (clause_arr+i)->fp = data_global_cl;
    *(((clause_arr+i)->cond_1)+j) = "IS"; j++;
    realloc((clause_arr+i)->cond_1,j*sizeof(char*)); j=0;
    *(((clause_arr+i)->cond_2)+j) = "GLOBAL"; j++;
    realloc((clause_arr+i)->cond_2,j*sizeof(char*)); j=0;
    i++;
    printf("alim_clause_arr : Succesful insertion of clause  3 \n");

    (clause_arr+i)->id = 4;
    (clause_arr+i)->fp = data_just_cl;
    *(((clause_arr+i)->cond_1)+j) = "JUST"; j++;
    *(((clause_arr+i)->cond_1)+j) = "JUSTIFIED"; j++;
    realloc((clause_arr+i)->cond_1,j*sizeof(char*)); j=0;
    *(((clause_arr+i)->cond_2)+j) = "GLOBAL"; j++;
    realloc((clause_arr+i)->cond_2,j*sizeof(char*)); j=0;
    i++;
    printf("alim_clause_arr : Succesful insertion of clause  4 \n");

    (clause_arr+i)->id = 5;
    (clause_arr+i)->fp = data_occurs_cl;
    *(((clause_arr+i)->cond_1)+j) = "OCCURS"; j++;
    realloc((clause_arr+i)->cond_1,j*sizeof(char*)); j=0;
    realloc((clause_arr+i)->cond_2,j*sizeof(char*)); j=0;
    i++;
    printf("alim_clause_arr : Succesful insertion of clause  5 \n");

    fp_t *temp = realloc(clause_arr,i*sizeof(fp_t));

    if (temp == NULL){
       printf("Error allocating memory!\n");
       return 0;
    }
    else {
       clause_arr = temp;
       printf("Succesful reallocation!\n");
    printf("sizeof(clause_arr) realloc : (%d) \n",sizeof(*clause_arr));
    }
    return 1;

}
*/

int
tkn_in_array (str_arr conditions){

   int i;

   debug_3("tkn_in_array: conditions.len = (%d) \n",conditions.len);

   for(i=0;i<conditions.len;i++){

      debug_3("tkn_in_array: conditions.arr:(%s) \n",conditions.arr[i]);
      if (equal_val(conditions.arr[i])) {
         debug_3("tkn_in_array: lookahead found(%s) \n",
                conditions.arr[i]);
         return 1;
      }
   }

   return 0;
}

fp_t_nod*
delete_clauses(fp_t_nod* clauses,fp_t_nod* to_delete){

/* printf("delete_clauses: clauses = (%d) to_delete(%d) \n",
                           clauses ,      to_delete);
*/ fp_t_nod* temp;

   /* list is empty */
   if(clauses == NULL) {
     debug_3("delete_clauses : List is empty no destroying to do. \n");
     return clauses;

   }

   /* node to delete is the head of list */
   if(clauses == to_delete) {
     debug_3("delete_clauses : Beheading the List >:) ... \n");
     temp = clauses->next;
     free(clauses);
     return temp;
   }


   /* when other */
   /* tout pointer vers head of list */
   temp = clauses;
   fp_t_nod* beforeTemp = clauses;

   /* refaire tant qu'on est pas à null (fin liste) */
   while (temp != NULL){

      /* si adresse d'element a supprimer trouver */
      if(temp == to_delete){
         debug_3("delete_clauses : node found, destroying ... \n");
         /* pointer le next du maillon d'avant vers celui d'apres */
         /* c-a-d : skip temp */
         beforeTemp->next = temp->next;
         /* liberer element a supprimer */
         free(temp);
         return clauses;
      }

      beforeTemp = temp;
      temp = temp->next;


   }
}

int
clause_print(fp_t clause,char* txt){

    int i = 0;
    txt ="";
    printf("clause.id           :(%d)\n",clause.id);
    printf("clause.cond_1.len   :(%d)\n",clause.cond_1.len);
    for(i=0;i<clause.cond_1.len;i++)
       printf("clause.cond_1.arr(%d):(%s)\n",i,clause.cond_1.arr[i]);
    printf("clause.cond_2.len   :(%d)\n",clause.cond_2.len);
    for(i=0;i<clause.cond_2.len;i++)
       printf("clause.cond_2.arr(%d):(%s)\n",i,clause.cond_2.arr[i]);

    return 1;
}

int
clause_nod_print(fp_t_nod* clause_nod,char* txt){

    printf("*************************************\n");
    printf("* Current * clause_nod       (%d) ***\n",clause_nod);
    printf("* Next    * clause_nod->next (%d) ***\n",clause_nod->next);
    clause_print(clause_nod->val,txt);
    printf("*************************************\n");

    return 1;
}

int
print_clauses(fp_t_nod* clauses){

    printf("printf_clauses Start...\n\n");

    fp_t_nod *iterator = clauses;

    do {

       clause_nod_print(iterator,"");
       iterator = iterator->next;
    /* assert(iterator == NULL); */

    }
    while (iterator != NULL);

    printf("printf_clauses End.\n");
    return 1;
}

/*---------------------- Build AST from attr ------------------------*/
/* cette fonction peut etre remplacer par un simple make_literal */
/* avec en parametres les equal_attr comme des int */
ast*
build_literal(int bool_all){

    ast* ret=NULL;

    int   bool_numeric      =0;
    int   bool_alphanumeric =0;
    int   bool_hexadecimal  =0;
    int   bool_positive     =0;
    int   bool_negative     =0;
    int   bool_unsigned     =0;
    int   bool_singl_q      =0;
    int   bool_doubl_q      =0;
    int   bool_intgr        =0;
    int   bool_decim        =0;
    int   bool_figurative   =0;
    int   char_length       =0;
    char* value;

    if(equal_attr("NUMERIC"))          bool_numeric      =1;
    if(equal_attr("ALPHANUMERIC"))     bool_alphanumeric =1;
    if(equal_attr("HEX"))              bool_hexadecimal  =1;
    if(equal_attr("POSITIVE"))         bool_positive     =1;
    if(equal_attr("NEGATIVE"))         bool_negative     =1;
    if(equal_attr("UNSIGNED"))         bool_unsigned     =1;
    if(equal_attr("SINGLE QUOTE"))     bool_singl_q      =1;
    if(equal_attr("DOUBLE QUOTE"))     bool_doubl_q      =1;
    if(equal_attr("INTEGER"))          bool_intgr        =1;
    if(equal_attr("DECIMAL"))          bool_decim        =1;
    if(equal_attr("FIGURATIVE CONST")) bool_figurative   =1;

    char_length = get_token_len();
    value       = get_token_val();
    /*value    = "Hard-coded value";*/
    /*strcpy(value,get_token_val());*/

    ret=make_literal(value,bool_all, bool_numeric, bool_alphanumeric,
                     bool_hexadecimal, bool_positive,
                     bool_negative, bool_unsigned,
                     bool_singl_q, bool_doubl_q,
                     bool_intgr, bool_decim,
                     bool_figurative,
                     char_length);

    return ret;
}

/*---------------------- Auxiliary functions ------------------------*/
int
equal_type(char* expected){

    if ((strcasecmp(lookahead.tkn_type,       expected) == 0 )){
        debug_2("equal_type (%s) ?? \n",expected);
        return 1;
    }
    else {
    /*  printf("FALSE \n");
    */  return 0;
    }
}
/*----*/
int
equal_val(char* expected){

    if ((strcasecmp(lookahead.tkn_val,        expected) == 0 )) {
        debug_2("equal_val (%s) ?? TRUE \n",expected);
        return 1;
    }

    else {
    /*  printf("FALSE \n"); */
        return 0;
    }
}
/*----*/
int
equal_attr(char* expected){


    int i;

    for(i=0;i<lookahead.tkn_attr_len;i++){

       if (strcasecmp(lookahead.tkn_attr[i], expected) == 0){
          debug_2("equal_attr (%s) ?? TRUE \n",expected);
          return 1;
       }
    }

    /*printf("FALSE \n"); */
    return 0;
}
/*----*/
int
match_type(char* expected){

    debug_2("match_type : Trying to match type  : <%s> \n", expected);

    if (equal_type(expected)) {
        consume();
        return 1;
    }
    else
        return 0;
}
/*----*/
int
match_val (char* expected){

    debug_2(" Trying to match Value <%s> with <%s>\n",
              expected,get_token_val());

    if (equal_val(expected)) {
        consume();
        return 1;
    }
    else
        return 0;
}
/*----*/
int
match_attr(char* expected){

    debug_2("match_attr : Trying to match Attr  : <%s> \n", expected);

    if (equal_attr(expected)) {
        consume();
        return 1;
    }
    else
        return 0;
}
/*----*/
int
consume(){

    debug_3("consume(%s).\n ",lookahead.tkn_val);
    lookahead = getNextToken();
    debug_3("current(%s). \n",lookahead.tkn_val);

    return 1;
}
/*----*/
int
error(char* msg ){

    printf("error : %s",msg);
    exit(EXIT_FAILURE);

}

/*----*/

int
match(char* terminal){
    printf("match()    : Trying to match token : <%s> \n",terminal);

    /*
    if    ((strcasecmp(lookahead.tkn_val,  "ZERO") == 0 )
        && (strcasecmp(lookahead.tkn_type, "ZERO") == 0 )
        && (strcasecmp(lookahead.tkn_type2,"ZERO") == 0 ) )
    {
        printf("match()    : Lookahead ZERO, Get next token");
        lookahead = getNextToken();

    }  */

    if    ((strcasecmp(lookahead.tkn_val,     terminal) == 0 )
        || (strcasecmp(lookahead.tkn_type,    terminal) == 0 )
        || (strcasecmp(lookahead.tkn_attr[0], terminal) == 0 )
        || (strcasecmp(lookahead.tkn_attr[1], terminal) == 0 ))
    {
        printf("match()    : Token recognized : %s \n",terminal);
    /* mettre lookahead a zero en attendant le chargement de context */
    /*  lookahead.tkn_val    = "ZERO";
        lookahead.tkn_type   = "ZERO";
        lookahead.tkn_type2  = "ZERO"; */

        lookahead = getNextToken();

    }
    else
    {
        printf("match()    : Syntax error expected %s found<%s><%s>\n",
		      terminal, lookahead.tkn_type,lookahead.tkn_val);
        return 0;
    }
    return 1;
}

/*------*/

int
tknEqual(char* expected){
/*  printf("tknEqual() : is Current token equal to:<%s>? \n",expected);
*/ /*
    if    ((strcasecmp(lookahead.tkn_val,  "ZERO") == 0 )
        && (strcasecmp(lookahead.tkn_type, "ZERO") == 0 )
        && (strcasecmp(lookahead.tkn_type2,"ZERO") == 0 ) )
    {
        printf("match()    : Lookahead ZERO, Get next token");
        lookahead = getNextToken();

    }
    */
    if ((strcasecmp(lookahead.tkn_val,        expected) == 0 )
        || (strcasecmp(lookahead.tkn_type,    expected) == 0 )
        || (strcasecmp(lookahead.tkn_attr[0], expected) == 0 )
        || (strcasecmp(lookahead.tkn_attr[1], expected) == 0 ) )
    {
        printf("tknEqual() : Current token == to <%s> \n",expected);
        return 1;

    }
    else
    {
    /*  printf("tknEqual() : Current token != to <%s> \n",expected);*/
        return 0;
    }

}
/*------------------------- Auxiliary functions ---------------------*/

char*
get_token_val(){
    return lookahead.tkn_val;
}

char*
get_token_type(){
    return lookahead.tkn_type;
}
int
get_token_len(){
    return lookahead.tkn_len;
}

int
get_token_line(){
    return lookahead.tkn_lnnum;
}

int
get_token_col(){
    return lookahead.tkn_colnum;
}

