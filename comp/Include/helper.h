#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#define SHIFT_LEN 3

/*
 * Helper file for displaying the abstract syntax tree. 
 * if the level =0  the screen is empty
 * first level is 1
 * each level shifts 5x
 * between two box 2y
 */


typedef struct scr_line {

  char   value[1024];
  int    lvl;
  int    border;
  struct scr_line* next;

} scr_line;


scr_line*
init_screen();

scr_line*
draw_box(scr_line* _screen, char* value_1, char* value_2, int lvl);

scr_line*
get_last_line(scr_line* _screen);

scr_line*
scr_append(scr_line* _screen,scr_line* to_append);

int
max_str_len(char* val_a, char* val_b);

scr_line*
draw_border(int length, int shift);

scr_line*
draw_value (int length, int shift,char* value);

scr_line*
draw_blank ();

int
print_boxes(scr_line* scr);

char*
h_realloc(char* temp,int size);

#endif
/* HELPER_H_INCLUDED */
