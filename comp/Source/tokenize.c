#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "tokenize.h"
//#include "grammar.h"

#define NB_MATCH_MAX    10
#define NB_REGEX_MAX    30

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

context _context={"","","","","",""};
context save;
//ast* build_literal(int);
token lookahead = { 0, 0, 0, "", "" };


  /*----------------------*/
  /* Lexical tokenizing   */ 
  /*----------------------*/
  
  /*---------------------------------------------------------------*/
  /* REGEX Order                                                   */
  /*---------------------------------------------------------------*/
  /* 1  - Keyword                                                  */
  /* 10 - Special registers                                        */
  /* 101- Figurative constants                                     */
  /* 11 - Hexa Literal1 & Hexa Literal2                            */
  /* 2  - Identifier                                               */
  /* 3  - Literal1 & Literal2                                      */
  /* 31 - Decimal                                                  */
  /* 32 - Integer                                                  */
  /* 4  - Period                                                   */
  /*      & plus minus equal expon divid multp Lbrack Rbrack colon */
  /* 5  - EndLine                                                  */
  /* 6  - Error                                                    */
  /*---------------------------------------------------------------*/
  /* Choose the longest match   
  /*---------------------------------------------------------------*/
  /* example : **   => exponent of multiply multiply               */              
  /*           2,2  => decimal instead of integer                  */  
  /*           a-b  => identifier a instead of a - b               */ 
  /*           a+b  => a + b                                       */
  /*---------------------------------------------------------------*/



