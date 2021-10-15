#ifndef LEXICAL_H
#define LEXICAL_H

#include "../ts/TS.h"
#include "../inputSystem/inputSystem.h"

typedef struct {
    int lexicalComp;
    char *lexeme;
} LexicalComponent;

typedef enum lexicalResult {
    SUCCESS = 1,
    LEXEME_TOO_LONG = -1,
    LEXICAL_ERROR = -2,
} LexicalResult;

/**
 * Función para recuperar o seguinte componente léxico do código fonte.
 */
LexicalResult nextLexicalComponent(TS *ts, InputSystem *inputSystem, LexicalComponent *lexicalComponent);

/**
 * Función para reservar a memoria dun componente léxico.
 */
LexicalComponent* initLexicalComponent();

/**
 * Función para liberar a memoria dun componente léxico.
 */
void freeLexicalComponent(LexicalComponent *lexicalComponent);

#endif /* LEXICAL_H */