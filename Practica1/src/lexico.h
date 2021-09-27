#ifndef LEXICO_H
#define LEXICO_H

#include "TS.h"

typedef struct {
    int compLexico;
    char *lexema;
} CompLexico;

CompLexico* seguinteCompLexico(TS *tablaSimbolos);
void liberarCompLexico(CompLexico *compLexico);

#endif /* LEXICO_H */