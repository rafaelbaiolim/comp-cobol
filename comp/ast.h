#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "helper.h"

#define MAX_LIST_STATEMENT 500
#define MAX_LIST_OPERAND   500

typedef enum { CL_UNKNOWN,
               CL_EXTERNAL, CL_BLANK_WHEN_ZERO, CL_GLOBAL,
               CL_JUSTIFIED, CL_OCCURS,
               CL_PICTURE, CL_SIGN, CL_VALUE, CL_SYNCHRONIZED, CL_USAGE
} tag_t;

typedef enum {
       UNKNOWN_FIELD_NAME,
       FILLER,
       SPACE,
       NAME
} field_name;

typedef enum     {
       UNKNOWN_FIELD_LVL,
       LVL_49,
       LVL_66,
       LVL_77,
       LVL_88,
} field_lvl;

typedef enum     {
       UNKNOWN_PIC_CMPNT,
       PIC_CHARS,
       PIC_REPEAT,
       PIC_CURRENCY,
} pic_cmpnt_t;

typedef enum     {
       UNKNOWN_USAGE,
       BINARY,
       COMPUTATIONAL,
       COMP,
       COMPUTATIONAL_1,
       COMP_1,
       COMPUTATIONAL_2,
       COMP_2,
       COMPUTATIONAL_3,
       COMP_3,
       COMPUTATIONAL_4,
       COMP_4,
       DISPLAY,
       DISPLAY_1,
       INDEX,
       PACKED_DECIMAL,
       POINTER,
       PROCEDURE_POINTER
} usage_t;

typedef enum     {
       UNKNOWN_SECTION,
       FILE_SECTION,
       WORKING_STORAGE_SECTION,
       LINKAGE_SECTION,
} section_t;

struct ast* make_ast         ();
struct ast* make_sentence    (int, struct ast*);
struct ast* make_move        (int, int, struct ast*, struct ast*);
struct ast* make_display     (int, int, char*, int, struct ast*);
struct ast* make_initialize  (int, int, struct ast*);
struct ast* make_operand     (int, int, struct ast*);
struct ast* make_literal     (char*,int, int, int, int, int, int, int,
                              int, int, int, int, int, int);
struct ast* make_identifier  (int, int, int,
                              struct ast*, struct ast*, struct ast*,
                              struct ast*);
struct ast* make_ident_name_qualif
                             (struct ast*, struct ast*);
struct ast* make_ident_name  (int, char*);
#define CB_NAME(x)    x->node.ident_name.value
struct ast* make_ident_qualif(struct ast*);
struct ast* make_ident_subscript
                             (struct ast*);
struct ast* make_ident_refmod(struct ast*, struct ast*);
struct ast* make_arith_exp   (char*, char*, char*,
                              struct ast*, struct ast*);
struct ast* make_data_div    (struct ast*,
                              struct ast*, struct ast*);
struct ast* make_field       (struct ast*, struct ast*,
                              field_name, field_lvl,
                              int,   int,   int,   int, int, int,
                              int,   int,   int,   int, int, int,
                              struct ast*, struct ast*, struct ast*,
                              struct ast*, struct ast*, struct ast*,
                              struct ast*, struct ast*, struct ast*,
                              struct ast*, struct ast*, struct ast*,
                              section_t);

#define FLD_LVL(x)       x->node.field.lvl
#define FLD_NB_PARENT(x) x->node.field.nb_parents
#define FLD_PARENT(x)    x->node.field.parent
#define FLD_NB_BCHON(x)  x->node.field.nb_bouchon
#define FLD_CHILD(x)     x->node.field.children
#define FLD_NAME(x)      CB_NAME(x->node.field.name)

struct ast* update_field_init(struct ast*, struct ast*,
                              field_name, field_lvl, int lvl);

struct ast* update_field     (struct ast*, struct ast*, tag_t);

struct ast* make_occurs      (int, int, struct ast*, struct ast*,
                                        struct ast*);

struct ast* make_occurs_key  (int, int, struct ast*);

struct ast* make_pic_cmpnt   (pic_cmpnt_t, int, char*,char*);

struct ast* make_field_sign  (int, int, int, int);

struct ast* make_field_value (struct ast*, struct ast*);

struct ast* make_field_sync  (int, int);

struct ast* make_usage       (usage_t);

struct ast* make_renames     (struct ast*, struct ast*);

struct ast* append_list      (struct ast* list, struct ast* toadd);
void        append_child     (struct ast* dad , struct ast* child);

int    chain_length          (struct ast* list);


