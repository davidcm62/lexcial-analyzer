#ifndef LEXICO_H
#define LEXICO_H

#include "TS.h"
#include "sisEntrada.h"

typedef struct {
    int compLexico;
    char *lexema;
} CompLexico;

CompLexico* seguinteCompLexico(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada);
void liberarCompLexico(CompLexico *compLexico);

#endif /* LEXICO_H */