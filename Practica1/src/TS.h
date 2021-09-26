#ifndef TS_H
#define TS_H

#include "hashmap.h"

#define TS_SIZE 7

typedef HashMap TS;

TS* crearTS();
void liberarTS(TS *tablaSimbolos);
void imprimirTS(TS tablaSimbolos);
void insertarTS(TS *tablaSimbolos, char *key, int value);
int* buscarTS(TS tablaSimbolos, char *key);

#endif /* TS_H */