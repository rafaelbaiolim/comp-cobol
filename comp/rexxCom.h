#ifndef REXXCOM_H_INCLUDED
#define REXXCOM_H_INCLUDED

#define DS_NAME_LEN 45 
#define NB_INC_PDS  20
#define NB_OPTIONS  20
#define NB_RESULTS  500

#if defined(__IBMC__)
	#pragma runopts(PLIST(OS))
#else
void **__osplist;
#endif

int getArgsRexx();
int setCurrLnNum(int);
struct _srcLine* getSource(char* src_file);
struct _srcLine* getCurrSrcLine(struct _srcLine* src_list,int cursl,
                                                          int cursc);
int break_str_space(char *line,char words[][DS_NAME_LEN],int maxWords);
void setCurrLnStart();

typedef struct _editLine {

  char   funct;     // 1  char
  char*  line_num;  // 6  char
  char*  coln_num;  // 6  char
  char*  line_nbr;  // 6  char
  char*  src_line;  // 81 char

} _editLine;

typedef struct _srcLine {
  char   value[1000];
  int    num_file;
  int    num_all;
  int    ln_num;
  char   file[DS_NAME_LEN];
  struct _srcLine *next;
} _srcLine;

extern int argc;
extern int cursl;
extern int cursc;
extern char src_file[DS_NAME_LEN];
extern char inc_pds[NB_INC_PDS][DS_NAME_LEN];
extern char options[NB_OPTIONS][DS_NAME_LEN];
extern int main_ret;
extern _editLine result[NB_RESULTS];
extern _srcLine *src_list;
extern _srcLine *curr_src_line;
#endif
