#include <stdio.h>
#include <stdlib.h>
#include "TS.h"
#include "definiciones.h"

typedef struct {
    char lexema[20];
    int compLexico;
} Keyword;

TS* crearTS(){
    TS *ts = crearHashMap(TS_SIZE);

    Keyword keywords[] = {
        {"False", _FALSE},
        {"await", _AWAIT},
        {"else", _ELSE},
        {"import", _IMPORT},
        {"pass", _PASS},
        {"None", _NONE},
        {"break", _BREAK},
        {"except", _EXCEPT},
        {"in", _IN},
        {"raise", _RAISE},
        {"True", _TRUE},
        {"class", _CLASS},
        {"finally", _FINALLY},
        {"is", _IS},
        {"return", _RETURN},
        {"and", _AND},
        {"continue", _CONTINUE},
        {"for", _FOR},
        {"lambda", _LAMBDA},
        {"try", _TRY},
        {"as", _AS},
        {"def", _DEF},
        {"from", _FROM},
        {"nonlocal", _NONLOCAL},
        {"while", _WHILE},
        {"assert", _ASSERT},
        {"del", _DEL},
        {"global", _GLOBAL},
        {"not", _NOT},
        {"with", _WITH},
        {"async", _ASYNC},
        {"elif", _ELIF},
        {"if", _IF},
        {"or", _OR},
        {"yield", _YIELD}
    };

    //max 15 char
    // char keywordss[][15] = {
    //     "False",    "await",    "else",     "import",   "pass",
    //     "None",     "break",    "except",   "in",       "raise",
    //     "True",     "class",    "finally",  "is",       "return",
    //     "and",      "continue", "for",      "lambda",   "try",
    //     "as",       "def",      "from",     "nonlocal", "while",
    //     "assert",   "del",      "global",   "not",      "with",
    //     "async",    "elif",     "if",       "or",       "yield"
    // };

    for(size_t i=0; i<sizeof(keywords)/sizeof(Keyword); i++){
        insertarHashMap(ts, keywords[i].lexema,keywords[i].compLexico);
    }

    return ts;
}

void liberarTS(TS *tablaSimbolos){
    liberarHashMap(tablaSimbolos);
}

void imprimirTS(TS tablaSimbolos){
    printf("==Tabla de Simbolos:==\n");
    imprimirHashMap(tablaSimbolos);
}

void insertarTS(TS *tablaSimbolos, char *key, int value){
    insertarHashMap(tablaSimbolos, key, value);
}

int* buscarTS(TS tablaSimbolos, char *key){
    return buscarHashMap(tablaSimbolos, key);
}