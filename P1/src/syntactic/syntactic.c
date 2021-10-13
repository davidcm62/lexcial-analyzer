#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "syntactic.h"
#include "../lexical/lexico.h"
#include "../common/definiciones.h"
#include "../error/error.h"

char* _escapeChars(const char *str) {
    int i, j;
    char *newStr;

    for (i = j = 0; i < strlen(str); i++) {
        if (str[i] == '\n' || str[i] == '\t') {
            j++;
        }
    }
    newStr = malloc(i + j + 1);

    for (i = j = 0; i < strlen(str); i++) {
        switch (str[i]) {
            case '\n': 
                newStr[i+j] = '\\'; 
                newStr[i+j+1] = 'n'; 
                j++; 
                break;
            case '\t': 
                newStr[i+j] = '\\'; 
                newStr[i+j+1] = 't'; 
                j++; 
                break;
            default:
                newStr[i+j] = str[i];
                break;
        }
    }
    newStr[i+j] = '\0';
    return newStr;
}

void startSyntacticAnalysis(TS *ts,  SistemaEntrada *inputSystem){
    bool analyze = true;
    
    while(analyze){
        CompLexico *lexicalComp = initCompLexico();
        LexicalResult result = seguinteCompLexico(ts, inputSystem, lexicalComp);
        

        if(result == SUCCESS){
            //componente lexico correcto
            printf("<%d, %s>\n", lexicalComp->compLexico, _escapeChars(lexicalComp->lexema));
            analyze = lexicalComp->compLexico != FIN_FICHEIRO;
        }else{
            //componente lexio mal formado
            handleErrorWithFileStats(LEXICAL, lexicalComp->lexema, *(inputSystem->stats), inputSystem->filename);
        }

        liberarCompLexico(lexicalComp);
    }
}