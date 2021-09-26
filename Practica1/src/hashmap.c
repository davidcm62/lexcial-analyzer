#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

unsigned long _hashFunction(char *str){
    unsigned long hash = 5381;
    long c;
    
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

// typedef struct Node {
//     char *key;
//     int value;
//     struct Node *nextNode;
// } BucketNode;

// typedef struct {
//     BucketNode *firstNode;
// } Bucket;

// typedef struct {
//     Bucket *buckets;
//     long hashMapSize;
//     long totalItems;
// } HashMap;

HashMap* crearHashMap(long initialSize){
    HashMap *hashMap = (HashMap*)malloc(sizeof(HashMap));
    hashMap->hashMapSize = initialSize;
    hashMap->totalItems = 0;
    hashMap->buckets = (Bucket*)malloc(initialSize * sizeof(Bucket));
    
    for (long i = 0; i < initialSize; i++){
        hashMap->buckets[i].firstNode = NULL;
    }
    
    return hashMap;
}

void _liberarBucketNode(BucketNode *node){
    free(node->key);
    free(node);
}

void _liberarBucketList(BucketNode *first){
    BucketNode *tmp;

    while(first != NULL){
        tmp = first;
        first = first->nextNode;
        _liberarBucketNode(tmp);
    }
}

void liberarHashMap(HashMap *hashMap){
    for (long i = 0; i < hashMap->hashMapSize; i++){
        _liberarBucketList(hashMap->buckets[i].firstNode);
    }
    free(hashMap->buckets);
    free(hashMap);
}

void _imprimirBucket(Bucket bucket){
    BucketNode *node = bucket.firstNode;
    while (node != NULL){
        printf("{%s: %d} - ", node->key, node->value);
        node = node->nextNode;
    }
    printf("NULL");
}

void imprimirHashMap(HashMap hashMap){
    printf("======================\n");
    for (long i = 0; i < hashMap.hashMapSize; i++){
        printf("[%-4d] -> ", i);
        _imprimirBucket(hashMap.buckets[i]);
        printf("\n");
    }
    printf("======================\n");
}

void insertarHashMap(HashMap *hashMap, char *key, int value){
    BucketNode *newNode = (BucketNode*)malloc(sizeof(BucketNode));
    unsigned long hash = _hashFunction(key) % hashMap->hashMapSize;
    Bucket *bucket = &(hashMap->buckets[hash]);

    //inicializo o novo nodo
    newNode->key = (char*)malloc(strlen(key)+1);
    strcpy(newNode->key, key);
    newNode->value = value;
    newNode->nextNode = NULL;

    //non hai nodos nesa lista
    if(bucket->firstNode == NULL){
        bucket->firstNode = newNode;
        return;
    }
    
    BucketNode *next = bucket->firstNode;
    //reemplazo a clave se xa existe
    while(next != NULL){
        if(strcmp(next->key, key) == 0){
            next->value = value;
            return;
        }
        next = next->nextNode;
    }
    
    //meto o novo nodo ao principio
    BucketNode *first = bucket->firstNode;
    newNode->nextNode = first;
    bucket->firstNode = newNode;
}

int* buscarHashMap(HashMap hashMap, char *key){
    unsigned long hash = _hashFunction(key) % hashMap.hashMapSize;
    Bucket bucket = hashMap.buckets[hash];
    BucketNode *tmp = bucket.firstNode;
    
    while(tmp != NULL){
        if(strcmp(tmp->key, key) == 0){
            return &(tmp->value);
        }
        tmp = tmp->nextNode;
    }

    return NULL;
}

int* borrarHashMap(HashMap *hashMap, char *key){
    int *oldValue = (int*)malloc(sizeof(int));
    unsigned long hash = _hashFunction(key) % hashMap->hashMapSize;
    Bucket *bucket = &(hashMap->buckets[hash]);
    BucketNode *tmp = bucket->firstNode;
    BucketNode *prev;

    //se é o primeiro actualizo o inicio do bucket
    if(tmp != NULL && (strcmp(tmp->key, key) == 0)){
        *oldValue = tmp->value;
        bucket->firstNode = tmp->nextNode;
        _liberarBucketNode(tmp);
        return oldValue;    
    }

    //busco o nodo
    while(tmp != NULL && (strcmp(tmp->key, key) != 0)){
        prev = tmp;
        tmp = tmp->nextNode;
    }

    if(tmp == NULL){
        return NULL;
    }

    // se esta polo medio, actualizo para que o 
    // anterior enlaze o seguinte do borrado
    *oldValue = tmp->value;
    prev->nextNode = tmp->nextNode;
    _liberarBucketNode(tmp);

    return oldValue;
}
