#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "sintactico.h"
#include "lexico.h"
#include "definiciones.h"

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
    CompLexico *compLexico = NULL;
    bool analyze = true;
    
    while(analyze){
        compLexico = seguinteCompLexico(tablaSimbolos, sistemaEntrada);
        
        if(compLexico == NULL){
            return;
        }
        
        printf("<%d, %s>\n", compLexico->compLexico, _escapeChars(compLexico->lexema));

        if(compLexico->compLexico == FIN_FICHEIRO){
            analyze = false;
        }

        liberarCompLexico(compLexico);
    }
}