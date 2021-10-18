#ifndef LEXICAL_H
#define LEXICAL_H

/**
 * Compoñente léxico
 */
typedef struct {
    int lexicalComp;
    char *lexeme;
} LexicalComponent;

/**
 * Resultado de invocar a seguinte compoñente léxico: compoñente reconocido, tamaño de lexema excedido ou lexema mal formado
 */
typedef enum lexicalResult {
    SUCCESS = 1,
    LEXEME_TOO_LONG = -1,
    LEXICAL_ERROR = -2,
} LexicalResult;

/**
 * Devolve o seguinte compoñente léxico do código fonte.
 * Recibe o compoñente léxico coa memoria reservada e devolve o resultado da búsqueda léxica, xunto co compoñente léxico
 * incializado cos valores lexema e compoñente léxico
 */
LexicalResult nextLexicalComponent(LexicalComponent *lexicalComponent);

/**
 * Reserva a memoria dun compoñente léxico.
 */
LexicalComponent* initLexicalComponent();

/**
 * Libera a memoria dun compoñente léxico.
 */
void freeLexicalComponent(LexicalComponent *lexicalComponent);

#endif /* LEXICAL_H */