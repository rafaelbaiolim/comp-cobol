#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "helper.h"

char* dummy_border = "***********************************************" \
"***********************************************";

scr_line*
init_screen(){

   scr_line* scr=(scr_line*) malloc(sizeof(scr_line));
   scr->lvl = 0;
   scr->value[0] = '\0';
   scr->border = 0;
   scr->next = NULL;

   debug_3("init_screen()\n");
   return scr;

}

scr_line*
draw_box(scr_line* screen, char* value_1, char* value_2, int lvl){

      debug_3("draw_box() inicio ... \n");


   scr_line* last_line=NULL;
   scr_line* temp     =NULL;
   scr_line* ret      =NULL;

   int shift = 0;
   int length =0;

   ret = screen;

   /* get last line of screen */
   last_line =get_last_line(screen);
   /*
   if((lvl - last_line->lvl) > 1) {

      printf("Saut de plusieurs lvl non permis: lvl(%d)last(%d)\n",lvl,
                                                 last_line->lvl);
      exit(EXIT_FAILURE);
   }*/

   if(lvl < 0) {

      printf("Nao permitido lvl negativo : ultimo lvl (%d) lvl (%d)\n",
                                 last_line->lvl,lvl);
      exit(EXIT_FAILURE);
   }

   /* calculate shift                          */
   shift = lvl * SHIFT_LEN;
   /* get max length of values                 */
   length= max_str_len(value_1,value_2);

   debug_3("draw_box():last_line->lvl (%d) \n",last_line->lvl);
   debug_3("draw_box():lvl            (%d) \n",lvl);
   debug_3("draw_box():shift          (%d) \n",shift);
   debug_3("draw_box():max_str_len    (%d) \n",length);

   temp  = draw_border(length,shift);
   ret   = scr_append(ret   ,temp);
   temp  = draw_value (length,shift,value_1);
   ret   = scr_append(ret   ,temp);
   temp  = draw_value (length,shift,value_2);
   ret   = scr_append(ret   ,temp);
   temp  = draw_border(length,shift);
   ret   = scr_append(ret   ,temp);

   debug_3("draw_box() fim. \n");

   return ret   ;

}

scr_line*
get_last_line(scr_line* screen) {

   scr_line* temp=NULL;
   scr_line* ret =NULL;

   int i;

   temp = screen;
   ret  = temp;

   for(i=0;temp;i++){

      ret=temp;
      temp=temp->next;
   }

      debug_3 ("Proxima linha (%d th).\n",i);

   return ret;

}

scr_line*
scr_append(scr_line* screen,scr_line* to_append) {

   scr_line* ret =NULL;

   ret =get_last_line(screen);
   ret ->next = to_append;

   return screen;

}

int
max_str_len(char* val_a, char* val_b) {

   int a,b;
   a= strlen(val_a);
   b= strlen(val_b);
   debug_3("max_str_len(): a (%d) b (%d).\n",a,b);

   if(a>b)
     return a;
   else
     return b;
}

scr_line*
draw_border(int length, int shift) {

   scr_line* ret=(scr_line*) malloc(sizeof(scr_line));

   int lvl;
   int indent =4;
   char  temp[1024];

   temp[0]='\0';

   sprintf(temp,"%*s"  "%.*s"  "\0"
           ,shift,""                     /* build shift  */
           ,length+indent, dummy_border  /* build asterisk */
           );

   strcpy(ret->value,temp);
   debug_3("draw_border(): value = (%s)\n",ret->value);

   /* compute level */
   lvl = shift/SHIFT_LEN;
   ret->lvl    =lvl;
   ret->border =1;
   ret->next   =NULL;

   return ret;
}


scr_line*
draw_value (int length, int shift,char* value) {

   scr_line* ret=(scr_line*) malloc(sizeof(scr_line));

   int   lvl;
   int   indent =4;
   char  temp[1024];

   temp[0]='\0';

   sprintf(temp,"%*s"  "%.*s"  "%c"  "%s"  "%*s"  "%c"  "%.*s"  "\0"
           ,shift,""                     /* build shift      */
           ,1 , dummy_border             /* build asterisk 1 */
           ,' '                          /* indent 1         */
           ,value                        /* build value      */
           ,length-strlen(value),""      /* space pad to max */
           ,' '                          /* indent 2         */
           ,1 , dummy_border             /* build asterisk 2 */
           );

   strcpy(ret->value,temp);
   debug_3("draw_border() : value=(%s)\n",ret->value);

   /* compute level */
   lvl         =shift/SHIFT_LEN;
   ret->lvl    =lvl;
   ret->border =0;
   ret->next   =NULL;

   return ret;
}

int
print_boxes(scr_line* scr){

   debug_3("boxes ... \n");

   int x   =1;
   int y   =1;
   int i;
   scr_line* temp;


   /* skip first node */
   for(temp=scr,i=1;temp=temp->next;i++){

      printf("%.*s\n",79,temp->value);

   /* if (!(i%4))
       printf("\n");
   */
   }

   return 0;

}