token tokenizer(char* ln_8_72)
{
    token _ret = {0,0,0,"ERROR","ERROR"};

    static int reduction_len;
    /****/ int regcomp_type_done=0;
    /****/ int regcomp_attr_done=0;

    /* Regex for token type         */
    /****/ regex_t    pregIdent;
    /****/ regex_t    pregPicChars;
    /****/ regex_t    pregLiterNum;
    /****/ regex_t    pregLiterQuot;
    /****/ regex_t    pregKeywoAll;
    /****/ regex_t    pregSymbols;
    /****/ regex_t    pregEndLine;
    /****/ regex_t    pregSpace;

    /* Regex for token attributes   */
    /****/ regex_t    pregSpclReg;
    /****/ regex_t    pregFgrtvConst;
    /****/ regex_t    pregNumeric;
    /****/ regex_t    pregQuoted;
    /****/ regex_t    pregSQuoted;
    /****/ regex_t    pregDQuoted;
    /****/ regex_t    pregHex;
    /****/ regex_t    pregAlphanum;
    /****/ regex_t    pregIntgr;
    /****/ regex_t    pregDecim;
    /****/ regex_t    pregUnsigned;
    /****/ regex_t    pregPositive;
    /****/ regex_t    pregNegative;
    /****/ regex_t    pregLvlnum;
    /****/ regex_t    pregRelOp;
    /****/ regex_t    pregAritOp;

 /* regex_t    pregLiter1;
    regex_t    pregLiter2;
    regex_t    pregPerio;
    regex_t    pregLvl66;
    regex_t    pregLvl88;
    regex_t    pregHexL1;
    regex_t    pregHexL2;
    regex_t    pregLBrack;
    regex_t    pregRBrack;
    regex_t    pregPlus;
    regex_t    pregMinus;
    regex_t    pregExpon;
    regex_t    pregMultp;
    regex_t    pregDivid;
    regex_t    pregEqual;
    regex_t    pregColon;
    regex_t    pregDollar;
    regex_t    pregUintgr; */

  /*    _____________________________________________________
      _(                                                     )_
     =_                I D E N T I F I E R                    _=
       (_____________________________________________________)
                                                                  */

  /*----------------------------*/
  /* Pattern pour indentifier   */
  /*--------------------------------------------------------------*/
  /* - Séparateur début: aucan ou plusieurs espaces ou virgules   */
  /* - Corps : Un mot cobol compos{ de lettres et chiffres        */
  /*   ainsi que tiret et undersoce qui doivent pas apparaitre    */
  /*   au debut ou à a fin. doit contenir au moins une lettre     */
  /* - S{parateur fin  : point, espace, virgule, fin, quote ou "  */
  /*--------------------------------------------------------------*/

    /****/ char *ptrnIdent = "^( !,)*"                                \
   "(([0-9]+[-_]*)*[0-9]*[A-Za-z][A-Za-z0-9]*([-_]+[A-Za-z0-9]+)*)"\
                          "(.*)";

  /*    _____________________________________________________
      _(                                                     )_
     =_            P I C T U R E  C H A R A C T E R           _=
       (_____________________________________________________)
                                                                  */

  /*-------------------------------------------*/
  /* FTA1: ponctuation                         */
  /* FTA2: CR et DB                            */
  /*--------------------------------------------------------------*/
  /* - picture-string =  currency? (picchar+ repeat?)+            */
  /*   currency       =  $                                        */
  /*   picchar        =  [ABEGPSVXZabegpsvxz90\+\-\*\$]           */
  /*   repeat         =  "(" [0-9]+ ")"                           */
  /*   mannque plus que CR et DB pour les pic string              */
  /*--------------------------------------------------------------*/

  /*char     *ptrnPicChars = "(^"                              \
                   "([ABEGPSVXZabegpsvxz90\\+\\-\\*\\$\\/\\,\\.]+))"\
  */          /*   "(( !,)*!\\()(.*)"; */                  \
              /*   "(( !,)+!\\.!\\()(.*)"; */              \
  /*               "(( !,!$)+\\.!\\.!\\()(.*)";
  */
  /*char     *ptrnPicChars = "(^"                              \ */
  /* A*  */ /*     "([ABEGPSVXZabegpsvxz90\\+\\-\\*\\$\\/\\,]*"\ */
  /* (A+ */ /*     "([ABEGPSVXZabegpsvxz90\\+\\-\\*\\$\\/\\,]+"\ */
  /* b)  */ /*     "!\\.)"                                     \ */
  /* A*  */ /*     "[ABEGPSVXZabegpsvxz90\\+\\-\\*\\$\\/\\,]*))"\ */
              /*   "(( !,)*!\\()(.*)"; */
              /*   "(( !,)+[a-zA-Z]+!( !,)*\\.!\\()(.*)"; */
            /*     "(( !,!$)+!\\.!\\()(.*)"; */


    /****/ char *ptrnPicChars = "(^"                              \
  /* A+  */  /*    "([ABEGPSVXZabegpsvxz90\\+\\-\\*\\$\\/\\,]+))" */\
                   "([-+*$/,ABEGPSVXZabegpsvxz90]+))" \
                   "(( !,!$)+!\\.!\\()(.*)";


  /*    _____________________________________________________
      _(                                                     )_
     =_             L I T E R A L  N U M E R I C              _=
       (_____________________________________________________)
                                                                  */

  /*--------------------------------------------------------------*/
  /* - Séparateur début: Zero ou plusieurs : espaces ou virgules  */
  /* - Corps : Commence par le separateur de string quote ou "    */
  /*   puis zero ou une suite de caractères sauf separateur de    */
  /*   string ou un double separateur de string et finit par le   */
  /*   meme separateur de string                                  */
  /* - S{parateur fin  : point, espace, virgule, fin, quote ou "  */
  /*--------------------------------------------------------------*/
    /****/ char   *ptrnLiterNum = "^( !,)*("                      \
                           "([\\+\\-]?[0-9]*,[0-9]+)!"  /* -9,7 */\
                           "([\\+\\-]?[0-9]+)"         /* +987 */\
                      /*    ")([^']!$)(.*)"; */                   \
                           ")(.*)";                               \
  /*    _____________________________________________________
      _(                                                     )_
     =_             L I T E R A L  Q U O T E                  _=
       (_____________________________________________________)
                                                                  */

    /****/ char *ptrnLiterQuot = "^( !,)*("                         \
                           "('([^']!'')*')!"            /*  '.' */ \
                           "(\"([^\"]!\"\")*\")!"       /*  "." */ \
                           "([Xx]['][0-9A-Fa-f]+['])!"  /* x'.' */ \
                           "([Xx][\"][0-9A-Fa-f]+[\"])" /* x"." */ \
                      /*    ")([^']!$)(.*)"; */                    \
                           ")(.*)";                                \
  /*    _____________________________________________________
      _(                                                     )_
     =_                   K E Y W O R D                       _=
       (_____________________________________________________)
                                                                  */

    /****/ char *ptrnKeywoAll = "^( !,)*("                          \
    /*   A   */                                                    \
    "ACCEPT!ALPHABETIC-LOWER!APPLY!"                               \
    "ACCESS!ALPHABETIC-UPPER!ARE!"                                 \
    "ADD!ALPHANUMERIC!AREA!"                                       \
    "ALPHANUMERIC-EDITED!AREAS!ADDRESS!"                           \
    "ADVANCING!ALSO!ASCENDING!"                                    \
    "AFTER!ALTER!ASSIGN!"                                          \
    "ALTERNATE!AT!ALL!"                                            \
    "ALPHABET!AND!AUTHOR!"                                         \
    "ALPHABETIC!ANY!"                                              \
    /*   B   */                                                    \
    "BASIS!BINARY!BOTTOM!"                                         \
    "BEFORE!BLANK!BY!"                                             \
    "BEGINNING!BLOCK!"                                             \
    /*   C   */                                                    \
    "CALL!COLUMN!COMPUTATIONAL-5!"                                 \
    "CANCEL!COM-REG!COMPUTE!"                                      \
    "CBL!COMMA!CONFIGURATION!"                                     \
    "CD!COMMON!CONTAINS!"                                          \
    "CF!COMMUNICATION!CONTENT!"                                    \
    "CH!COMP!CONTINUE!"                                            \
    "CHARACTER!COMP-1!CONTROL!"                                    \
    "CHARACTERS!COMP-2!CONTROLS!"                                  \
    "CLASS!COMP-3!CONVERTING!"                                     \
    "CLASS-ID!COMP-4!COPY!"                                        \
    "CLOCK-UNITS!COMP-5!CORR!"                                     \
    "CLOSE!COMPUTATIONAL!CORRESPONDING!"                           \
    "COBOL!COMPUTATIONAL-1!COUNT!"                                 \
    "CODE!COMPUTATIONAL-2!CURRENCY!"                               \
    "CODE-SET!COMPUTATIONAL-3!"                                    \
    "COLLATING!COMPUTATIONAL-4!"                                   \
    /*   D   */                                                    \
    "DATA!DEBUG-SUB-1!DESTINATION!"                                \
    "DATE-COMPILED!DEBUG-SUB-2!DETAIL!"                            \
    "DATE-WRITTEN!DEBUG-SUB-3!DISPLAY!"                            \
    "DAY!DEBUGGING!DISPLAY-1!"                                     \
    "DAY-OF-WEEK!DECIMAL-POINT!DIVIDE!"                            \
    "DBCS!DECLARATIVES!DIVISION!"                                  \
    "DE!DELETE!DOWN!"                                              \
    "DEBUG-CONTENTS!DELIMITED!DUPLICATES!"                         \
    "DELIMITER!DYNAMIC!DEBUG-ITEM!"                                \
    "DEPENDING!DEBUG-LINE!"                                        \
    "DESCENDING!DEBUG-NAME!"                                       \
    /*   E   */                                                    \
    "EGCS!END-INVOKE!ENDING!"                                      \
    "EGI!END-MULTIPLY!ENTER!"                                      \
    "EJECT!END-OF-PAGE!ENTRY!"                                     \
    "ELSE!END-PERFORM!ENVIRONMENT!"                                \
    "EMI!END-READ!EOP!"                                            \
    "ENABLE!END-RECEIVE!EQUAL!"                                    \
    "END!END-RETURN!ERROR!"                                        \
    "END-ADD!END-REWRITE!ESI!"                                     \
    "END-CALL!END-SEARCH!EVALUATE!"                                \
    "END-COMPUTE!END-START!EVERY!"                                 \
    "END-DELETE!END-STRING!EXCEPTION!"                             \
    "END-DIVIDE!END-SUBTRACT!EXIT!"                                \
    "END-EVALUATE!END-UNSTRING!EXTEND!"                            \
    "END-IF!END-WRITE!EXTERNAL!"                                   \
    /*   F   */                                                    \
    "FALSE!FILLER!FOR!"                                            \
    "FD!FINAL!FROM!"                                               \
    "FILE!FIRST!FUNCTION!"                                         \
    "FILE-CONTROL!FOOTING!"                                        \
    /*   G   */                                                    \
                                                                   \
    "GENERATE!GO!GROUP!"                                           \
    "GIVING!GOBACK!"                                               \
    "GLOBAL!GREATER!"                                              \
    /*   H   */                                                    \
                                                                   \
    "HEADING!HIGH-VALUE!HIGH-VALUES!"                              \
    /*   I   */                                                    \
    "I-O!INDICATE!INSPECT!"                                        \
    "I-O-CONTROL!INHERITS!INSTALLATION!"                           \
    "ID!INITIAL!INTO!"                                             \
    "IDENTIFICATION!INITIALIZE!INVALID!"                           \
    "IF!INITIATE!INVOKE!"                                          \
    "IN!INPUT!IS!"                                                 \
    "INDEX!INPUT-OUTPUT!"                                          \
    "INDEXED!INSERT!"                                              \
    /*   J   */                                                    \
    "JUST!JUSTIFIED!"                                              \
    /*   K   */                                                    \
    "KANJI!KEY!"                                                   \
    /*   L   */                                                    \
    "LABEL!LIMIT!LINES!"                                           \
    "LAST!LIMITS!LINKAGE!"                                         \
    "LEADING!LINAGE!LOCAL-STORAGE!"                                \
    "LEFT!LINAGE-COUNTER!LOCK!"                                    \
    "LINE!LENGTH!LOW-VALUE!"                                       \
    "LESS!LINE-COUNTER!LOW-VALUES!"                                \
    /*   M   */                                                    \
    "MEMORY!METHOD!MORE-LABELS!"                                   \
    "MERGE!METHOD-ID!MOVE!"                                        \
    "MESSAGE!MODE!MULTIPLE!"                                       \
    "METACLASS!MODULES!MULTIPLY!"                                  \
    /*   N   */                                                    \
    "NATIVE!NO!NUMBER!"                                            \
    "NATIVE_BINARY!NOT!NUMERIC!"                                   \
    "NEGATIVE!NULL!NUMERIC-EDITED!"                                \
    "NEXT!NULLS!"                                                  \
    /*   O   */                                                    \
    "OBJECT!ON!OTHER!"                                             \
    "OBJECT-COMPUTER!OPEN!OUTPUT!"                                 \
    "OCCURS!OPTIONAL!OVERFLOW!"                                    \
    "OF!OR!OVERRIDE!"                                              \
    "OFF!ORDER!"                                                   \
    "OMITTED!ORGANIZATION!"                                        \
    /*   P   */                                                    \
    "PACKED-DECIMAL!PIC!PROCEDURE-POINTER!"                        \
    "PADDING!PICTURE!PROCEDURES!"                                  \
    "PAGE!PLUS!PROCEED!"                                           \
    "PAGE-COUNTER!POINTER!PROCESSING!"                             \
    "PASSWORD!POSITION!PROGRAM!"                                   \
    "PERFORM!POSITIVE!PROGRAM-ID!"                                 \
    "PF!PRINTING!PURGE!"                                           \
    "PH!PROCEDURE!"                                                \
    /*   Q   */                                                    \
    "QUEUE!QUOTE!QUOTES!"                                          \
    /*   R   */                                                    \
    "RANDOM!RELATIVE!RESERVE!"                                     \
    "RD!RELEASE!RESET!"                                            \
    "READ!RELOAD!RETURN!"                                          \
    "READY!REMAINDER!RETURN-CODE!"                                 \
    "RECEIVE!REMOVAL!RETURNING!"                                   \
    "RECORD!RENAMES!REVERSED!"                                     \
    "RECORDING!REPLACE!REWIND!"                                    \
    "RECORDS!REPLACING!REWRITE!"                                   \
    "RECURSIVE!REPORT!RF!"                                         \
    "REDEFINES!REPORTING!RH!"                                      \
    "REEL!REPORTS!RIGHT!"                                          \
    "REFERENCE!REPOSITORY!ROUNDED!"                                \
    "REFERENCES!RERUN!RUN!"                                        \
    /*   S   */                                                    \
    "SAME!SIGN!STANDARD!"                                          \
    "SD!SIZE!STANDARD-1!"                                          \
    "SEARCH!SKIP1!STANDARD-2!"                                     \
    "SECTION!SKIP2!START!"                                         \
    "SECURITY!SKIP3!STATUS!"                                       \
    "SEGMENT!SORT!STOP!"                                           \
    "SEGMENT-LIMIT!SORT-CONTROL!STRING!"                           \
    "SELECT!SORT-CORE-SIZE!SUB-QUEUE-1!"                           \
    "SELF!SORT-FILE-SIZE!SUB-QUEUE-2!"                             \
    "SEND!SORT-MERGE!SUB-QUEUE-3!"                                 \
    "SENTENCE!SORT-MESSAGE!SUBTRACT!"                              \
    "SEPARATE!SORT-MODE-SIZE!SUM!"                                 \
    "SEQUENCE!SUPER!SORT-RETURN!"                                  \
    "SEQUENTIAL!SOURCE!SUPPRESS!"                                  \
    "SERVICE!SOURCE-COMPUTER!SYMBOLIC!"                            \
    "SET!SYNC!SPACE!"                                              \
    "SHIFT-IN!SYNCHRONIZED!SPACES!"                                \
    "SHIFT-OUT!SPECIAL-NAMES!"                                     \
    /*   T   */                                                    \
    "TABLE!TEXT!TITLE!"                                            \
    "THAN!TO!TALLY!"                                               \
    "TALLYING!THEN!TOP!"                                           \
    "TAPE!THROUGH!TRACE!"                                          \
    "TERMINAL!THRU!TRAILING!"                                      \
    "TERMINATE!TIME!TRUE!"                                         \
    "TEST!TIMES!TYPE!"                                             \
    /*   U   */                                                    \
    "UNIT!UP!USE!"                                                 \
    "UNSTRING!UPON!USING!"                                         \
    "UNTIL!USAGE!"                                                 \
    /*   V   */                                                    \
    "VALUE!VALUES!VARYING!"                                        \
    /*   W   */                                                    \
    "WHEN!WORDS!WRITE-ONLY!"                                       \
    "WORKING-STORAGE!WHEN-COMPILED!"                               \
    "WITH!WRITE!"                                                  \
    /*   X   */                                                    \
    /*   Y   */                                                    \
    /*   Z   */                                                    \
    "ZERO!ZEROES!ZEROS"                                            \
    /*  END  */                                                    \
    ")(\\.! !,!$!'!\")(.*)";

  /*    _____________________________________________________
      _(                                                     )_
     =_                   S Y M B O L S                       _=
       (_____________________________________________________)
                                                                  */

    /****/ char *ptrnSymbols = "^( !,)*("                          \
                            "\\.!"             /*  .        */    \
                            "\\+!\\-!"         /*  +  -     */    \
                            "\\(!\\)!"         /*  (  )     */    \
                            "\\*\\*!\\*!\\/!"  /*  ** *  /  */    \
                            "<=!>=!"           /*  <= >=    */    \
                            "<!>!=!"           /*  <  >  =  */    \
                            ":!\\$"            /*  :  $     */    \
                            ")(.*)";

  /*    _____________________________________________________
      _(                                                     )_
     =_                   E N D  L I N E                     _=
       (_____________________________________________________)
                                                                  */

    /****/ char *ptrnEndLine = "^(( !,)*)$";

  /*    _____________________________________________________
      _(                                                     )_
     =_                     S P A C E                        _=
       (_____________________________________________________)
                                                                  */

    /****/ char  *ptrnSpace = "(^( !,)+)";


  /*    ______________________________
      _(                              )_
     =_    SPECIAL REGISTER KEYWORD    _=
       (______________________________)
                                                                  */

    /****/ char *ptrnSpclReg  = "^( !,)*"                           \
                               "(ADDRESS!"                         \
                               "DEBUG-ITEM!DEBUG-LINE!DEBUG-NAME!" \
                               "LENGTH!RETURN-CODE!SORT-RETURN!"   \
                               "WHEN-COMPILED!TALLY)"              \
                               "(\\.! !,!$!'!\")(.*)";

  /*    ______________________________
      _(                              )_
     =_  FIGURATIVE CONSTANTS KEYWORD  _=
       (______________________________)
                                                                  */

    /****/ char *ptrnFgrtvConst = "^( !,)*("                       \
                                 "ALL!"                            \
                                 "ZERO!ZEROS!ZEROES!"              \
                                 "SPACE!SPACES!"                   \
                                 "HIGH-VALUE!HIGH-VALUES!"         \
                                 "LOW-VALUE!LOW-VALUES!"           \
                                 "QUOTE!QUOTES"                    \
                                 ")(\\.! !,!$!'!\")(.*)";

  /*    _____________________________
      _(                             )_
     =_        NUMERIC LITERAL        _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnNumeric = "^("                                  \
                              "ZERO!ZEROS!ZEROES!"                  \
                              "([\\+\\-]?[0-9]+)!"       /* Intgr */\
                              "([\\+\\-]?[0-9]*,[0-9]+)" /* Decim */\
                              ")$";

  /*    _____________________________
      _(                             )_
     =_        QUOTED LITERAL         _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnQuoted = "^("                                    \
                             "('([^']!'')*')!"            /*  '.' */ \
                             "(\"([^\"]!\"\")*\")!"       /*  "." */ \
                             "([Xx]['][0-9A-Fa-f]+['])!"  /* x'.' */ \
                             "([Xx][\"][0-9A-Fa-f]+[\"])" /* x"." */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_    SINGLE QUOTED LITERAL      _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnSQuoted = "^("                                  \
                             "('([^']!'')*')!"            /*  '.' */ \
                             "([Xx]['][0-9A-Fa-f]+['])"  /* x'.' */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_    DOUBLE QUOTED LITERAL      _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnDQuoted = "^("                                   \
                             "(\"([^\"]!\"\")*\")!"       /*  "." */ \
                             "([Xx][\"][0-9A-Fa-f]+[\"])" /* x"." */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_      HEXADECIMAL LITERAL      _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnHex     = "^("                                   \
                             "([Xx]['][0-9A-Fa-f]+['])!"  /* x'.' */ \
                             "([Xx][\"][0-9A-Fa-f]+[\"])" /* x"." */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_      ALPHANUMERIC LITERAL     _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnAlphanum = "^("                                  \
                             "('([^']!'')*')!"            /*  '.' */ \
                             "(\"([^\"]!\"\")*\")"       /*  "." */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_        INTEGER LITERAL        _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnIntgr    = "^("                                 \
                             "([\\+\\-]?[0-9]+)"          /*  987 */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_        DECIMAL LITERAL        _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnDecim    = "^("                                \
                             "([\\+\\-]?[0-9]*,[0-9]+)"             \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_      UNSIGNED NUMERIC         _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnUnsigned = "^("                               \
                             "([0-9]+)!"                /* 987 */  \
                             "([0-9]*,[0-9]+)"          /* 9,7 */  \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_      POSITIVE NUMERIC         _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnPositive = "^("                               \
                             "(\\+[0-9]+)!"             /* +987 */ \
                             "(\\+[0-9]*,[0-9]+)"       /* +9,7 */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_      NEGATIVE NUMERIC         _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnNegative = "^("                                 \
                             "(\\-[0-9]+)!"             /* -987 */ \
                             "(\\-[0-9]*,[0-9]+)"       /* -9,7 */ \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_         LEVEL NUMBER          _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnLvlNum = "^("                                   \
                             "([0]?[1-9]![1-4][0-9]!66!77!88)"     \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_      RELATIONAL OPERATOR      _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnRelOp  = "^("                                   \
                             "(<!>!<=!>=!=)"                       \
                              ")$";

  /*    _____________________________
      _(                             )_
     =_      ARITHMITIC OPERATOR      _=
       (_____________________________)
                                                                  */

    /****/ char *ptrnAritOp  = "^("                                  \
                              "\\+!\\-!"         /*  +  -     */   \
                              "\\(!\\)!"         /*  (  )     */   \
                              "\\*\\*!\\*!\\/"  /*  ** *  /  */   \
                              ")$";

  /*
    char       *ptrnLiter2 = "^( !,)*"                              \
                            "(\"([^\"]!\"\")*\")"                   \
                            "([^\"]!$)(.*)";
  */
  /*-----------------------------------*/
  /* Pattern pour HEXA Literal X'FF'   */
  /*-----------------------------------*/
  /*char       *ptrnHexL1  = "^( !,)*"                              \
                            "([Xx][\"][0-9A-Fa-f]+[\"])"            \
                            "([^\"]!$)(.*)";

    char       *ptrnHexL2  = "^( !,)*"                              \
                            "([Xx]['][0-9A-Fa-f]+['])"              \
                            "([^']!$)(.*)";
  */
  /*-----------------------------*/
  /* Pattern pour Integer        */
  /*-----------------------------*/
  /*char       *ptrnIntgr = "^( !,)*"                             \
                            "([\\+\\-]?[0-9]+)"                   \
                            "(.)(.*)";
  */
  /*-----------------------------*/
  /* Pattern pour Level Number   */
  /*-----------------------------*/
  /*char       *ptrnLvlnum = "([0]?[1-9]![1-4][0-9]!77)";

  */
  /*-----------------------------*/
  /* Pattern pour Level 66       */
  /*-----------------------------*/
  /*char       *ptrnLvl66  = "66";
  */
  /*-----------------------------*/
  /* Pattern pour Level 88       */
  /*-----------------------------*/
  /*char       *ptrnLvl88  = "88";
  */
  /*-----------------------------*/
  /* Pattern pour Decimal        */
  /*-----------------------------*/
  /*char       *ptrnDecim  = "^( !,)*"                             \
                            "([\\+\\-]?[0-9]*,[0-9]+)"             \
                            "(.)(.*)";
  */
  /*-------------------------------*/
  /* Pattern pour Unsigned integer */
  /*-------------------------------*/
  /*char       *ptrnUintgr  = "^([0-9]+)$";
  */

  /*---------------------------------------------------------------*/
  /*---------------------------------------------------------------*/
  /*---------------------------------------------------------------*/

    int         rc=0;
    int         i=0;
    int         j=0;
    size_t      nmatch = NB_MATCH_MAX;
    regmatch_t  pmatch[NB_MATCH_MAX];
    //char        buffer[100];
    char        str[480];

    static int  rets_len =0;
    static char rets_data[480]="";

    char        args[480]="";

    strcpy(args,ln_8_72);

  /*---------------------------------------------------------------*/
  /*---------------------------------------------------------------*/
  /*---------------------------------------------------------------*/

  /*---------------------------------------------------------------*/
  /* Ordre des REGEX                                               */
  /*---------------------------------------------------------------*/
  /* 1  - Keyword                                                  */
  /* 10 - Special registers                                        */
  /* 101- Figurative constants                                     */
  /* 11 - Hexa Literal1 & Hexa Literal2                            */
  /* 2  - Identifier                                               */
  /* 3  - Literal1 & Literal2                                      */
  /* 31 - Decimal                                                  */
  /* 32 - Integer                                                  */
  /* 4  - Period                                                   */
  /*      & plus minus equal expon divid multp Lbrack Rbrack colon */
  /* 5  - EndLine                                                  */
  /* 6  - Error                                                    */
  /*---------------------------------------------------------------*/
    /* Pointer Array for pregs order for type*/
    /****/ regex_npp   pregs [NB_REGEX_MAX];

    int nb_regex_for_type;

    /* Default pregs order (empty context) */

    pregs[j].name ="KEYWORD";
    pregs[j].ptrn =ptrnKeywoAll;
    pregs[j].preg =&pregKeywoAll;
    j++;
    pregs[j].name ="LITERAL";
    pregs[j].ptrn =ptrnLiterQuot;
    pregs[j].preg =&pregLiterQuot;
    j++;
    pregs[j].name ="LITERAL";
    pregs[j].ptrn =ptrnLiterNum;
    pregs[j].preg =&pregLiterNum;
    j++;
    pregs[j].name ="IDENTIFIER";
    pregs[j].ptrn =ptrnIdent;
    pregs[j].preg =&pregIdent;
    j++;
    pregs[j].name ="SYMBOL";
    pregs[j].ptrn =ptrnSymbols;
    pregs[j].preg =&pregSymbols;
    j++;
    pregs[j].name ="ENDLINE";
    pregs[j].ptrn =ptrnEndLine;
    pregs[j].preg =&pregEndLine;
    j++;

    /* Space ??
    pregs[j].name ="SPACE";
    pregs[j].ptrn =ptrnSpace;
    pregs[j].preg =&pregSpace;
    j++; */

 /* pregs[j].name ="KEYWORD";
    pregs[j].ptrn =ptrnKeywoAll;
    pregs[j].preg =&pregKeywoAll;
    j++;
    pregs[j].name ="SPECIAL REGISTER";
    pregs[j].ptrn =ptrnSpclReg;
    pregs[j].preg =&pregSpclReg;
    j++;
    pregs[j].name ="FIGURATIVE CONST";
    pregs[j].ptrn =ptrnFgrtvConst;
    pregs[j].preg =&pregFgrtvConst;
    j++;
    pregs[j].name ="HEXLITERAL";
    pregs[j].ptrn =ptrnHexL1;
    pregs[j].preg =&pregHexL1;
    j++;
    pregs[j].name ="HEXLITERAL";
    pregs[j].ptrn =ptrnHexL2;
    pregs[j].preg =&pregHexL2;
    j++;
    pregs[j].name ="IDENTIFIER";
    pregs[j].ptrn =ptrnIdent;
    pregs[j].preg =&pregIdent;
    j++;
    pregs[j].name ="LITERAL";
    pregs[j].ptrn =ptrnLiter1;
    pregs[j].preg =&pregLiter1;
    j++;
    pregs[j].name ="LITERAL";
    pregs[j].ptrn =ptrnLiter2;
    pregs[j].preg =&pregLiter2;
    j++;
    pregs[j].name ="DECIMAL";
    pregs[j].ptrn =ptrnDecim;
    pregs[j].preg =&pregDecim;
    j++;
    pregs[j].name ="INTEGER";
    pregs[j].ptrn =ptrnIntgr;
    pregs[j].preg =&pregIntgr;
    j++;
    pregs[j].name ="SYMBOL";
    pregs[j].ptrn =ptrnSymbols;
    pregs[j].preg =&pregSymbols;
    j++;
    pregs[j].name="ENDLINE";
    pregs[j].ptrn=ptrnEndLine;
    pregs[j].preg=&pregEndLine;
    j++;
  */
    nb_regex_for_type = j;

  /*---------------------------------------------------------------*/
  /*-------------- ! Context Sensitive Lexer ! --------------------*/
  /*---------------------------------------------------------------*/
  /* If clause data_pic_chars => add PICCHARS as the first regex   */
  /*---------------------------------------------------------------*/
  /*                          => next is the pattern for space     */
  /*---------------------------------------------------------------*/

    if((strcmp(_context.clause,"data_pic_chars"))==0){

       j = 0;

       pregs[j].name ="PICCHARS";
       pregs[j].ptrn =ptrnPicChars;
       pregs[j].preg =&pregPicChars;
       j++;
       pregs[j].name ="SPACE";
       pregs[j].ptrn =ptrnSpace;
       pregs[j].preg =&pregSpace;
       j++;
       pregs[j].name ="SYMBOL"; /* ?? symbol ici */
       pregs[j].ptrn =ptrnSymbols;
       pregs[j].preg =&pregSymbols;
       j++;
       pregs[j].name ="KEYWORD";
       pregs[j].ptrn =ptrnKeywoAll;
       pregs[j].preg =&pregKeywoAll;
       j++;
       pregs[j].name ="LITERAL";
       pregs[j].ptrn =ptrnLiterQuot;
       pregs[j].preg =&pregLiterQuot;
       j++;
       pregs[j].name ="LITERAL";
       pregs[j].ptrn =ptrnLiterNum;
       pregs[j].preg =&pregLiterNum;
       j++;
       pregs[j].name ="IDENTIFIER";
       pregs[j].ptrn =ptrnIdent;
       pregs[j].preg =&pregIdent;
       j++;
       pregs[j].name ="ENDLINE";
       pregs[j].ptrn =ptrnEndLine;
       pregs[j].preg =&pregEndLine;
       j++;
/*
       pregs[j].name ="PICCHARS";
       pregs[j].ptrn =ptrnPicChars;
       pregs[j].preg =&pregPicChars;
       j++;
*/   /*pregs[j].name ="SYMBOL";
       pregs[j].ptrn =ptrnPerio;
       pregs[j].preg =&pregPerio;
       j++;*/
/*     pregs[j].name ="SYMBOL";
       pregs[j].ptrn =ptrnSymbols;
       pregs[j].preg =&pregSymbols;
       j++;
       pregs[j].name="SPACE";
       pregs[j].ptrn=ptrnSpace;
       pregs[j].preg=&pregSpace;
       j++;
       pregs[j].name ="KEYWORD";
       pregs[j].ptrn =ptrnKeywoAll;
       pregs[j].preg =&pregKeywoAll;
       j++;
       pregs[j].name ="SPECIAL REGISTER";
       pregs[j].ptrn =ptrnSpclReg;
       pregs[j].preg =&pregSpclReg;
       j++;
       pregs[j].name ="FIGURATIVE CONST";
       pregs[j].ptrn =ptrnFgrtvConst;
       pregs[j].preg =&pregFgrtvConst;
       j++;
       pregs[j].name ="HEXLITERAL";
       pregs[j].ptrn =ptrnHexL1;
       pregs[j].preg =&pregHexL1;
       j++;
       pregs[j].name ="HEXLITERAL";
       pregs[j].ptrn =ptrnHexL2;
       pregs[j].preg =&pregHexL2;
       j++;
       pregs[j].name ="IDENTIFIER";
       pregs[j].ptrn =ptrnIdent;
       pregs[j].preg =&pregIdent;
       j++;
       pregs[j].name ="LITERAL";
       pregs[j].ptrn =ptrnLiter1;
       pregs[j].preg =&pregLiter1;
       j++;
       pregs[j].name ="LITERAL";
       pregs[j].ptrn =ptrnLiter2;
       pregs[j].preg =&pregLiter2;
       j++;
       pregs[j].name ="DECIMAL";
       pregs[j].ptrn =ptrnDecim;
       pregs[j].preg =&pregDecim;
       j++;
       pregs[j].name ="INTEGER";
       pregs[j].ptrn =ptrnIntgr;
       pregs[j].preg =&pregIntgr;
       j++;
       pregs[j].name="ENDLINE";
       pregs[j].ptrn=ptrnEndLine;
       pregs[j].preg=&pregEndLine;
       j++;
 */    nb_regex_for_type = j;
    }

  /* Additional Regex for token attributes   */
  /****/ regex_npp   pregs_attr [NB_REGEX_MAX];

  int nb_regex_for_attr;
  j =0;

  pregs_attr[j].name ="SPECIAL REGISTER";
  pregs_attr[j].ptrn =ptrnSpclReg;
  pregs_attr[j].preg =&pregSpclReg;
  j++;
  pregs_attr[j].name ="FIGURATIVE CONST";
  pregs_attr[j].ptrn =ptrnFgrtvConst;
  pregs_attr[j].preg =&pregFgrtvConst;
  j++;
  pregs_attr[j].name ="NUMERIC";
  pregs_attr[j].ptrn =ptrnNumeric;
  pregs_attr[j].preg =&pregNumeric;
  j++;
  pregs_attr[j].name ="QUOTED";
  pregs_attr[j].ptrn =ptrnQuoted;
  pregs_attr[j].preg =&pregQuoted;
  j++;
  pregs_attr[j].name ="SINGLE QUOTE";
  pregs_attr[j].ptrn =ptrnSQuoted;
  pregs_attr[j].preg =&pregSQuoted;
  j++;
  pregs_attr[j].name ="DOUBLE QUOTED";
  pregs_attr[j].ptrn =ptrnDQuoted;
  pregs_attr[j].preg =&pregDQuoted;
  j++;
  pregs_attr[j].name ="HEX";
  pregs_attr[j].ptrn =ptrnHex;
  pregs_attr[j].preg =&pregHex;
  j++;
  pregs_attr[j].name ="ALPHANUMERIC";
  pregs_attr[j].ptrn =ptrnAlphanum;
  pregs_attr[j].preg =&pregAlphanum;
  j++;
  pregs_attr[j].name ="INTEGER";
  pregs_attr[j].ptrn =ptrnIntgr;
  pregs_attr[j].preg =&pregIntgr;
  j++;
  pregs_attr[j].name ="DECIMAL";
  pregs_attr[j].ptrn =ptrnDecim;
  pregs_attr[j].preg =&pregDecim;
  j++;
  pregs_attr[j].name ="UNSIGNED";
  pregs_attr[j].ptrn =ptrnUnsigned;
  pregs_attr[j].preg =&pregUnsigned;
  j++;
  pregs_attr[j].name ="POSITIVE";
  pregs_attr[j].ptrn =ptrnPositive;
  pregs_attr[j].preg =&pregPositive;
  j++;
  pregs_attr[j].name ="NEGATIVE";
  pregs_attr[j].ptrn =ptrnNegative;
  pregs_attr[j].preg =&pregNegative;
  j++;
  pregs_attr[j].name ="LEVEL NUMBER";
  pregs_attr[j].ptrn =ptrnLvlNum;
  pregs_attr[j].preg =&pregLvlnum;
  j++;
  pregs_attr[j].name ="RELATIONAL OP";
  pregs_attr[j].ptrn =ptrnRelOp;
  pregs_attr[j].preg =&pregRelOp;
  j++;
  pregs_attr[j].name ="ARITHMITIC OP";
  pregs_attr[j].ptrn =ptrnAritOp;
  pregs_attr[j].preg =&pregAritOp;
  j++;

  nb_regex_for_attr = j;

  /*
  regex_npp   reg_lvlnum;
  regex_npp   reg_lvl66;
  regex_npp   reg_lvl88;
  regex_npp   reg_uintgr;

  reg_lvlnum.name="LVLNUM";
  reg_lvlnum.ptrn=ptrnLvlnum;
  reg_lvlnum.preg=&pregLvlnum;

  reg_lvl66.name="LVL66";
  reg_lvl66.ptrn=ptrnLvl66;
  reg_lvl66.preg=&pregLvl66;

  reg_lvl88.name="LVL88";
  reg_lvl88.ptrn=ptrnLvl88;
  reg_lvl88.preg=&pregLvl88;

  reg_uintgr.name="UINTEGER";
  reg_uintgr.ptrn=ptrnUintgr;
  reg_uintgr.preg=&pregUintgr;

  if(!(compil_regexp(reg_lvlnum))) return _ret;
  if(!(compil_regexp(reg_lvl66))) return _ret;
  if(!(compil_regexp(reg_lvl88))) return _ret;
  if(!(compil_regexp(reg_uintgr))) return _ret;
  */

  /*---------------------------------------------------------------*/
  /*-------------- ! Compilation des Regex  ! ---------------------*/
  /*---------------------------------------------------------------*/

     /* _______________________________
      _(                               )_
     =_  COMPILE REGEX FOR TOKEN TYPE   _=
       (_______________________________)*/


 /* debug_2("regcomp_type_done (%d) \n",regcomp_type_done);

    if(!(regcomp_type_done)) {
 */
       i=0;
       while ((i < nb_regex_for_type) && (compil_regexp(pregs[i])))
       {
        //debug_3("Compilation regex (%s) reussie \n",pregs[i].name);
          i++;
       }

       /* Sortie pr{mature : erreur compilation */
       if (i < nb_regex_for_type) exit(EXIT_FAILURE);
       else regcomp_type_done = 1;
 /* }
 */
     /* ____________________________________
      _(                                    )_
     =_  COMPILE REGEX FOR TOKEN ATTRIBUTES  _=
       (____________________________________)*/


 /* debug_2("regcomp_attr_done (%d) \n",regcomp_attr_done);

    if(!(regcomp_attr_done)) {
 */
       i=0;
       while ((i < nb_regex_for_attr) && (compil_regexp(pregs_attr[i])))
       {
        //debug_3("Compilation regex (%s) reussie \n",
        //                                 pregs_attr[i].name);
          i++;
       }


       /* Sortie pr{mature : erreur compilation */
       if (i < nb_regex_for_attr) exit(EXIT_FAILURE);
       else regcomp_attr_done = 1;
  /*
    }
  */
     /* _____________________________
      _(                             )_
     =_  MATCH REGEX FOR TOKEN TYPE   _=
       (_____________________________)*/

    i =0;
    while ((i < nb_regex_for_type) &&
          /* patch horrible dans condition while */
         (((rc =regexec(pregs[i].preg, args, nmatch, pmatch, 0)) != 0)
          /* continuer la recherche si on trouve picchar et   */
          /* son dernier caractere est le point et le prochain*/
          /* caractere nest pas le point piouf .. */
        /*
        ||(((rc =regexec(pregs[i].preg, args, nmatch, pmatch, 0)) == 0)
        && (strcmp(pregs[i].name,"PICCHARS")==0)    picchar
        && (*(args+pmatch[2].rm_eo-1)=='.')    dernier char est '.'
        && ((pmatch[2].rm_eo)<=strlen(args))    pas fin de args
        && (*(args+(pmatch[2].rm_eo))!='.'))
        */
          )) {
        /*
        if(strcmp(pregs[i].name,"PICCHARS")==0) {

        debug_2("this line should not be printed, otherwise it's bug");
        debug_2("rc (%d)\n",rc);
        debug_2("\npregs.name            (%s)\n",pregs[i].name);
        debug_2("args+pmatch2.rm_eo-1  (%d)\n",args+pmatch[2].rm_eo-1);
        debug_2("*args+pmatch2.rm_eo-1 (%c)\n",
                                          *(args+pmatch[2].rm_eo-1));
        debug_2("strlen(args)          (%d)\n",strlen(args));
        debug_2("pmatch2.rm_eo         (%d)\n",(pmatch[2].rm_eo));
        debug_2("*(args+(pmatch2.rm_eo)(%c)\n",
                                          *(args+(pmatch[2].rm_eo)));
        */
        /* Display matched sub-expression */
        /* affich_subexpr(pmatch,args); */
     /* } */
    // affich_fail_match(rc,args,pregs[i].name,pregs[i].preg);
       i++;
    }

    /* match found */
    if (i < nb_regex_for_type) {

        if(strcmp(pregs[i].name,"PICCHARS")==0) {
        /*
        debug_2("pregs.name            (%s)\n",pregs[i].name);
        debug_2("args+pmatch2.rm_eo-1  (%d)\n",args+pmatch[2].rm_eo-1);
        debug_2("*args+pmatch2.rm_eo-1 (%c)\n",
                                          *(args+pmatch[2].rm_eo-1));
        debug_2("strlen(args)          (%d)\n",strlen(args));
        debug_2("pmatch2.rm_eo         (%d)\n",(pmatch[2].rm_eo));
        debug_2("*(args+(pmatch2.rm_eo)(%c)\n",
                                          *(args+(pmatch[2].rm_eo)));
        */
        /* Display matched sub-expression */
        /* affich_subexpr(pmatch,args);
        */
        /* Patch pour annulation context une fois picchar lu */
        erase_context();
       }


        /* extraire token vers var tmp : str */
        sprintf(str,"%.*s",pmatch[2].rm_eo - pmatch[2].rm_so,
                  args+pmatch[2].rm_so);

        /* reduire args */
        /*
        sprintf(args,"%.*s",pmatch[3].rm_eo - pmatch[3].rm_so,
                  args+pmatch[3].rm_so); */

        reduction_len  = 65 - strlen(args);

        sprintf(args,"%.*s",((strlen(args))-(pmatch[2].rm_eo))
                 ,args+(pmatch[2].rm_eo));

        _ret.tkn_colnum = pmatch[2].rm_so + 1 + reduction_len;
        _ret.tkn_len    = pmatch[2].rm_eo - pmatch[2].rm_so;

      /*printf("tokenizer     :strlen(args) (%d) reduction_len (%d)" \
        " tkn_colnum (%d) pmatch[2].rm_so (%d) tkn_len (%d) \n",
        strlen(args),reduction_len,_ret.tkn_colnum,pmatch[2].rm_so,
        _ret.tkn_len);
      */
        /* concat str avec rets */
        strcat(rets_data,pregs[i].name);
        strcat(rets_data,";;");
        strcat(rets_data,str);
        strcat(rets_data,";;");
        char* temp="\0";
        sprintf(temp, "%d", _ret.tkn_colnum);
        strcat(rets_data,temp);
        strcat(rets_data,";;");

        _ret.tkn_type   = pregs[i].name;
        _ret.tkn_val    = str;


        /*
        if ((rc =regexec(reg_lvlnum.preg,str,nmatch,pmatch,0)) == 0){
            _ret.tkn_attr[0] = "LVLNUM";
            printf ("LVLNUM found \n");
        }
        if ((rc =regexec(reg_lvl66.preg,str,nmatch,pmatch,0)) == 0){
            _ret.tkn_attr[0]= "LVL66";
            printf ("LVL66  found \n");
        }
        if ((rc =regexec(reg_lvl88.preg,str,nmatch,pmatch,0)) == 0){
            _ret.tkn_attr[0]= "LVL88";
            printf ("LVL88  found \n");
        }
        if ((rc =regexec(reg_uintgr.preg,str,nmatch,pmatch,0)) == 0){
            _ret.tkn_attr[1]= "UINTEGER";
            printf ("UINTEGER  found \n");
        } */
    }

    /* no match found */
    else {

       strcpy(rets_data,"ERROR;;");
       _ret.tkn_type = "ERROR";
       _ret.tkn_val = "ERROR";

       printf("ERROR, args :%s",args);
    }

     /* ____________________________________
      _(                                    )_
     =_  COMPILE REGEX FOR TOKEN ATTRIBUTES  _=
       (____________________________________)*/

    i =0;
    j =0;
    _ret.tkn_attr_len =0;
    while ((i < nb_regex_for_attr) &&
           (rc=strcmp(_ret.tkn_val,"ERROR"))!=0)
    {
       if((rc =regexec(pregs_attr[i].preg,str,nmatch,pmatch,0)) == 0)
       {
          /* Display matched sub-expression */
        /*affich_subexpr(pmatch,str); */
          _ret.tkn_attr[j]=pregs_attr[i].name;
          j++;
       }
       else
       {
        /*affich_fail_match(rc,args,pregs[i].name,pregs[i].preg);*/
       }

       i++;
    }
    _ret.tkn_attr_len=j;

    rets_len = strlen(rets_data);
    /*
    printf("\n");
    printf("*----------------------------------------------*\n");
    printf("*--------------  END LEXERCOB  ----------------*\n");
    printf("*----------------------------------------------*\n");
    printf("*   Rets length : %d \n", rets_len);
    printf("*   Rets data   :'%s'\n", rets_data);
    printf("*----------------------------------------------*\n");
    */

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

    i =0;
    while (i < _ret.tkn_attr_len) {
       debug_2("*   Token Attr  : %s \n", _ret.tkn_attr[i]);
       i++;
    }
    debug_2("*----------------------------------------------*\n");


    for(i=0;i<nb_regex_for_type;i++)
        regfree(pregs[i].preg);

    for(i=0;i<nb_regex_for_attr;i++)
        regfree(pregs_attr[i].preg);

    /*
    regfree(&pregKeywo);
    regfree(&pregIdent);
    regfree(&pregPerio);
    regfree(&pregLiter1);
    regfree(&pregLiter2);
    regfree(&pregHexL1);
    regfree(&pregHexL2);
    regfree(&pregKeywoAll);
    regfree(&pregLBrack);
    regfree(&pregRBrack);
    regfree(&pregPlus);
    regfree(&pregMinus);
    regfree(&pregMultp);
    regfree(&pregDivid);
    regfree(&pregExpon);
    regfree(&pregEqual);
    regfree(&pregColon);
    regfree(&pregIntgr);
    regfree(&pregDecim);

    printf("tokenizer()   : ");
    printf("Token Value <%s> Token Type   <%s> \n",
            _ret.tkn_val,_ret.tkn_type);
    */
    /*
    printf("tokenizer()   : ");
    printf("args        : <%s>\n",args);
    printf("len args    : <%d>\n",strlen(args));
    */
    strcpy(ln_8_72,args);

    /*
    printf("tokenizer()   : ");
    printf("ln_8_72     : <%s>\n",ln_8_72);
    printf("len ln_8_72 : <%d>\n",strlen(ln_8_72));
    */
    return _ret;

}


