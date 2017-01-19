#ifndef GRAMMAR_H_INCLUDED
#define GRAMMAR_H_INCLUDED

/*
Cobol Cripple Compiler Grammar Version 0.01
Parsing algorithm : Recursive descent parser
Procedures corresponding to grammar non-terminals.

Below the supported subset of Cobol language Syntax in EBNF form.
*/

/*-----------------------  Sentence and Statement -------------------*/
/*
sntce:		stmnt
|	stmnt '.'
;

stmnt:	 	intlz
| 	dsply
|	move
;
*/
/*-------------------------- initialize ------------------------*/
/*
intlz:		INITIALIZE ids intlz.rplc
;

intlz.rplc:
|	REPLACING intlz.rplc.oprnds
;

intlz.rplc.oprnds:	intlz.rplc.oprnds chartype BY id.litr
|	chartype BY id.litr
;

chartype:	ALPHABETIC
|	ALPHANUMERIC
|	NUMERIC
;
*/

/*-------------------------- move ------------------------------*/
/*
move:		move1
|	move2
;

move1:		MOVE id.litr TO ids
;

move2:		MOVE corresp id TO id
;

corresp:	CORR
|	CORRESPONDING
;

Left factoring

move:  MOVE move_oprnd

move_oprnd:   corresp id TO id
| id.litr TO ids
;
*/

/*--------------------------- display --------------------------*/
/*
dsply:		DISPLAY ids.litrs dsply.upon dsply.noadv
;

dsply.upon:
|	UPON dsply.upon.oprnds
;

dsply.upon.oprnds:	mnemo.or.envir
;

mnemo.or.envir: id.name
;

dsply.noadv:
|	WITH NO ADVANCING
|	NO ADVANCING
;
*/
/*------------------------  Identifiers and Literals ----------------*/
/*
ids.litrs:	ids.litrs id.litr
|	id.litr
;

id.litr:	id
|	litr
;

(unused) litrs:		litrs litr |	litr	;

ids:		ids	id
|	id
;

*/

/*------------------------  Identifier and Literal ------------------*/

/*
litr:		LITERAL
|  HEXLITERAL
|  INTEGER
|  DECIMAL
;

id:			id.name id.qualif id.subscript id.refmodif
;

id.name: 	IDENTIFIER
;

id.qualif:
|	id.qualif OF id.name
;

id.subscript:
|	'(' subscripts ')'
;

id.refmodif:
|	'(' refmodif ')'
;
*/
/*-----------------------  Identifier body --------------------------*/

/*

    subscripts:	subscripts subscript
			| 	subscript
	;

    subscript:	INTEGER
			|	id.name id.qualif
			|	id.name id.qualif '-' INTEGER
			|	id.name id.qualif '+' INTEGER
	;
    subscript : arith.expr2;


    refmodif:	charleftpos ':' refmodif.length
	;

	charleftpos: 	| 	INTEGER
					|	id.name id.qualif
					|	arith.expr
	;

	refmodif.length: 	arith.expr
		 	| 	INTEGER
			|	id.name id.qualif
    ;

*/

/*------------------------  Arithmetic expression -------------------*/

/*
	arith.expr:	 arith.expr.oprnd arith.expr.oprtn arith.expr
                | arith.expr.oprnd
	;
	4+(3*(2-(1)))

	arith.expr.oprnd:	INTEGER
				|	id.name id.qualif
	;

	arith.expr.oprtn: '+'
				|	'-'
				|	'*'
	;
    arith.expr2: arith.expr.oprnd arith.expr.oprtn arith.expr.oprnd
                | arith.expr.oprnd
	;

*/

#include "ast.h"
#include "Lexer.h"

#define MAX_STR_ARR  20
/* Declaration pour les fonctions et varaibles de permutations */

/* Typedef pour function to Pointer */
typedef ast* (*AstFunc)();
//typedef ast* (*AstFunc)(void);

