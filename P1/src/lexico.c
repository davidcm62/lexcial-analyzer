#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"
#include "definiciones.h"

CompLexico* seguinteCompLexico(TS *tablaSimbolos){
    // CompLexico *compLexico = (CompLexico*)malloc(sizeof(CompLexico));
    // compLexico->lexema = (char *)malloc(7 * sizeof(char));
    // strcpy(compLexico->lexema, "lexema");
    // compLexico->compLexico = _IDENTIFICADOR;
    
    return NULL;
}

void liberarCompLexico(CompLexico *compLexico){
    if(compLexico != NULL){
        free(compLexico->lexema);
        free(compLexico);
    }
}