int affich_subexpr(regmatch_t pmatch[],char args[]){

   int i =0;

   printf("Whole expression :<");
   printf("%s",args);
   printf(">\n");

   for (i=1;i<NB_MATCH_MAX;i++){

      printf("\nSub-expression %d, a matched sub-string " \
             "\"%.*s\" found at position %d to %d.\n ",
      i, pmatch[i].rm_eo - pmatch[i].rm_so,
      args+pmatch[i].rm_so,pmatch[i].rm_so,
      pmatch[i].rm_eo - 1);

   }
   printf("\n");

   return 1;
}

int affich_fail_match(int rc,char args[],char* ptrn,regex_t* preg){

   char buffer[100];

   regerror(rc, preg, buffer, 100);

   printf("failed to match\n'%s'with\n'%s',\nRC :%d, '%s'\n"
             ,args, ptrn, rc, buffer);

   return 1;
}

int compil_regexp(regex_npp _regex_npp){

    int  rc =0;
    char buffer[100];

    /* Compiler la REGEXP pour ptrn dans preg */

    if ((rc = regcomp(_regex_npp.preg, _regex_npp.ptrn,
         REG_ICASE|REG_EXTENDED)) != 0)
    {
       regerror(rc, _regex_npp.preg, buffer, 100);
       printf("regcomp(%s) fail RC (%d : '%s')\n",_regex_npp.name,
              rc, buffer);
       return 0;
    }
    /* printf("regcomp for (%s) succesful \n",_regex_npp.name);*/

    return 1;
}

int
erase_context(){

    debug_2("Erasing Context ... OK\n");
    _context.division ="";
    _context.section  ="";
    _context.clause   ="";
    _context.keyword  ="";
    _context.other    ="";

    return  1;

}
