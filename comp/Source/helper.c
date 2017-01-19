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
   scr->lvl       =0;
   scr->value[0]  ='\0';
   scr->border    =0;
   scr->next      =NULL;

      debug_3("init_screen() succesfully.\n");
   return scr;

}

scr_line*
draw_box(scr_line* screen, char* value_1, char* value_2, int lvl){

      debug_3("draw_box() start ... \n");


   scr_line* last_line=NULL;
   scr_line* temp     =NULL;
   scr_line* ret      =NULL;

   int shift          =0;
   int length         =0;

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

      printf("Negative lvl not allowed : last lvl (%d) lvl (%d)\n",
                                 last_line->lvl,lvl);
      exit(EXIT_FAILURE);
   }

   /* calculate shift                          */
   shift = lvl * SHIFT_LEN;
   /* get max length of values                 */
   length= max_str_len(value_1,value_2);

   debug_3("draw_box():last_line->lvl (%d) \n",last_line->lvl);
   debug_3("draw_box():box to draw lvl (%d) \n",lvl);
   debug_3("draw_box():shift          (%d) \n",shift);
   debug_3("draw_box():max_str_len    (%d) \n",length);


   /* draw first border                        */
   temp  = draw_border(length,shift);
   /* append to screen                         */
   ret   = scr_append(ret   ,temp);

   /* draw value 1                             */
   temp  = draw_value (length,shift,value_1);
   /* append to screen                         */
   ret   = scr_append(ret   ,temp);

   /* draw value 2                             */
   temp  = draw_value (length,shift,value_2);
   /* append to screen                         */
   ret   = scr_append(ret   ,temp);

   /* draw last  border                        */
   temp  = draw_border(length,shift);
   /* append to screen                         */
   ret   = scr_append(ret   ,temp);

   debug_3("draw_box() succesfully. \n");

   return ret   ;

}

scr_line*
get_last_line(scr_line* screen) {

   scr_line* temp=NULL;
   scr_line* ret =NULL;

   int i;

   temp = screen;
   ret  = temp;

      debug_3 ("start.\n");

   for(i=0;temp;i++){

      debug_3 ("screen-%d : (%d):%s\n",
                               i,temp,temp->value);
      ret=temp;
      temp=temp->next;
   }

      debug_3 ("last line is the (%d th) next.\n",i);

   return ret;

}

scr_line*
scr_append(scr_line* screen,scr_line* to_append) {

   scr_line* ret =NULL;

   ret =get_last_line(screen);
   ret ->next = to_append;

   debug_3("succesful        .\n");
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


   /*
   strcat(buffer,temp);
   printf("temp_len = (%d)\n",temp_len);
   printf("draw_border(shift): temp=(%.*s)\n",temp_len,buffer);


   temp=h_realloc(temp,temp_len+length+indent);
   sprintf(temp+temp_len,"%.*s", length+indent, dummy_border);
   temp_len+=(length+indent);

   sprintf(temp,"%*s",length+indent, dummy_border);
   strcat(buffer,temp);
   printf("temp_len = (%d)\n",temp_len);
   printf("draw_border(***)  : temp=(%.*s)\n",temp_len,buffer);

   temp=h_realloc(temp,temp_len+1);
   sprintf(temp+temp_len,"%c", '\0');
   temp_len+=1;
   printf("temp_len = (%d)\n",temp_len);
   printf("draw_border(\\0)   : temp=(%.*s)\n",temp_len,temp);
   */

   strcpy(ret->value,temp);
   debug_3("draw_border()   : value=(%s)\n",ret->value);

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


   /*
   temp=h_realloc(temp,temp_len+shift);
   sprintf(temp+temp_len,"%*s", shift, " ");
   temp_len+=shift;

   printf("temp_len = (%d)\n",temp_len);
   printf("draw_value (shift): temp=(%.*s)\n",temp_len,temp);

   temp=h_realloc(temp,temp_len+1);
   sprintf(temp+temp_len,"%.*s", 1,dummy_border);
   temp_len+=1;


   printf("temp_len = (%d)\n",temp_len);
   printf("draw_value (*)    : temp=(%.*s)\n",temp_len,temp);

   temp=h_realloc(temp,temp_len+1);
   sprintf(temp+temp_len,"%c", ' ');
   temp_len+=1;

   printf("temp_len = (%d)\n",temp_len);
   printf("draw_value ( )    : temp=(%.*s)\n",temp_len,temp);

   temp=h_realloc(temp,temp_len+strlen(value));
   strcpy(temp+temp_len,value);
   temp_len+=strlen(value);

   printf("temp_len = (%d)\n",temp_len);
   printf("draw_value (value): temp=(%.*s)\n",temp_len,temp);

   if(strlen(value)<length){
      int diff=0;
      diff = length-strlen(value);
      printf("padding difference between values length (%d)\n",diff);

      temp=h_realloc(temp,temp_len+diff);
      sprintf(temp+temp_len,"%*s", diff , " ");
      temp_len+=diff;

      printf("temp_len = (%d)\n",temp_len);
      printf("draw_value (padd):temp=(%.*s)\n",temp_len,temp);

   }

   temp=h_realloc(temp,temp_len+1);
   sprintf(temp+temp_len,"%c", ' ');
   temp_len+=1;

   printf("temp_len = (%d)\n",temp_len);
   printf("draw_value ( ): temp=(%.*s)\n",temp_len,temp);

   temp=h_realloc(temp,temp_len+1);
   sprintf(temp+temp_len,"%.*s", 1,dummy_border);
   temp_len+=1;


   printf("temp_len = (%d)\n",temp_len);
   printf("draw_value (*): temp=(%.*s)\n",temp_len,temp);

   temp=h_realloc(temp,temp_len+1);
   sprintf(temp+temp_len,"%c", '\0');
   temp_len+=1;

   printf("temp_len = (%d)\n",temp_len);
   printf("draw_value (\0): temp=(%.*s)\n",temp_len,temp);
   */

   strcpy(ret->value,temp);
   debug_3("draw_border() : value=(%s)\n",ret->value);

   /* compute level */
   lvl         =shift/SHIFT_LEN;
   ret->lvl    =lvl;
   ret->border =0;
   ret->next   =NULL;

   return ret;
}

scr_line*
draw_blank (){
   /* inutile, faire un double printf("\n") suffit */
   scr_line* ret=(scr_line*) malloc(sizeof(scr_line));

   ret->lvl          = 0;
   sprintf(ret->value,"%0*s", 1024 , " ");
   ret->value[1023]  ='\0';
   ret->border       =0;
   ret->next         =NULL;

   return ret;

}

int
print_boxes(scr_line* scr){

   debug_3("printing screen boxes ... \n");

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

char*
h_realloc(char* temp,int size){

   debug_3("\n\n");
   debug_3("h_realloc start. size (%d) \n",size);
   debug_3("h_realloc start. temp (%d) \n",temp);

   char* ret;

   debug_3("ret before (%d)\n",ret);
   ret=(char*) realloc(temp,(size) * sizeof(char));

   debug_3("ret after (%d)\n",ret);
   debug_3("\n");

   if((ret==NULL) && (size>0)) {
     printf("Realloc Failed ! \n");
     exit(EXIT_FAILURE);
   }
   return ret;

}

