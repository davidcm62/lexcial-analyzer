#include <stdio.h>
#include <stdlib.h>
#include "TS.h"
#include "definiciones.h"

typedef struct {
    char lexema[20];
    int compLexico;
} Keyword;

TS* crearTS(){
    TS *ts = crearHashMap(TS_SIZE);

    Keyword keywords[] = {
        {"False", FALSE},
        {"await", AWAIT},
        {"else", ELSE},
        {"import", IMPORT},
        {"pass", PASS},
        {"None", NONE},
        {"break", BREAK},
        {"except", EXCEPT},
        {"in", IN},
        {"raise", RAISE},
        {"True", TRUE},
        {"class", CLASS},
        {"finally", FINALLY},
        {"is", IS},
        {"return", RETURN},
        {"and", AND},
        {"continue", CONTINUE},
        {"for", FOR},
        {"lambda", LAMBDA},
        {"try", TRY},
        {"as", AS},
        {"def", DEF},
        {"from", FROM},
        {"nonlocal", NONLOCAL},
        {"while", WHILE},
        {"assert", ASSERT},
        {"del", DEL},
        {"global", GLOBAL},
        {"not", NOT},
        {"with", WITH},
        {"async", ASYNC},
        {"elif", ELIF},
        {"if", IF},
        {"or", OR},
        {"yield", YIELD},
        {"print", PRINT},
    };

    for(size_t i=0; i<sizeof(keywords)/sizeof(Keyword); i++){
        insertarHashMap(ts, keywords[i].lexema,keywords[i].compLexico);
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