/* Typedef pour array of string with length */
typedef struct str_arr{
   int    len;
   char*  arr [MAX_STR_ARR];
} str_arr;

typedef struct fp_t{
   AstFunc fp;
   str_arr cond_1;
   str_arr cond_2;
   int    id;
   tag_t  tag;
} fp_t;

/* Typedef pour array of fp_t with length */
typedef struct fp_t_nod{
   fp_t              val;
   struct fp_t_nod* next;
} fp_t_nod;

fp_t_nod* alim_clauses();
int tkn_in_array (str_arr);
fp_t_nod* delete_clauses(fp_t_nod*,fp_t_nod*);

/*  End declaration pour permutation */

/* Grammar functions return */
typedef struct ast_ret {
          struct ast*  _ast;             /* AST     */
          int          _ret;             /* return  */
} ast_ret;


/* _context Context   */
typedef struct context{
          char*  division;             /* Division                   */
          char*  section;              /* Section                    */
          char*  sentence;             /* Sentence                   */
          char*  clause;               /* Clause                     */
          char*  keyword;              /* Keyword                    */
          char*  other;                /* Other                      */
} context;

extern context _context;
extern char* usageValues[];
extern char* tagValues[];

ast* get_sentence();
ast* get_data();
ast* data_division();
ast* file_section();
ast* working_storage_section();
ast* linkage_section();

/*--------------  Grammar recursive procedures ------------*/


ast* sntce();
ast* stmnt();

ast* move();
ast* dsply();
ast* intlz();

int intlz_rplc ();
int intlz_rplc_oprnds();
int intlz_rplc_oprnd();
int chartype();

/* int move_1(); */
/* int move_2(); */
ast* move_oprnd();
ast* corspnd();


int dsply_upon();
int dsply_noadv();
int dsply_upon_oprnds();
int dsply_upon_oprnd();
int mnemo_or_envir();

ast* ids_litrs();
ast* id_litr();
ast* ids();


ast* litr();
ast* id();
ast* id_name_qualif();
ast* id_name();
ast* id_qualif();
ast* id_subscript();
ast* id_refmodif();

/*--15*07*2014-----------  Additional production  -------------------*/

ast* figurative_constants();
ast* special_register();
ast* id_subs_refm();
ast* id_oprnd_1_subs_refm();
ast* id_subs_refm_rest();
ast* id_refm_rest();
ast* id_subs_rest();
ast* times_div();
ast* power();
ast* basis();

/*-----------------------  Identifier body --------------------------*/


ast* id_qualif_oprnd();
ast* subscripts();
ast* subscript();
ast* refmodif();
ast* charleftpos();
ast* refmodif_length();
ast* arith_expr();
ast* arith_expr2();


/*------------------------  Arithmetic expression -------------------*/



ast* arith_expr_oprnd();
ast* arith_expr_oprtn();

/*--------------------- Data Declaration    -------------------------*/

ast* data_fields();
ast* data_field();
ast* data_desc();
ast* data_redefines_cl();
ast* data_index_key_cls();
ast* data_index_key_cl();
ast* data_index_by_cl();
ast* data_pic_str();
ast* data_pic_chars();
ast* data_pic_repeat();
ast* data_pic_size_local();
ast* renames_cl();
ast* cond_val_cl();
ast* cond_name();

ast* data_external_cl();
ast* data_blankzero_cl();
/*--------------------- Auxiliary functions -------------------------*/

int      match(char*);
char*    get_token_val();
char*    get_token_type();
int      expect(char*);
int      error(char*);
context  set_context(char* );
int      restore_context(context);
char*    get_token_val();
char*    get_token_type();
int      balayeur_pgm();
int	 equal_val(char* expected);
int	 equal_type(char* expected);
int	 match_type(char* expected);
int	 match_val(char* expected);
int	 consume();
int	 get_token_line();
int	 get_token_col();
int	 get_token_len();
int	 match_attr(char* expected);
int	 equal_attr(char* expected);

#endif
/* GRAMMAR_H_INCLUDED */
