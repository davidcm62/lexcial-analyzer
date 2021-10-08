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
    long hashMapSize;
    long totalItems;
} HashMap;

HashMap* crearHashMap(long initialSize);
void liberarHashMap(HashMap *hashMap);
void imprimirHashMap(HashMap hashMap);
void insertarHashMap(HashMap *hashMap, char *key, int value);
int* buscarHashMap(HashMap hashMap, char *key);
int* borrarHashMap(HashMap *hashMap, char *key);

#endif /* HASHMAP_H */