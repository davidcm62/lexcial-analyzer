#ifndef TS_H
#define TS_H

/**
 * Inicializa a estructura da tabla de símbolos
 */
void initTS();

/**
 * Libera os recursos da tabla de símbolos
 */
void freeTS();


/**
 * Imprime a TS
 */
void printTS();


/**
 * Inserta unha nova entrada na TS
 */
void insertTS(char *key, int value);


/**
 * Busca na TS
 * devolve NULL se non se atopa a entrada con lexema "key"
 */
int* searchTS(char *key);

#endif /* TS_H */