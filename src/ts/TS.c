#include <stdio.h>
#include <stdlib.h>
#include "TS.h"
#include "../common/definitions.h"

typedef struct {
    char lexeme[20];
    int lexicalComp;
} Keyword;

TS* initTS(){
    TS *ts = initHashMap(TS_SIZE);

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
        insertHashMap(ts, keywords[i].lexeme,keywords[i].lexicalComp);
    }

    return ts;
}

void freeTS(TS *symbolTable){
    freeHashMap(symbolTable);
}

void printTS(TS symbolTable){
    printf("==Tabla de Simbolos:==\n");
    printHashMap(symbolTable);
}

void insertTS(TS *symbolTable, char *key, int value){
    insertHashMap(symbolTable, key, value);
}

int* searchTS(TS symbolTable, char *key){
    return searchHashMap(symbolTable, key);
}