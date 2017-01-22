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

int argc;
int cursl;
int cursc;
char src_file[DS_NAME_LEN];
char inc_pds[NB_INC_PDS][DS_NAME_LEN];
char options[NB_OPTIONS][DS_NAME_LEN];
int main_ret;
_editLine result[NB_RESULTS];

// int        curr_ln_num;
_srcLine* src_list;
_srcLine* curr_src_line;

/* bug connu : rawcode depasse le code reel par 14 car à la fin */
int getArgsRexx()
{

    curr_src_line = getSource("teste.cbl");
}

void setCurrLnStart()
{

    curr_src_line = src_list;
}

int break_str_space(char* line, char words[][DS_NAME_LEN], int maxWords)
{

    char* p = line;
    char* buff[DS_NAME_LEN];
    int nbWords = 0;
    int i;

    while(1) {
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

    for(i = 0; i <= nbWords; i++) {
        strcpy(words[i], buff[i]);
    }

    return nbWords;
}
/**
 * @brief Cria a struct do arquivo
 * aninhando as linhas para struct p->next
 * @param fileName
 * @return
 */
_srcLine* getSource(char* fileName)
{

    FILE* fp;
    _srcLine* p;
    _srcLine* l;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(fileName, "r");
    if(fp == NULL) {
        exit(0);
    }
    p = NULL;
    l = NULL;
    int i = 1;
    while((read = getline(&line, &len, fp)) != EOF) {
        if(!p) {
            p = malloc(sizeof(_srcLine));
            l = p;
        } else {
            p->next = malloc(sizeof(_srcLine));
            p = p->next;
        }
        p->ln_num = i;
        strncpy(p->value, line, 1000);
        p->next = NULL;
        i++;
    }
    p = l->next;

    fclose(fp);
    if(line) {
        free(line);
    }
    return l;
}

_srcLine* getCurrSrcLine(_srcLine* srcList, int cursl, int cursc)
{

    _srcLine* l;
    int i;

    debug_3("srcList (%d)\n", srcList);
    debug_3("cursl   (%d)\n", cursl);
    assert(srcList);

    l = srcList;

    for(i = 1; l; i++) {
        //  printf("l (%s)\n",l->value);
        if(i == cursl)
            break;
        l = l->next;
    }

    if(!l) {

        printf("current srcLine cant be found from cursor\n");
        exit(EXIT_FAILURE);
    }

    debug_3("l       (%d)\n", l);
    return l;
}
