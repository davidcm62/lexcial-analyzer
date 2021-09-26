#include <stdio.h>
#include <stdlib.h>
#include "TS.h"

TS* crearTS(long initialSize){
    TS *ts = crearHashMap(initialSize);

    insertarHashMap(ts, "populating table", 123456789);

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