#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "tokenize.h"
#include "grammar.h"

#define NB_MATCH_MAX    10
#define NB_REGEX_MAX    30

token tokenizer(char* ln_8_72)
{
    token _ret = {0,0,0,"ERROR","ERROR"};

    static int reduction_len;
    int regcomp_type_done=0;
    int regcomp_attr_done=0;

    regex_t    pregIdent;
    regex_t    pregPicChars;
    regex_t    pregLiterNum;
    regex_t    pregLiterQuot;
    regex_t    pregKeywoAll;
    regex_t    pregSymbols;
    regex_t    pregEndLine;
    regex_t    pregSpace;
    
    regex_t    pregSpclReg;
    regex_t    pregFgrtvConst;
    regex_t    pregNumeric;
    regex_t    pregQuoted;
    regex_t    pregSQuoted;
    regex_t    pregDQuoted;
    regex_t    pregHex;
    regex_t    pregAlphanum;
    regex_t    pregIntgr;
    regex_t    pregDecim;
    regex_t    pregUnsigned;
    regex_t    pregPositive;
    regex_t    pregNegative;
    regex_t    pregLvlnum;
    regex_t    pregRelOp;
    regex_t    pregAritOp;


    char *ptrnIdent = 
		"^( !,)*"
		"(([0-9]+[-_]*)*[0-9]*[A-Za-z][A-Za-z0-9]*([-_]+[A-Za-z0-9]+)*)"
        "(.*)";

   char *ptrnPicChars = 
		"(^"
        "([ABEGPSVXZabegpsvxz90\\+\\-\\*\\$\\/\\,]+))"
        "([-+*$/,ABEGPSVXZabegpsvxz90]+))"
        "(( !,!$)+!\\.!\\()(.*)";

   char   *ptrnLiterNum = 
		"^( !,)*("
        "([\\+\\-]?[0-9]*,[0-9]+)!"
        "([\\+\\-]?[0-9]+)"
        ")(.*)";

    char *ptrnLiterQuot = 
		"^( !,)*("
        "('([^']!'')*')!"
        "(\"([^\"]!\"\")*\")!"
        "([Xx]['][0-9A-Fa-f]+['])!"
        "([Xx][\"][0-9A-Fa-f]+[\"])"
        ")(.*)";

    char *ptrnKeywoAll = 
		"^( !,)*("
		"ACCEPT!ALPHABETIC-LOWER!APPLY!"
		"ACCESS!ALPHABETIC-UPPER!ARE!"
		"ADD!ALPHANUMERIC!AREA!"
		"ALPHANUMERIC-EDITED!AREAS!ADDRESS!"
		"ADVANCING!ALSO!ASCENDING!"
		"AFTER!ALTER!ASSIGN!"
		"ALTERNATE!AT!ALL!"
		"ALPHABET!AND!AUTHOR!"
		"ALPHABETIC!ANY!"
		"BASIS!BINARY!BOTTOM!"
		"BEFORE!BLANK!BY!"
		"BEGINNING!BLOCK!"
		"CALL!COLUMN!COMPUTATIONAL-5!"
		"CANCEL!COM-REG!COMPUTE!"
		"CBL!COMMA!CONFIGURATION!"
		"CD!COMMON!CONTAINS!"
		"CF!COMMUNICATION!CONTENT!"
		"CH!COMP!CONTINUE!"
		"CHARACTER!COMP-1!CONTROL!"
		"CHARACTERS!COMP-2!CONTROLS!"                  
		"CLASS!COMP-3!CONVERTING!"                     
		"CLASS-ID!COMP-4!COPY!"
		"CLOCK-UNITS!COMP-5!CORR!"                     
		"CLOSE!COMPUTATIONAL!CORRESPONDING!"           
		"COBOL!COMPUTATIONAL-1!COUNT!"                 
		"CODE!COMPUTATIONAL-2!CURRENCY!"               
		"CODE-SET!COMPUTATIONAL-3!"
		"COLLATING!COMPUTATIONAL-4!"
		"DATA!DEBUG-SUB-1!DESTINATION!"                
		"DATE-COMPILED!DEBUG-SUB-2!DETAIL!"            
		"DATE-WRITTEN!DEBUG-SUB-3!DISPLAY!"            
		"DAY!DEBUGGING!DISPLAY-1!"
		"DAY-OF-WEEK!DECIMAL-POINT!DIVIDE!"            
		"DBCS!DECLARATIVES!DIVISION!"                  
		"DE!DELETE!DOWN!"                              
		"DEBUG-CONTENTS!DELIMITED!DUPLICATES!"         
		"DELIMITER!DYNAMIC!DEBUG-ITEM!"
		"DEPENDING!DEBUG-LINE!"                        
		"DESCENDING!DEBUG-NAME!"
		"EGCS!END-INVOKE!ENDING!"                      
		"EGI!END-MULTIPLY!ENTER!"                      
		"EJECT!END-OF-PAGE!ENTRY!"
		"ELSE!END-PERFORM!ENVIRONMENT!"                
		"EMI!END-READ!EOP!"                            
		"ENABLE!END-RECEIVE!EQUAL!"                    
		"END!END-RETURN!ERROR!"                        
		"END-ADD!END-REWRITE!ESI!"
		"END-CALL!END-SEARCH!EVALUATE!"                
		"END-COMPUTE!END-START!EVERY!"                 
		"END-DELETE!END-STRING!EXCEPTION!"             
		"END-DIVIDE!END-SUBTRACT!EXIT!"                
		"END-EVALUATE!END-UNSTRING!EXTEND!"
		"END-IF!END-WRITE!EXTERNAL!"
		"FALSE!FILLER!FOR!"                            
		"FD!FINAL!FROM!"                               
		"FILE!FIRST!FUNCTION!"                         
		"FILE-CONTROL!FOOTING!"
		"GENERATE!GO!GROUP!"                           
		"GIVING!GOBACK!"                               
		"GLOBAL!GREATER!"
		"HEADING!HIGH-VALUE!HIGH-VALUES!"
		"I-O!INDICATE!INSPECT!"
		"I-O-CONTROL!INHERITS!INSTALLATION!"           
		"ID!INITIAL!INTO!"                             
		"IDENTIFICATION!INITIALIZE!INVALID!"           
		"IF!INITIATE!INVOKE!"                          
		"IN!INPUT!IS!"
		"INDEX!INPUT-OUTPUT!"                          
		"INDEXED!INSERT!"
		"JUST!JUSTIFIED!"
		"KANJI!KEY!"
		"LABEL!LIMIT!LINES!"
		"LAST!LIMITS!LINKAGE!"                         
		"LEADING!LINAGE!LOCAL-STORAGE!"                
		"LEFT!LINAGE-COUNTER!LOCK!"                    
		"LINE!LENGTH!LOW-VALUE!"                       
		"LESS!LINE-COUNTER!LOW-VALUES!"
		"MEMORY!METHOD!MORE-LABELS!"                   
		"MERGE!METHOD-ID!MOVE!"                        
		"MESSAGE!MODE!MULTIPLE!"                       
		"METACLASS!MODULES!MULTIPLY!"
		"NATIVE!NO!NUMBER!"
		"NATIVE_BINARY!NOT!NUMERIC!"                   
		"NEGATIVE!NULL!NUMERIC-EDITED!"                
		"NEXT!NULLS!"
		"OBJECT!ON!OTHER!"                             
		"OBJECT-COMPUTER!OPEN!OUTPUT!"
		"OCCURS!OPTIONAL!OVERFLOW!"                    
		"OF!OR!OVERRIDE!"                              
		"OFF!ORDER!"                                   
		"OMITTED!ORGANIZATION!"
		"PACKED-DECIMAL!PIC!PROCEDURE-POINTER!"
		"PADDING!PICTURE!PROCEDURES!"                  
		"PAGE!PLUS!PROCEED!"                           
		"PAGE-COUNTER!POINTER!PROCESSING!"             
		"PASSWORD!POSITION!PROGRAM!"                   
		"PERFORM!POSITIVE!PROGRAM-ID!"
		"PF!PRINTING!PURGE!"                           
		"PH!PROCEDURE!"
		"QUEUE!QUOTE!QUOTES!"
		"RANDOM!RELATIVE!RESERVE!"                     
		"RD!RELEASE!RESET!"
		"READ!RELOAD!RETURN!"                          
		"READY!REMAINDER!RETURN-CODE!"                 
		"RECEIVE!REMOVAL!RETURNING!"                   
		"RECORD!RENAMES!REVERSED!"                     
		"RECORDING!REPLACE!REWIND!"
		"RECORDS!REPLACING!REWRITE!"                   
		"RECURSIVE!REPORT!RF!"                         
		"REDEFINES!REPORTING!RH!"                      
		"REEL!REPORTS!RIGHT!"                          
		"REFERENCE!REPOSITORY!ROUNDED!"
		"REFERENCES!RERUN!RUN!"
		"SAME!SIGN!STANDARD!"                          
		"SD!SIZE!STANDARD-1!"                          
		"SEARCH!SKIP1!STANDARD-2!"                     
		"SECTION!SKIP2!START!"
		"SECURITY!SKIP3!STATUS!"                       
		"SEGMENT!SORT!STOP!"                           
		"SEGMENT-LIMIT!SORT-CONTROL!STRING!"           
		"SELECT!SORT-CORE-SIZE!SUB-QUEUE-1!"           
		"SELF!SORT-FILE-SIZE!SUB-QUEUE-2!"
		"SEND!SORT-MERGE!SUB-QUEUE-3!"                 
		"SENTENCE!SORT-MESSAGE!SUBTRACT!"              
		"SEPARATE!SORT-MODE-SIZE!SUM!"                 
		"SEQUENCE!SUPER!SORT-RETURN!"                  
		"SEQUENTIAL!SOURCE!SUPPRESS!"
		"SERVICE!SOURCE-COMPUTER!SYMBOLIC!"            
		"SET!SYNC!SPACE!"                              
		"SHIFT-IN!SYNCHRONIZED!SPACES!"                
		"SHIFT-OUT!SPECIAL-NAMES!"
		"TABLE!TEXT!TITLE!"                            
		"THAN!TO!TALLY!"
		"TALLYING!THEN!TOP!"
		"TAPE!THROUGH!TRACE!"
		"TERMINAL!THRU!TRAILING!"
		"TERMINATE!TIME!TRUE!"
		"TEST!TIMES!TYPE!"
		"UNIT!UP!USE!"
		"UNSTRING!UPON!USING!"
		"UNTIL!USAGE!"
		"VALUE!VALUES!VARYING!"
		"WHEN!WORDS!WRITE-ONLY!"
		"WORKING-STORAGE!WHEN-COMPILED!"
		"WITH!WRITE!"
		"ZERO!ZEROES!ZEROS"
		")(\\.! !,!$!'!\")(.*)";

    char *ptrnSymbols = 
		"^( !,)*("                          \
		"\\.!"             /*  .        */    \
		"\\+!\\-!"         /*  +  -     */    \
		"\\(!\\)!"         /*  (  )     */    \
		"\\*\\*!\\*!\\/!"  /*  ** *  /  */    \
		"<=!>=!"           /*  <= >=    */    \
		"<!>!=!"           /*  <  >  =  */    \
		":!\\$"            /*  :  $     */    \
		")(.*)";

    char *ptrnEndLine = 
		"^(( !,)*)$";

    char  *ptrnSpace = 
		"(^( !,)+)";

    char *ptrnSpclReg  = 
		"^( !,)*"                           \
	    "(ADDRESS!"                         \
	    "DEBUG-ITEM!DEBUG-LINE!DEBUG-NAME!" \
	    "LENGTH!RETURN-CODE!SORT-RETURN!"   \
	    "WHEN-COMPILED!TALLY)"              \
	    "(\\.! !,!$!'!\")(.*)";

    char *ptrnFgrtvConst = 
		"^( !,)*("                       \
		"ALL!"                            \
		"ZERO!ZEROS!ZEROES!"              \
		"SPACE!SPACES!"                   \
		"HIGH-VALUE!HIGH-VALUES!"         \
		"LOW-VALUE!LOW-VALUES!"           \
		"QUOTE!QUOTES"                    \
		")(\\.! !,!$!'!\")(.*)";

    char *ptrnNumeric = 
		"^("                                  \
		"ZERO!ZEROS!ZEROES!"                  \
		"([\\+\\-]?[0-9]+)!"       /* Intgr */\
		"([\\+\\-]?[0-9]*,[0-9]+)" /* Decim */\
		")$";

    char *ptrnQuoted = 
		"^("                                    \
        "('([^']!'')*')!"            /*  '.' */ \
        "(\"([^\"]!\"\")*\")!"       /*  "." */ \
        "([Xx]['][0-9A-Fa-f]+['])!"  /* x'.' */ \
        "([Xx][\"][0-9A-Fa-f]+[\"])" /* x"." */ \
        ")$";

    char *ptrnSQuoted = 
		"^("                                  \
        "('([^']!'')*')!"            /*  '.' */ \
        "([Xx]['][0-9A-Fa-f]+['])"  /* x'.' */ \
        ")$";

    char *ptrnDQuoted = 
		"^("                                   \
		"(\"([^\"]!\"\")*\")!"       /*  "." */ \
		"([Xx][\"][0-9A-Fa-f]+[\"])" /* x"." */ \
		")$";

    char *ptrnHex = 
		"^("                                   \
		"([Xx]['][0-9A-Fa-f]+['])!"  /* x'.' */ \
		"([Xx][\"][0-9A-Fa-f]+[\"])" /* x"." */ \
		")$";

    char *ptrnAlphanum = 
		"^("                                  \
		"('([^']!'')*')!"            /*  '.' */ \
		"(\"([^\"]!\"\")*\")"       /*  "." */ \
		")$";

    char *ptrnIntgr = 
		"^("                                 \
        "([\\+\\-]?[0-9]+)"          /*  987 */ \
        ")$";

    char *ptrnDecim = 
		"^("                                \
        "([\\+\\-]?[0-9]*,[0-9]+)"             \
        ")$";

    char *ptrnUnsigned = 
		"^("                               \
        "([0-9]+)!"                /* 987 */  \
        "([0-9]*,[0-9]+)"          /* 9,7 */  \
        ")$";

    char *ptrnPositive = 
		"^("                               \
        "(\\+[0-9]+)!"             /* +987 */ \
        "(\\+[0-9]*,[0-9]+)"       /* +9,7 */ \
        ")$";

    char *ptrnNegative = 
		"^("                                 \
        "(\\-[0-9]+)!"             /* -987 */ \
        "(\\-[0-9]*,[0-9]+)"       /* -9,7 */ \
        ")$";

    char *ptrnLvlNum = 
		"^("                                   \
        "([0]?[1-9]![1-4][0-9]!66!77!88)"     \
        ")$";

    char *ptrnRelOp  = 
		"^("                                   \
        "(<!>!<=!>=!=)"                       \
        ")$";

    char *ptrnAritOp = 
		"^("                                  \
        "\\+!\\-!"         /*  +  -     */   \
        "\\(!\\)!"         /*  (  )     */   \
        "\\*\\*!\\*!\\/"  /*  ** *  /  */   \
        ")$";

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
        
    if(ln_8_72 == 0x0){
        return;
    }
    strcpy(args,ln_8_72);

    regex_npp   pregs [NB_REGEX_MAX];

    int nb_regex_for_type;

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

  
    nb_regex_for_type = j;

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
       pregs[j].name ="SYMBOL";
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
       nb_regex_for_type = j;
    }

	regex_npp   pregs_attr [NB_REGEX_MAX];

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

   i=0;
   while ((i < nb_regex_for_type) && (compil_regexp(pregs[i])))
   {
	//debug_3("Compilation regex (%s) reussie \n",pregs[i].name);
	  i++;
   }

   /* Sortie pr{mature : erreur compilation */
   if (i < nb_regex_for_type) exit(EXIT_FAILURE);
   else regcomp_type_done = 1;

       i=0;
       while ((i < nb_regex_for_attr) && (compil_regexp(pregs_attr[i])))
       {
          i++;
       }

       if (i < nb_regex_for_attr) exit(EXIT_FAILURE);
       else regcomp_attr_done = 1;

    i =0;
    while ( (i < nb_regex_for_type) &&
			(((rc =regexec(pregs[i].preg, args, nmatch, pmatch, 0)) != 0)) ) {
       i++;
    }

    if (i < nb_regex_for_type) {
        if(strcmp(pregs[i].name,"PICCHARS")==0) {
			erase_context();
		}
		
        sprintf(str,"%.*s",pmatch[2].rm_eo - pmatch[2].rm_so,
                  args+pmatch[2].rm_so);

        reduction_len  = 65 - strlen(args);

        sprintf(args,"%.*s",((strlen(args))-(pmatch[2].rm_eo))
                 ,args+(pmatch[2].rm_eo));

        _ret.tkn_colnum = pmatch[2].rm_so + 1 + reduction_len;
        _ret.tkn_len    = pmatch[2].rm_eo - pmatch[2].rm_so;

        strcat(rets_data,pregs[i].name);
        strcat(rets_data,";;");
        strcat(rets_data,str);
        strcat(rets_data,";;");
        char temp[1000];
        
        sprintf(temp, "%d", _ret.tkn_colnum);
        strcat(rets_data,temp);
        strcat(rets_data,";;");

        _ret.tkn_type   = pregs[i].name;
        _ret.tkn_val    = str;

    }

    else {

       strcpy(rets_data,"ERROR;;");
       _ret.tkn_type = "ERROR";
       _ret.tkn_val = "ERROR";

       printf("ERROR, args :%s",args);
    }

    i =0;
    j =0;
    _ret.tkn_attr_len =0;
    while ( (i < nb_regex_for_attr) &&
            ((rc=strcmp(_ret.tkn_val,"ERROR")) !=0) ) {
	   if ((rc =regexec(pregs_attr[i].preg,str,nmatch,pmatch,0)) == 0) {
		  _ret.tkn_attr[j]=pregs_attr[i].name;
		  j++;
	   }
	   else
	   {
		/*show__fail_match(rc,args,pregs[i].name,pregs[i].preg);*/
	   }

	   i++;
	}
    _ret.tkn_attr_len=j;

    rets_len = strlen(rets_data);
    
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

    strcpy(ln_8_72,args);

    return _ret;

}


int show__subexpr(regmatch_t pmatch[],char args[]){

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

int show__fail_match(int rc,char args[],char* ptrn,regex_t* preg){

   char buffer[100];

   regerror(rc, preg, buffer, 100);

   printf("failed to match\n'%s'with\n'%s',\nRC :%d, '%s'\n"
             ,args, ptrn, rc, buffer);

   return 1;
}

int compil_regexp(regex_npp _regex_npp){

    int  rc =0;
    char buffer[100];

    if ((rc = regcomp(_regex_npp.preg, _regex_npp.ptrn,
         REG_ICASE|REG_EXTENDED)) != 0)
    {
       regerror(rc, _regex_npp.preg, buffer, 100);
       printf("regcomp(%s) fail RC (%d : '%s')\n",_regex_npp.name,
              rc, buffer);
       return 0;
    }

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
