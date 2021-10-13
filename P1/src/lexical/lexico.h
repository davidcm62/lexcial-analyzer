#ifndef LEXICO_H
#define LEXICO_H

#include "../ts/TS.h"
#include "../inputSystem/sisEntrada.h"

typedef struct {
    int compLexico;
    char *lexema;
} CompLexico;

typedef enum LexicalResult {
    SUCCESS = 1,
    LEXEME_TOO_LONG = -1,
    LEXICAL_ERROR = -2,
} LexicalResult;

LexicalResult seguinteCompLexico(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada, CompLexico *compLexico);

CompLexico* initCompLexico();
void liberarCompLexico(CompLexico *compLexico);

#endif /* LEXICO_H */