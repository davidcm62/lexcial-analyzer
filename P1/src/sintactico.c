#include <stdio.h>
#include <stdbool.h>
#include "sintactico.h"
#include "lexico.h"


void iniciarAnalisisSintactico(TS *tablaSimbolos){
    CompLexico *compLexico = NULL;

    while(true){
        compLexico = seguinteCompLexico(tablaSimbolos);
        
        if(compLexico == NULL){
            return;
        }
        
        printf("<%d, %s>\n", compLexico->compLexico, compLexico->lexema);

        liberarCompLexico(compLexico);
    }
}