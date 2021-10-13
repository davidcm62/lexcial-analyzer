#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "sintactico.h"
#include "lexico.h"
#include "definiciones.h"
#include "error.h"

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

void iniciarAnalisisSintactico(TS *tablaSimbolos,  SistemaEntrada *sistemaEntrada){
    bool analyze = true;
    
    while(analyze){
        CompLexico *compLexico = initCompLexico();
        LexicalResult result = seguinteCompLexico(tablaSimbolos, sistemaEntrada, compLexico);
        
        if(result == SUCCESS){
            printf("<%d, %s>\n", compLexico->compLexico, _escapeChars(compLexico->lexema));
            analyze = compLexico->compLexico != FIN_FICHEIRO;
        }else{
            handleErrorWithFileStats(LEXICAL, compLexico->lexema, *(sistemaEntrada->stats), sistemaEntrada->filename);
        }

        liberarCompLexico(compLexico);
    }
}