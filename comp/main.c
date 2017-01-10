#include <string.h>
#include <ctype.h>
#include "constantes.h"
#include "cab.h"
#include "lex.h"


char imprime_token(Token* tok){
    printf("\n%i\n",tok->valor);
}

int main(int argc, char** argv)
{
    char c;
    file = fopen("test.txt", "r");

    char entrada[200];
    Token tok;
    printf("Analise Lexica para Expressoes\n");
    printf("Expressao: ");
    fgets(entrada, 200, file);
 
    inicializa_analise(entrada);
//    printf("\n===== Analise =====\n");
    while(proximo_token(&tok) != NULL) {
        imprime_token(&tok);
    }
    
    
    
//    printf("\n");
//    return 0;
//
//    printf("TOKEN %i : %s\n", 1, getToken());
//
//    printf("TOKEN %i : %s\n", 2, getToken());
//
//    puts("\n**Suposto erro ocorreu\n");
//    fseek(file, crrFilePointer, SEEK_END);
//    printf("TOKEN %i : %s\n", 3, getToken());
//
//    fclose(file);
//    return 0;
}
