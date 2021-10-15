#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashmap.h"


/**
 * Devolve o valor mais cercano a target (v1 ou v2)
 */
unsigned long _getClosest(unsigned long v1, unsigned long v2, unsigned long target){
    return target - v1 >= v2 - target? v2 : v1;
}


/**
 * Devolve o primo máis próximo a target
 */
unsigned long _nextPrime(unsigned long target){
    //numeros primos onde cada un é maior que o doble do anterior
    //o mais grande é o primo mais pequeno menor que max unsigned long
    const unsigned long primes[] = {53, 107, 223, 449, 907, 1823, 3659, 7321, 14653, 29311, 58631, 117269, 234539, 469099, 938207, 1876417, 3752839, 7505681, 15011389, 30022781, 60045577, 120091177, 240182359, 480364727, 960729461, 1921458943, 3842917907};
    unsigned long totalPrimes = sizeof(primes)/sizeof(primes[0]);
    
    if (target <= primes[0]){
        return primes[0];
    }
    if (target >= primes[totalPrimes - 1]){
        return primes[totalPrimes - 1];
    }
 
    // Busqueda binaria do mais cercano
    int i = 0, j = totalPrimes, mid = 0;
    while (i < j) {
        mid = (i + j) / 2;
 
        if (primes[mid] == target){
            return primes[mid];
        }
 
        if (target < primes[mid]) {
            if (mid > 0 && target > primes[mid - 1]){
                return _getClosest(primes[mid - 1], primes[mid], target);
            }
            j = mid;
        } else {
            if (mid < totalPrimes - 1 && target < primes[mid + 1]){
                return _getClosest(primes[mid], primes[mid + 1], target);
            }
            i = mid + 1;
        }
    }
 
    return primes[mid];
}

/**
 * Hash djb2
 */
