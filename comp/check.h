#ifndef CHECK_H_INCLUDED
#define CHECK_H_INCLUDED

#include "ast.h"

int chk_ast              (ast* tree);
int chk_field            (ast* tree);
int chk_display          (ast* tree);
int chk_initialize       (ast* tree);

/* Check for Remise aux norme */

int chk_initialize_ran   (ast* tree);

#endif
/* CHECK_H_INCLUDED */
