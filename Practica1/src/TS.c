#include <stdio.h>
#include <stdlib.h>
#include "TS.h"

TS* crearTS(){
    TS *ts = crearHashMap(TS_SIZE);

    //max 15 char
    char keywords[][15] = {
        "False",    "await",    "else",     "import",   "pass",
        "None",     "break",    "except",   "in",       "raise",
        "True",     "class",    "finally",  "is",       "return",
        "and",      "continue", "for",      "lambda",   "try",
        "as",       "def",      "from",     "nonlocal", "while",
        "assert",   "del",      "global",   "not",      "with",
        "async",    "elif",     "if",       "or",       "yield"
    };

    for(size_t i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++){
        insertarHashMap(ts, keywords[i], i+301);
    }

    return ts;
}

void liberarTS(TS *tablaSimbolos){
    liberarHashMap(tablaSimbolos);
}

void imprimirTS(TS tablaSimbolos){
    printf("==Tabla de Simbolos:==\n");
    imprimirHashMap(tablaSimbolos);
}

void insertarTS(TS *tablaSimbolos, char *key, int value){
    insertarHashMap(tablaSimbolos, key, value);
}

int* buscarTS(TS tablaSimbolos, char *key){
    return buscarHashMap(tablaSimbolos, key);
}