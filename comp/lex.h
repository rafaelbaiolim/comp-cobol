

void inicializa_analise(char *prog)
{
    codigo = prog;
    tamanho = strlen(codigo);
    pos = 0;
}

char le_caractere(){
    char c = codigo[pos];
    pos++;
    return c;
}

int operador(char c){
    switch (c){
        case '+': return SOMA; 
        case '/': return DIV;
        case '*': return MULT;
        case '-': return SUB;
        default : return '\0';
    }
}

 Token* proximo_token(Token* tok)
{
    char c;
    char valor[200];
    // string para obter valor de um numero
    int vpos = 0;
    // posicao na string de valor
    c = le_caractere();
    
    // pula todos os espacos em branco
    while(isspace(c)) {
        c = le_caractere();
    }
    if(isdigit(c)) {
        tok->tipo = TOK_NUM;
        valor[vpos++] = c;
        c = le_caractere();
        while(isdigit(c)) {
            valor[vpos++] = c;
            c = le_caractere();
        }
        // retorna o primeiro caractere que nao eh um digito
        // para ser lido como parte do proximo token
        pos--;
        // termina a string de valor com um caractere 0
        valor[vpos] = '\0';
        // converte string de valor para numero
        tok->valor = atoi(valor);
        
    } else if(strchr(ops, c) != NULL && c != '\0') {
        tok->tipo = TOK_OP;
        tok->valor = operador(c);
    } else if(c == '(' || c == ')') {
        
        tok->tipo = TOK_PONT;
        tok->valor = (c == '(' ? PARESQ : PARDIR);
    } else{
        return NULL;
    }
    return tok;
}
