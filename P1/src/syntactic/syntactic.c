#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "syntactic.h"
#include "../lexical/lexical.h"
#include "../common/definiciones.h"
#include "../error/error.h"

char* _escapeChars(const char *str) {
    //reemplaza \n por \\n e \t por \\t
    int i, j;
    char *newStr;

    for (i = j = 0; i < strlen(str); i++) {
        if (str[i] == '\n' || str[i] == '\t') j++;
    }
    newStr = malloc(i + j + 1);

    for (i = j = 0; i < strlen(str); i++) {
        if(str[i] == '\n' || str[i] == '\t'){
                newStr[i+j] = '\\';
                newStr[i+j+1] = str[i] == '\n'? 'n':'t'; 
                j++; 
        }else{
            newStr[i+j] = str[i];
        }
    }
    newStr[i+j] = '\0';
    return newStr;
}

void startSyntacticAnalysis(TS *ts,  InputSystem *inputSystem){
    bool analyze = true;
    
    while(analyze){
        LexicalComponent *lexicalComponent = initLexicalComponent();
        LexicalResult result = nextLexicalComponent(ts, inputSystem, lexicalComponent);
        

        if(result == SUCCESS){
            //componente lexico correcto
            printf("<%d, %s>\n", lexicalComponent->lexicalComp, _escapeChars(lexicalComponent->lexeme));
            analyze = lexicalComponent->lexicalComp != FIN_FICHEIRO;
        }else{
            //componente lexio mal formado
            handleErrorWithFileStats(LEXICAL, lexicalComponent->lexeme, inputSystem->filename, inputSystem->stats->line);
        }

        freeLexicalComponent(lexicalComponent);
    }
}