unsigned long _hashFunction(char *str){
    unsigned long hash = 5381;
    long c;
    
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

HashMap* initHashMap(unsigned long initialSize){
    //reserva de memoria hashmap
    HashMap *hashMap = (HashMap*)malloc(sizeof(HashMap));
    hashMap->hashMapSize = initialSize;
    hashMap->totalItems = 0;
    //reserva de memoria lista de punterios a listas enlazadas
    hashMap->buckets = (Bucket*)malloc(initialSize * sizeof(Bucket));
    
    for (unsigned long i = 0; i < initialSize; i++){
        hashMap->buckets[i].firstNode = NULL;
    }
    
    return hashMap;
}

/**
 * Libera un nodo
 */
void _freeBucketNode(BucketNode *node){
    free(node->key);
    free(node);
}

/**
 * Libera unha lista de nodos
 */
void _freeBucketList(BucketNode *first){
    BucketNode *tmp;
    //recorre a lista e vai liberando cada nodo
    while(first != NULL){
        tmp = first;
        first = first->nextNode;
        _freeBucketNode(tmp);
    }
}

void freeHashMap(HashMap *hashMap){
    //libera todas as listas enlazadas
    for (unsigned long i = 0; i < hashMap->hashMapSize; i++){
        _freeBucketList(hashMap->buckets[i].firstNode);
    }
    free(hashMap->buckets);
    free(hashMap);
}

void _printBucket(Bucket bucket){
    BucketNode *node = bucket.firstNode;
    while (node != NULL){
        printf("{%s: %d} - ", node->key, node->value);
        node = node->nextNode;
    }
    printf("NULL");
}

void printHashMap(HashMap hashMap){
    printf("======================\n");
    for (unsigned long i = 0; i < hashMap.hashMapSize; i++){
        printf("[%-4ld] -> ", i);
        _printBucket(hashMap.buckets[i]);
        printf("\n");
    }
    printf("Tamano: %ld elementos\n", hashMap.totalItems);
    printf("======================\n");
}

/**
 * Convirte un hashmap a unha lista de nodos
 */
BucketNode* _hashMapToList(HashMap hashMap){
    BucketNode *bucketNodes = (BucketNode*)malloc(hashMap.totalItems * sizeof(BucketNode));
    unsigned long totalItems = 0;
    //recorro todo o hashmap e introduzo cada bucket no array
    for (unsigned long i = 0; i < hashMap.hashMapSize; i++){
        BucketNode *node = hashMap.buckets[i].firstNode;

        while (node != NULL){ //recorrido da lista enlazada
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

/**
 * Inserta un nodo con clave key e valor value
 */
void _insertHashMap(HashMap *hashMap, char *key, int value){
    BucketNode *newNode = (BucketNode*)malloc(sizeof(BucketNode));
    unsigned long hash = _hashFunction(key) % hashMap->hashMapSize;
    Bucket *bucket = &(hashMap->buckets[hash]);

    //inicializo o novo nodo
    newNode->key = (char*)malloc(strlen(key)+1);
    strcpy(newNode->key, key);
    newNode->value = value;
    newNode->nextNode = NULL;

    //non hai nodos nesa lista: metoo ao principio e inicio a lista con ese nodo
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

/**
 * Aumenta o tamaño do hashmap
 */
void _resizeHashMap(HashMap *hashMap){
    //novo tamano -> primo mas cercano ao tamano actual * 2
    unsigned long newHashMapSize = _nextPrime(hashMap->hashMapSize * 2);
    unsigned long totalItems = hashMap->totalItems;
    BucketNode *bucketNodes = _hashMapToList(*hashMap);
    
    //redimension {
    for (unsigned long i = 0; i < hashMap->hashMapSize; i++){
        _freeBucketList(hashMap->buckets[i].firstNode);
    }

    free(hashMap->buckets);
    //nova lista de buckets (listas enlazadas)
    hashMap->buckets = (Bucket*)malloc(newHashMapSize * sizeof(Bucket));
    
    for (unsigned long i = 0; i < newHashMapSize; i++){
        hashMap->buckets[i].firstNode = NULL;
    }

    hashMap->hashMapSize = newHashMapSize;
    hashMap->totalItems = 0;
    // }

    for(long long i = totalItems - 1; i >= 0; i--){
        //reinserto os nodos outra vez
        _insertHashMap(hashMap, bucketNodes[i].key, bucketNodes[i].value);
    }

    free(bucketNodes);
}

void insertHashMap(HashMap *hashMap, char *key, int value){
    unsigned long currentTotalItems = hashMap->totalItems;

    //se se supera o factor de cargar entonces redimensiono o hashmap
    if(((1 + currentTotalItems*1.0)/hashMap->hashMapSize) >= LOAD_FACTOR){
        _resizeHashMap(hashMap);
    }

    //inserto o novo valor
    _insertHashMap(hashMap, key, value);
}

int* searchHashMap(HashMap hashMap, char *key){
    unsigned long hash = _hashFunction(key) % hashMap.hashMapSize;
    Bucket bucket = hashMap.buckets[hash];
    BucketNode *tmp = bucket.firstNode;
    int *value = (int*)malloc(sizeof(int));
    
    //recorro a lista enlazada ata atopalo
    while(tmp != NULL){
        if(strcmp(tmp->key, key) == 0){
            *value = tmp->value;
            return value;
        }
        tmp = tmp->nextNode;
    }

    return NULL;
}

int* deleteHashMap(HashMap *hashMap, char *key){
    int *oldValue = (int*)malloc(sizeof(int));
    unsigned long hash = _hashFunction(key) % hashMap->hashMapSize;
    Bucket *bucket = &(hashMap->buckets[hash]);
    BucketNode *tmp = bucket->firstNode;
    BucketNode *prev;

    //se é o primeiro actualizo o startPointer do bucket
    if(tmp != NULL && (strcmp(tmp->key, key) == 0)){
        *oldValue = tmp->value;
        bucket->firstNode = tmp->nextNode;
        _freeBucketNode(tmp);
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
    _freeBucketNode(tmp);
    hashMap->totalItems--;

    //devolvo o valor que había asociado
    return oldValue;
}