typedef struct ast {

  struct ast* next;

  enum {
/*0*/    UNKNOWN_AST,
/*1*/    SENTENCE,
/*2*/    DECLARATION,
/*3*/    DATA_DIV,
/*4*/    FIELD,
/*5*/    OCCURS,
/*6*/    OCCURS_KEY,
/*7*/    PIC_CMPNT,
/*8*/    FIELD_SIGN,
/*9*/    FIELD_VALUE,
/*10*/   FIELD_SYNC,
/*11*/   USAGE,
/*12*/   RENAMES,
/*13*/   STATEMENT,
/*14*/   MOVE_STM,
/*15*/   DISPLAY_STM,
/*16*/   INITIALIZE_STM,
/*17*/   OPERAND,
/*18*/   LITERAL,
/*19*/   IDENTIFIER,
/*20*/   IDENT_NAME,
/*21*/   IDENT_NAME_QUALIF,
/*22*/   IDENT_QUALIF,
/*23*/   IDENT_SUBSCRIPT,
/*24*/   IDENT_REFMOD,
/*25*/   SPECIAL_REG,
/*26*/   ARITH_EXP,
/*27*/   BINARY_OP
                              } tag;

  union {
          struct { /* sentence    */
                   char*        ast_name;
                   int          nb_statement;
                   struct ast*  list_statement;  /* STATEMENT */
                 } sentence;

          struct { /* move        */
                   char*        ast_name;
                   int          bool_corresp;
                   struct ast*  operand_g;       /* OPERAND   */
                   int          nb_operand_r;
                   struct ast*  list_operand_r;  /* OPERAND   */
                 } move_stm;

          struct { /* display     */
                   char*        ast_name;
                   int          bool_upon;
                   int          bool_no_adv;
                   char         upon_operand[120];
                   int          nb_operand;
                   struct ast*  list_operand;    /* OPERAND   */
                 } display_stm;

          struct { /* initialize  */
                   char*        ast_name;
                   int          bool_repl;
                   int          nb_operand;
                   struct ast*  list_operand;    /* OPERAND   */
                 } initialize_stm;

          struct { /* operand     */
                   char*        ast_name;
                   int          bool_literal;
                   int          bool_identifier;
                   struct ast*  operand;    /* LITERAL / IDENTIFIER */
                 } operand;

          struct { /* literal */
                   char*        ast_name;
                   char         value[120];
                   int          bool_all;
                   int          bool_numeric;
                   int          bool_alphanumeric;
                   int          bool_hexadecimal;
                   int          bool_positive;
                   int          bool_negative;
                   int          bool_unsigned;
                   int          bool_singl_q;
                   int          bool_doubl_q;
                   int          bool_intgr;
                   int          bool_decim;
                   int          bool_figurative;

                   int          char_length;
                 } literal;

          struct { /* identifier  */

                   char*        ast_name;
                   int          bool_length;
                   int          bool_address;
                   int          bool_special_reg;

                   struct ast*  name;         /* IDENT_NAME      */
                   struct ast*  qualif;       /* IDENT_QUALIF    */
                   struct ast*  subscript;    /* IDENT_SUBSCRIPT */
                   struct ast*  refmod;       /* IDENT_REFMOD    */
                 } identifier;

          struct { /* ident_name_qualif */
                   char*        ast_name;
                   struct ast*  name;         /* IDENT_NAME      */
                   struct ast*  qualif;       /* IDENT_QUALIF    */
                 } ident_name_qualif;

          struct { /* ident_name */
                   char*        ast_name;
                   int          length;
                   char         value[120];
                 } ident_name;

          struct { /* ident_qualif */
                   char*        ast_name;
                   struct ast*  list_qualif;  /* IDENT_NAME      */
                 } ident_qualif;

          struct { /* ident_subscript */
                   char*        ast_name;
                   struct ast*  list_subscript; /* ARITH_EXP     */
                 } ident_subscript;

          struct { /* ident_refmod */
                   char*        ast_name;
                   struct ast*  charleft;       /* ARITH_EXP     */
                   struct ast*  length;         /* ARITH_EXP     */
                 } ident_refmod;

          struct { /* arith_exp   */
                   char*        ast_name;
                   char         value[120];
                   char         oper[120];
                   char         sign[120];
                   struct ast*  left;           /* ARITH_EXP     */
                   struct ast*  right;          /* ARITH_EXP     */
                 } arith_exp;

          struct {
                   char*        ast_name;
                   struct ast*  file_sect;
                   struct ast*  ws_sect;
                   struct ast*  link_sect;
                 } data_div;

          struct { /* field       */
                   char*        ast_name; /* FIELD */
                   struct ast*  name;        /* NAME, FILLER OR SPACE */
                   struct ast*  copy;        /* Copy from which field */
                   field_name   field_name; /* filler space */
                   field_lvl    field_lvl;  /* 66 77 88 49  */
                   int          lvl;
                   int          name_length; /* character number */
                   int          size;        /* Bytes number */
                   int          offset;      /* offset from top */
                   int          nb_indexes;  /* parent nb with occurs */
                   int          nb_parents;  /* parent                */
                   int          bool_blank_zero;
                   int          bool_justified;
                   int          bool_global;
                   int          bool_external;
                   int          bool_bouchon;
                   int          nb_bouchon;
                   struct ast*  occurs;      /* occurs dep */
                   struct ast*  usage;       /* usage */
                   struct ast*  field_value; /* value or values 88 */
                   struct ast*  cond_88;     /* condition 88 */
                   struct ast*  parent;      /* upper lvl field */
                   struct ast*  children;    /* top lower lvl field */
                   struct ast*  redefines;   /* redefines           */
                   struct ast*  renames;     /* renames thru        */
                   struct ast*  search;      /* search              */
                   struct ast*  picture;     /* pic_cmpnt           */
                   struct ast*  field_sign;  /* field_sign          */
                   struct ast*  field_sync;  /* field_sign          */
                   section_t    section;     /* section             */
                 } field;

          struct { /* occurs */
                   char*        ast_name;
                   int          times;
                   int          to_times;    /* zero for fixed length*/
                   struct ast*  depend_on;   /* IDENT_NAME           */
                   struct ast*  keys;        /* OCCURS_KEYS          */
                   struct ast*  indexes;     /* IDENT_NAME list      */
                 } occurs;

          struct { /* occurs_key  */
                   char*        ast_name;
                   struct ast*  key;         /* IDENT_NAME           */
                   int          bool_asc;
                   int          bool_des;
                 } occurs_key;

          struct { /* pic_cmpnt   */
                   char*        ast_name;
                   pic_cmpnt_t  pic_cmpnt;   /* IDENT_NAME           */
                   union {
                      int       repeat;
                      char      chars[120];
                      char      currency[120];
                   } pic_val;
                 } pic_cmpnt;

          struct { /* field_sign  */
                   char*        ast_name;
                   int          bool_leading;
                   int          bool_trailing;
                   int          bool_separate;
                   int          bool_character;
                 } field_sign;

          struct { /* FIELD_value */
                   char*        ast_name;
                   struct ast*  value;
                   struct ast*  thru_value;
                 } field_value;

          struct { /* Synchronized */
                   char*        ast_name;
                   int          bool_left;
                   int          bool_right;
                 } field_sync;

          struct { /* usage        */
                   char*        ast_name;
                   usage_t      usage;
                 } usage;

          struct { /* renames      */
                   char*        ast_name;
                   struct ast*  id_qualif;
                   struct ast*  thru_id_qualif;
                 } renames;

      } node;
} ast;


 show_declaration(ast*, scr_line*);
 show_statement(ast*, scr_line*);
 show_move_stm(ast*, scr_line*);
 show__display_stm(ast*, scr_line*);
 show__initialize_stm(ast*, scr_line*);
 show__operand(ast*, scr_line*);
 show__literal(ast*, scr_line*);
 show__identifier(ast*, scr_line*);
 show__ident_name(ast*, scr_line*);
 show__ident_name_qualif(ast*, scr_line*);
 show__ident_qualif(ast*, scr_line*);
 show__ident_subscript(ast*, scr_line*);
 show__ident_refmod(ast*, scr_line*);
 show__special_reg(ast*, scr_line*);
 show__arith_exp(ast*, scr_line*);
 show__binary_op(ast*, scr_line*);
 show__unary_op(ast*, scr_line*);


 void show_node(ast*, scr_line*);
 void free_node(ast* tree);

 free_sentence(ast* tree);
 free_declaration(ast* tree);
 free_statement(ast* tree);
 free_move_stm(ast* tree);
 free_display_stm(ast* tree);
 free_initialize_stm(ast* tree);
 free_operand(ast* tree);
 free_literal(ast* tree);
 free_identifier(ast* tree);
 free_ident_name(ast* tree);
 free_ident_name_qualif(ast* tree);
 free_ident_qualif(ast* tree);
 free_ident_refmod(ast* tree);
 free_ident_subscript(ast* tree);
 free_special_reg(ast* tree);
 free_arith_exp(ast* tree);
 free_data_div(ast* tree);
 free_field(ast* tree);
 free_occurs(ast* tree);
 free_occurs_key(ast* tree);
 free_pic_cmpnt(ast* tree);
 free_field_sign(ast* tree);
 free_field_value(ast* tree);
 free_field_sync(ast* tree);
 free_usage(ast* tree);
 free_renames(ast* tree);
 free_binary_op(ast* tree);

#endif
/* AST_H_INCLUDED */