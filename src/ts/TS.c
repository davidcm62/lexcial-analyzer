#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "../common/definitions.h"

/* 
    Numero primo mas pequeno para que
    collan minimo todas as keywords mantendo
    factor de carga <= 0.75. Asi evito ter
    que redimensionar ao principio xa.
*/
#define TS_SIZE 53

// variable TS
HashMap *_hashmap = NULL;


typedef struct {
    char lexeme[20];
    int lexicalComp;
} Keyword;

void initTS(){
    _hashmap = initHashMap(TS_SIZE);

    Keyword keywords[] = {
        {"False", FALSE},
        {"await", AWAIT},
        {"else", ELSE},
        {"import", IMPORT},
        {"pass", PASS},
        {"None", NONE},
        {"break", BREAK},
        {"except", EXCEPT},
        {"in", IN},
        {"raise", RAISE},
        {"True", TRUE},
        {"class", CLASS},
        {"finally", FINALLY},
        {"is", IS},
        {"return", RETURN},
        {"and", AND},
        {"continue", CONTINUE},
        {"for", FOR},
        {"lambda", LAMBDA},
        {"try", TRY},
        {"as", AS},
        {"def", DEF},
        {"from", FROM},
        {"nonlocal", NONLOCAL},
        {"while", WHILE},
        {"assert", ASSERT},
        {"del", DEL},
        {"global", GLOBAL},
        {"not", NOT},
        {"with", WITH},
        {"async", ASYNC},
        {"elif", ELIF},
        {"if", IF},
        {"or", OR},
        {"yield", YIELD},
        {"print", PRINT},
    };

    for(size_t i=0; i<sizeof(keywords)/sizeof(Keyword); i++){
        //insetanse as palabras reservadas
        insertHashMap(_hashmap, keywords[i].lexeme,keywords[i].lexicalComp);
    }
}

void freeTS(){
    freeHashMap(_hashmap);
}

void printTS(){
    printf("==Tabla de Simbolos:==\n");
    printHashMap(*_hashmap);
}

void insertTS(char *key, int value){
    insertHashMap(_hashmap, key, value);
}

int* searchTS(char *key){
    return searchHashMap(*_hashmap, key);
}