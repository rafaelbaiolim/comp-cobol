#include <stdio.h>
#define __XPG4 // itoa, strccase
#define __UU
#define __OE_8
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "debug.h"
#include "rexxCom.h"

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
int getArgsRexx(char* src_file) {

       int    i=0;
       int    rawCode_length=0;
       int    rawCode_mod_80=0;
       int    rawCode_div_80=0;

      
       src_list = getSource(src_file,0);

       curr_src_line = getCurrSrcLine(src_list,0,0);

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
