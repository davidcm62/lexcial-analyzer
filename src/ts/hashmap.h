#ifndef HASHMAP_H
#define HASHMAP_H

#define LOAD_FACTOR 0.75

typedef struct Node {
    char *key;
    int value;
    struct Node *nextNode;
} BucketNode;

typedef struct {
    BucketNode *firstNode;
} Bucket;

typedef struct {
    Bucket *buckets;
    unsigned long hashMapSize;
    unsigned long totalItems;
} HashMap;


/**
 * Inicializa un hashmap de tamaño initialSize
 */
HashMap* initHashMap(unsigned long initialSize);


/**
 * Libera os recursos do hashmap
 */
void freeHashMap(HashMap *hashMap);

/**
 * Imprmie o hashmap
 */
void printHashMap(HashMap hashMap);

/**
 * Inserta unha nova entrada
 */
void insertHashMap(HashMap *hashMap, char *key, int value);

/**
 * Busca o valor asociado á entrada key
 */
int* searchHashMap(HashMap hashMap, char *key);

/**
 * Borra o nodo coa clave key e devolve o valor que tiña asociado ou NULL se non existía
 */
int* deleteHashMap(HashMap *hashMap, char *key);

#endif /* HASHMAP_H */