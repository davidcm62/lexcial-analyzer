#include <stdlib.h>
#include "TS.h"

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
    int size;
} HashMap;

