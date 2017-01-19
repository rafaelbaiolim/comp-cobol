#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "debug.h"
#include "generate.h"
#include "ast.h"

int
gen_display_ran (ast* tree){

    return 0;
}

int
gen_initialize_ran (ast* tree){

   static int nb_loop = 0;

   nb_loop++;

   if(nb_loop > 100) {

      printf("nb_loop excessif (%d) \n",nb_loop);
      exit(EXIT_FAILURE);
   }

   /* printf("tree (%d) tree.child (%d) tree->next (%d) name (%s) \n",
           tree,tree->node.field.children, tree->next,
           tree->node.field.name->node.ident_name.value);
   */

   assert(tree->tag==FIELD);

   ast* picture = tree->node.field.picture;

   if(picture){

      ast* pic_cmpnt = picture;

      if(pic_cmpnt->node.pic_cmpnt.pic_cmpnt==PIC_CHARS){

         debug_3(" pic_chars (%s) \n",
                   pic_cmpnt->node.pic_cmpnt.pic_val.chars);

         char id_name   [120];
         char id_qualif [120]="";

         strcpy (id_name,tree->node.field.name->node.ident_name.value);

         if((*(pic_cmpnt->node.pic_cmpnt.pic_val.chars)=='9')  ||
            (*(pic_cmpnt->node.pic_cmpnt.pic_val.chars)=='S')) {

            printf ("MOVE ZERO  TO %s \n",id_name);

         } else {

            printf ("MOVE SPACE TO %s \n",id_name);
         }

      } else {
         printf("Picture dont begin with a pic chars \n");
         exit(EXIT_FAILURE);
      }
   }

   ast* p =NULL;

   if(p=tree->node.field.children){

      gen_initialize_ran(p);

   }

   if(p=tree->next){

      gen_initialize_ran(p);

   }

   return 0;
}


int
gen_display_zone   (ast* tree){

   /* printf("tree (%d) tree.child (%d) tree->next (%d) name (%s) \n",
           tree,tree->node.field.children, tree->next,
           tree->node.field.name->node.ident_name.value);
   */

   assert(tree->tag==FIELD);

   ast* picture = tree->node.field.picture;

   if(picture){

      ast* pic_cmpnt = picture;

      if(pic_cmpnt->node.pic_cmpnt.pic_cmpnt==PIC_CHARS){

         debug_3(" pic_chars (%s) \n",
                   pic_cmpnt->node.pic_cmpnt.pic_val.chars);

         char id_name   [120];
         char indent    [12]="140871D    ";
         char id_qualif [120]="";

         strcpy (id_name,FLD_NAME(tree));
         printf ("%sDISPLAY '* %s : ' %s\n",indent,id_name,id_name);

      } else {
         printf("Picture dont begin with a pic chars \n");
         exit(EXIT_FAILURE);
      }
   }

   ast* p =NULL;

   if(p=tree->node.field.children){

      gen_display_zone    (p);

   }

   if(p=tree->next){

      gen_display_zone    (p);

   }

   return 0;
}
