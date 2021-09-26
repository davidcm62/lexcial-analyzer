#ifndef TS_H
#define TS_H

#include "hashmap.h"

typedef HashMap TS;

TS* crearTS(long initialSize);
void liberarTS(TS *tablaSimbolos);
void imprimirTS(TS tablaSimbolos);
void insertarTS(TS *tablaSimbolos, char *key, int value);
int* buscarTS(TS tablaSimbolos, char *key);

#endif /* TS_H */