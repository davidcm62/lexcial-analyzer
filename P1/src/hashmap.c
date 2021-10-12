#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashmap.h"

bool _isPrime(long x){
    if(x <= 1){
        return false;
    }
    if(x <= 3){
        return true;
    }

    if (x%2 == 0 || x%3 == 0){
        return false;
    }
   
    for (long i=5; i*i<=x; i+=6){
        if(x%i == 0 || x%(i+2) == 0){
            return false;
        }
    }

    return true;
}

long _nextPrime(long n){
    long prime = n;

    while (true){
        prime++;

        if(_isPrime(prime)){
            break;
        }
    }

    return prime;
}

//djb2 hash
unsigned long _hashFunction(char *str){
    unsigned long hash = 5381;
    long c;
    
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

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
        printf("[%-4ld] -> ", i);
        _imprimirBucket(hashMap.buckets[i]);
        printf("\n");
    }
    printf("Tamano: %ld elementos\n", hashMap.totalItems);
    printf("======================\n");
}

BucketNode* _hashMapToList(HashMap hashMap){
    BucketNode *bucketNodes = (BucketNode*)malloc(hashMap.totalItems * sizeof(BucketNode));
    long totalItems = 0;
    for (long i = 0; i < hashMap.hashMapSize; i++){
        BucketNode *node = hashMap.buckets[i].firstNode;
        while (node != NULL){
            bucketNodes[totalItems].key = (char*)malloc(strlen(node->key)+1);
            strcpy(bucketNodes[totalItems].key, node->key);
            bucketNodes[totalItems].value = node->value;
            bucketNodes[totalItems].nextNode = NULL;
            
            totalItems++;
            node = node->nextNode;
        }
    }

    return bucketNodes;
}

void _insertarHashMap(HashMap *hashMap, char *key, int value){
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
        hashMap->totalItems++;
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
    hashMap->totalItems++;
}

void _resizeHashMap(HashMap *hashMap){
    long newHashMapSize = _nextPrime(hashMap->hashMapSize * 2);
    long totalItems = hashMap->totalItems;
    BucketNode *bucketNodes = _hashMapToList(*hashMap);
    
    //redimension {
    for (long i = 0; i < hashMap->hashMapSize; i++){
        _liberarBucketList(hashMap->buckets[i].firstNode);
    }

    free(hashMap->buckets);
    hashMap->buckets = (Bucket*)malloc(newHashMapSize * sizeof(Bucket));
    
    for (long i = 0; i < newHashMapSize; i++){
        hashMap->buckets[i].firstNode = NULL;
    }

    hashMap->hashMapSize = newHashMapSize;
    hashMap->totalItems = 0;
    // }

    for(long i = totalItems - 1; i >= 0; i--){
        _insertarHashMap(hashMap, bucketNodes[i].key, bucketNodes[i].value);
    }

    free(bucketNodes);
}

void insertarHashMap(HashMap *hashMap, char *key, int value){
    long currentTotalItems = hashMap->totalItems;
    if(((1 + currentTotalItems*1.0)/hashMap->hashMapSize) >= LOAD_FACTOR){
        _resizeHashMap(hashMap);
    }

    _insertarHashMap(hashMap, key, value);
}

int* buscarHashMap(HashMap hashMap, char *key){
    unsigned long hash = _hashFunction(key) % hashMap.hashMapSize;
    Bucket bucket = hashMap.buckets[hash];
    BucketNode *tmp = bucket.firstNode;
    int *value = (int*)malloc(sizeof(int));
    
    while(tmp != NULL){
        if(strcmp(tmp->key, key) == 0){
            *value = tmp->value;
            return value;
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
        hashMap->totalItems--;
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
    hashMap->totalItems--;

    return oldValue;
}
