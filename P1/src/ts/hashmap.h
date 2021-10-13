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

HashMap* initHashMap(long initialSize);
void freeHashMap(HashMap *hashMap);
void printHashMap(HashMap hashMap);
void insertHashMap(HashMap *hashMap, char *key, int value);
int* searchHashMap(HashMap hashMap, char *key);
int* deleteHashMap(HashMap *hashMap, char *key);

#endif /* HASHMAP_H */