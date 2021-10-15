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

/**
 * Inicializa a estructura da tabla de símbolos
 */
TS* initTS();

/**
 * Libera os recursos da tabla de símbolos
 */
void freeTS(TS *symbolTable);


/**
 * Imprime a TS
 */
void printTS(TS symbolTable);


/**
 * Inserta unha nova entrada na TS
 */
void insertTS(TS *symbolTable, char *key, int value);


/**
 * Busca na TS
 * devolve NULL se non se atopa a entrada con lexema "key"
 */
int* searchTS(TS symbolTable, char *key);

#endif /* TS_H */