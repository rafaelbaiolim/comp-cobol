#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void lerArquivo();
void montarMatrizProducao();
void executarFirst();
void executarFollow();
void executarTabelaParser();
void addArrayResultado(char[], char);
void first(char* pResultado, char pLetra, int pIndiceFollow);
void follow(char[], char);
void tabelaParser(char* pResultado, char pSimbolo, char pTerminal);

FILE *arquivo;
FILE *arquivoSaida;
int nroProducao;
char matrizProducao[100][100];
char arrayProducao[50];

int main()
{
    lerArquivo();

    executarFirst();
    executarFollow();
    executarTabelaParser();

	fclose(arquivo);
}

void lerArquivo(){
	arquivo = fopen("Entrada.txt", "r");
	arquivoSaida = fopen("Saida.txt", "w");

    if(arquivo == NULL)
    {
		printf("Erro: Nao foi possivel abrir o arquivo!\n");
		exit(0);
	}
	else
	{
		montarMatrizProducao();
	}
	
	printf("Arquivo 'Saida' criado com sucesso na pasta do projeto!\n\n");
}

void montarMatrizProducao(){
	char producaoAUX[100][100];
	char auxExpressao[2];
	char auxProducao[50];
	char* ptr;
    int j, index;
    char *token;

	nroProducao = 0;

	while (fscanf(arquivo, "%s", producaoAUX[nroProducao]) == 1)
	{
		//Ira montar a matrizProducao e verifica se existe o delimitador "|" nas producoes
		ptr = strchr(producaoAUX[nroProducao], '|');
	    if (ptr != NULL)
	    {
	    	//Inicializa os arrays auxiliares
	    	memset(&auxExpressao[0], 0, sizeof(auxExpressao));
	    	memset(&auxProducao[0], 0, sizeof(auxProducao));

	    	strncpy(auxExpressao, producaoAUX[nroProducao],3);
            auxExpressao[strlen(auxExpressao)-1] = '\0';
			strncat(auxProducao, &producaoAUX[nroProducao][2], strlen(&producaoAUX[nroProducao][2]));

			token = strtok(auxProducao, "|");
	    	while (token != NULL)
	    	{
	    		strcat(matrizProducao[nroProducao], auxExpressao);
	    		strcat(matrizProducao[nroProducao], token);

	    		fprintf(arquivoSaida,"Producao %d : ", nroProducao+1);
				fprintf(arquivoSaida,"%s\n", matrizProducao[nroProducao]);
				nroProducao++;

	    		token = strtok(NULL, "|");
			}
	    }
		else
		{
			strncpy(matrizProducao[nroProducao],producaoAUX[nroProducao],100);

			fprintf(arquivoSaida,"Producao %d : ", nroProducao+1);
			fprintf(arquivoSaida,"%s\n", matrizProducao[nroProducao]);
			nroProducao++;
		}
	}
}

void executarFirst(){
	int i, j;
    char c;
    char resultado[50];
    char arrayResultado[50];

    memset(&resultado[0], 0, sizeof(resultado));
    memset(&arrayResultado[0], 0, sizeof(arrayResultado));
    
    fprintf(arquivoSaida,"--------------------------------------------\n");

	for (i = 0; i < nroProducao; i++)
    {
    	c = matrizProducao[i][0];
		addArrayResultado(arrayResultado, c);
	}

	for (i = 0; i < strlen(arrayResultado); i++)
    {
    	c = arrayResultado[i];
        first(resultado, c, -1);
		fprintf(arquivoSaida,"FIRST(%c) = { ", c);
        for (j=0; resultado[j] != '\0'; j++)
        	fprintf(arquivoSaida," %c ", resultado[j]);
        fprintf(arquivoSaida,"}\n");
    }
}

void executarFollow(){
	int i, j;
    char c;
    char resultado[50];
    char arrayResultado[50];

    memset(&resultado[0], 0, sizeof(resultado));
    memset(&arrayResultado[0], 0, sizeof(arrayResultado));
    
    fprintf(arquivoSaida,"--------------------------------------------\n");

	for (i = 0; i < nroProducao; i++)
    {
    	c = matrizProducao[i][0];
		addArrayResultado(arrayResultado, c);
	}

	for	(i = 0; i < strlen(arrayResultado); i++)
	{
		c = arrayResultado[i];
        follow(resultado, c);
		fprintf(arquivoSaida,"FOLLOW(%c) = { ", c);
        for (j=0; resultado[j] != '\0'; j++)
        	fprintf(arquivoSaida," %c ", resultado[j]);
        fprintf(arquivoSaida,"}\n");
    }
}

