#include <stdio.h>
#define __XPG4 // itoa, strccase
#define __UU
#define __OE_8
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "debug.h"
#include "rexxcom.h"

int        argc;
int        cursl;
int        cursc;
char       src_file[DS_NAME_LEN];
char       inc_pds[NB_INC_PDS][DS_NAME_LEN];
char       options[NB_OPTIONS][DS_NAME_LEN];
int        main_ret;
_editLine  result[NB_RESULTS];

//int        curr_ln_num;
_srcLine   *src_list;
_srcLine   *curr_src_line;

/* bug connu : rawcode depasse le code reel par 14 car à la fin */
int getArgsRexx() {

       int    i=0;
       int    rawCode_length=0;
       int    rawCode_mod_80=0;
       int    rawCode_div_80=0;

       debug_3("osplist 0 : %s \n",        __osplist[0]) ;

       if ((strcmp(__osplist[0],"")) && (atoi(__osplist[0]) > 0)) {
          argc = atoi(__osplist[0]);
          debug_3("argc          : (%d) \n", argc);
       }
       else {
          printf("getArgsRexx() : ");
          printf("Required argument argc not found or not numeric \n");
		  exit(EXIT_FAILURE);
       }

       debug_3("osplist 1 : %s \n",        __osplist[1]) ;
       if ((strcmp(__osplist[1],"")) && (atoi(__osplist[1]) > 0)) {
          cursc = atoi(__osplist[1]);
          debug_3("cursc         : (%d) \n", cursc);
       }
       else {
          printf("getArgsRexx() : ");
          printf("Required argument cursc not found or not numeric\n");
          exit(EXIT_FAILURE);
       }

       debug_3("osplist 2 : %s \n",        __osplist[2]) ;
       if ((strcmp(__osplist[2],"")) && (atoi(__osplist[2]) > 0)) {
          cursl = atoi(__osplist[2]);
          debug_3("cursl         : (%d) \n", cursl);
          /* current line number to increment on each read */
          //curr_ln_num = cursl;
       }
       else {
          printf("getArgsRexx() : ");
          printf("Required argument cursl not found or not numeric\n");
          exit(EXIT_FAILURE);
       }

       debug_3("osplist 3 : %s \n",        __osplist[3]) ;
       if ((strcmp(__osplist[3],""))) {
          strcpy(src_file,__osplist[3]);
          debug_3("src_file      : (%s) \n", src_file);
       }
       else {
          printf("getArgsRexx() : ");
          printf("Required argument src_file not found \n");
          exit(EXIT_FAILURE);
       }

       debug_3("osplist 4 : %s \n",        __osplist[4]) ;
       if ((strcmp(__osplist[4],""))) {
          break_str_space(__osplist[4],inc_pds,NB_INC_PDS);
          debug_3("inc_pds(0)    : (%s) \n", inc_pds[0]);
       }
       else {
          printf("getArgsRexx() : ");
          printf("Required argument inc_pds  not found \n");
          exit(EXIT_FAILURE);
       }

       debug_3("osplist 5 : %s \n",        __osplist[5]) ;
       if ((strcmp(__osplist[5],""))) {
          break_str_space(__osplist[5],options,NB_OPTIONS);
          debug_3("options(0)    : (%s) \n", options[0]);
       }
       else {
          printf("getArgsRexx() : ");
          printf("Required argument options  not found \n");
          exit(EXIT_FAILURE);
       }

       src_list = getSource(src_file,0);

       curr_src_line = getCurrSrcLine(src_list,cursl,cursc);

}

void
setCurrLnStart(){

    curr_src_line= src_list;

}

int
break_str_space(char *line, char words[][DS_NAME_LEN],int maxWords){

   char *p          = line;
   char *buff[DS_NAME_LEN];
   int   nbWords       = 0;
   int   i;

   while(1)
   {
      while(isspace(*p))
        p++;


      if(*p == '\0')
        break;

      buff[nbWords++] = p;

      while(!isspace(*p) && *p != '\0')
        p++;

      if(*p == '\0')
        break;

      *p++ = '\0';

      if(nbWords >= maxWords)
        break;
   }

   for(i=0;i<=nbWords;i++){
      strcpy(words[i],buff[i]);
   }

   return nbWords;
}

_srcLine*
getSource(char* srcFile,int currLine){

  FILE      *stream;
  _srcLine  *p;
  _srcLine  *l;
  char      buff[85];
  int       i;
  int       ret=0;

  printf("sizeof(_srcLine) (%d)\n",sizeof(_srcLine));

  if(stream=fopen(srcFile,
//"r,lrecl=80,blksize=240,recfm=fb,type=record")){
//"rb,lrecl=80,blksize=8000,recfm=fb,type=record")){
  "rb,lrecl=80,recfm=fb,type=record")){
//"r")){

     debug_2("Successful opening of file (%s)\n",srcFile);

  } else {

     perror("Error opening file ");
     exit(EXIT_FAILURE);
  }

  p = NULL;
  l = NULL;

  for(i=1;;i++){

     ret=fread(buff,1,80,stream);
     if(ferror(stream)){

        perror("Error reading file ");
        exit(EXIT_FAILURE);

     } else if (feof(stream)) {

        debug_2("EOF \n");
        if (fclose(stream) != 0) perror("fclose error");

        break;

     }

     if(!p){
        p = malloc(sizeof(_srcLine));
        l = p;
     } else {
        p->next = malloc(sizeof(_srcLine));
        p = p->next;
     }

     strncpy(p->value,buff,80);
  // printf("i (%d) ret (%d)\n",i,ret);
  // printf("p->value(%s)\n",p->value);

     p->num_file = i;
     p->num_all  = i + currLine;
     strcat(p->file,srcFile);
     p->next = NULL;

  }
  debug_3("nb lignes lues (%d)\n",i-1);
  debug_3("src_list l     (%d)\n",l);

  return l;

}

_srcLine*
getCurrSrcLine(_srcLine* srcList,int cursl,int cursc){

  _srcLine  *l;
  int        i;

  debug_3 ("srcList (%d)\n",srcList);
  debug_3 ("cursl   (%d)\n",cursl);
  assert(srcList);

  l = srcList;

  for(i=1;l;i++){
 //  printf("l (%s)\n",l->value);
     if(i==cursl)
        break;
     l = l->next;

  }

  if(!l) {

     printf("current srcLine cant be found from cursor\n");
     exit(EXIT_FAILURE);
  }

  debug_3 ("l       (%d)\n",l);
  return l;

}
