#ifndef TS_H
#define TS_H

#include "hashmap.h"

/* 
    Numero primo mas pequeno para que
    collan minimo todas as keywords mantendo
    factor de carga <= 0.75. Asi evito ter
    que redimensionar ao principio xa.
*/
#define TS_SIZE 53

typedef HashMap TS;

TS* crearTS();
void liberarTS(TS *tablaSimbolos);
void imprimirTS(TS tablaSimbolos);
void insertarTS(TS *tablaSimbolos, char *key, int value);
int* buscarTS(TS tablaSimbolos, char *key);

#endif /* TS_H */