void executarTabelaParser(){
	int i, j, k;
	int encontrouVazio;
    char simbolo, terminal;
    char resultado[50];    
    char arraySimbolo[50];
    char arrayTerminal[50];

    memset(&resultado[0], 0, sizeof(resultado));
    memset(&arraySimbolo[0], 0, sizeof(arraySimbolo));
    memset(&arrayTerminal[0], 0, sizeof(arrayTerminal));
    
    fprintf(arquivoSaida,"--------------------------------------------\n");

	for (i = 0; i < nroProducao; i++)
    {
    	simbolo = matrizProducao[i][0];
		addArrayResultado(arraySimbolo, simbolo);
		
		for (j = 2; j < strlen(matrizProducao[i]); j++)
		{
			terminal = matrizProducao[i][j];
			if(!(isupper(terminal)))
				addArrayResultado(arrayTerminal, terminal);
		}
	}
	
	addArrayResultado(arrayTerminal, '$');
	/*
	for (i = 0; i < strlen(arraySimbolo); i++)
    {
    	simbolo = arraySimbolo[i];
    	for (j = 0; j < strlen(arrayTerminal); j++)
   		{
   			terminal = arrayTerminal[j];
   			
   			if (terminal != '$'){
	   			tabelaParser(resultado, simbolo, terminal);
	   			
	   			fprintf(arquivoSaida,"TABELA[%c,%c] = ", simbolo, terminal);
		        for(k=0; resultado[k] != '\0'; k++)
		        	fprintf(arquivoSaida," %c", resultado[k]);
	        	fprintf(arquivoSaida,"\n");
	    	}
   		}
    }
    */
    
    for(i = 0; i < strlen(arraySimbolo); i++)
    {
    	encontrouVazio = 0;
    	
    	simbolo = arraySimbolo[i];
		first(resultado, simbolo, -1);
    	
    	for(j = 0; j < strlen(resultado); j++)
		{	
			terminal = resultado[j];
			
			if (terminal != 'E') 
			{
				fprintf(arquivoSaida,"TABELA[%c,%c] = ", simbolo, terminal);
				for (k = 2; k < strlen(matrizProducao[i]); k++)
		        	fprintf(arquivoSaida," %c", matrizProducao[i][k]);
	        	fprintf(arquivoSaida,"\n");
	    	}	
	    	else
	    		encontrouVazio = 1;
		}   
		
		if (encontrouVazio) 
		{
			follow(resultado, simbolo);
			
			for(j = 0; j < strlen(resultado); j++)
			{	
				terminal = resultado[j];
				
				if (terminal != 'E') 
				{
					fprintf(arquivoSaida,"TABELA[%c,%c] = ", simbolo, terminal);
					for (k = 2; k < strlen(matrizProducao[i]); k++)
			        	fprintf(arquivoSaida," %c", matrizProducao[i][k]);
		        	fprintf(arquivoSaida,"\n");
		    	}	
		    	else
		    		encontrouVazio = 1;
			} 
		}
    }
}

void addArrayResultado(char pResultado[], char pTerminal)
{
    int i;

    for(i=0 ; pResultado[i] != '\0'; i++)
        if(pResultado[i] == pTerminal)
            return;

    pResultado[i] = pTerminal;
    pResultado[i+1] = '\0';
}

void first(char* pResultado, char pLetra, int pIndiceFollow)
{
    int i, j, k;
    char subResultado[50];
    int econtrouVazio;

    memset(&pResultado[0], 0, sizeof(pResultado));
    memset(&subResultado[0], 0, sizeof(subResultado));

    //Verifica se eh um terminal
    if(!(isupper(pLetra)))
    {
        addArrayResultado(pResultado, pLetra);
        return ;
    }

	//Se n?o ? um terminal verifica as producoes
    for(i = 0; i < nroProducao; i++)
    {
	    if(matrizProducao[i][0] == pLetra)
	    {
			if(matrizProducao[i][2] == 'E' || matrizProducao[i][2] == 'e')
			{
				if (pIndiceFollow = -1)
					addArrayResultado(pResultado, 'E');
				else
					follow(pResultado, matrizProducao[pIndiceFollow][0]);
			}
	 		else
            {
            	j=2;
                while(matrizProducao[i][j] !='\0')
                {
                	econtrouVazio = 0;

                	first(subResultado, matrizProducao[i][j], -1);

                 	for(k=0; subResultado[k] != '\0'; k++)
                 	{
                     	if(subResultado[k] == 'E' || subResultado[k] == 'e')
                     	{
                        	econtrouVazio = 1;
                         	break;
                     	}
						else
							addArrayResultado(pResultado, subResultado[k]);
					}

                 	if(!econtrouVazio)
                    	break;

                 	j++;
                }

                if (econtrouVazio)
                {
                	addArrayResultado(pResultado, 'E');
				}
            }
	    }
	}
    return ;
}

void follow(char* pResultado, char pLetra)
{
	int i, j, k;
	char subResultado[50];

    memset(&pResultado[0], 0, sizeof(pResultado));
    memset(&subResultado[0], 0, sizeof(subResultado));

	if(matrizProducao[0][0] == pLetra)
		addArrayResultado(pResultado, '$');

	for(i = 0; i < nroProducao; i++)
	{
		for(j = 2; j < strlen(matrizProducao[i]); j++)
		{
		    if(matrizProducao[i][j] == pLetra)
			{
			    if(matrizProducao[i][j+1] != '\0')
			    {
					first(subResultado, matrizProducao[i][j+1], i);

				    for(k=0; subResultado[k] != '\0'; k++)
						addArrayResultado(pResultado, subResultado[k]);
				}

				if(matrizProducao[i][j+1] == '\0' && pLetra != matrizProducao[i][0])
				{
			    	follow(subResultado, matrizProducao[i][0]);

				    for(k=0; subResultado[k] != '\0'; k++)
						addArrayResultado(pResultado, subResultado[k]);
				}
		    }
		}
	}
}

void tabelaParser(char* pResultado, char pSimbolo, char pTerminal)
{
	char resultado[50];
	
	memset(&pResultado[0], 0, sizeof(pResultado));
	
	first(resultado, pSimbolo, -1);
	/*	
	if (encontrouVazio(resultado))
	{
		follow(resultado, pSimbolo);
	}
	
	if (encontrouFim(resultado)){
		//M[pSimbolo,'$'] = producao
	}
	*/
}
