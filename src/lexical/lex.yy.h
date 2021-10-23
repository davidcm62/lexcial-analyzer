#ifndef LEXYY_H
#define LEXYY_H

#include <stdbool.h>

/**
 * Compoñente léxico
 */
typedef struct {
    int lexicalComp;
    char *lexeme;
} LexicalComponent;

/**
 * Inicializa a entrada de flex.
 * Devolve true se se puido abrir o ficheiro
 */
bool initFlex(const char *filename);

/**
 * Libera os recursos usados por flex (o ficheiro)
 */
void freeFlex();

// Sobreescribe a funcion yylex
#define YY_DECL LexicalComponent* nextLexicalComponent()

/**
 * Devolve o seguinte compoñente léxico do código fonte.
 * Devolve un punteiro á estructura con lexema e compoñente léxico. Este último
 * pode ser INVALID_LEXICAL_COMPONENT en caso de erro.
 */
LexicalComponent* nextLexicalComponent();

/**
 * Libera a memoria dun compoñente léxico.
 */
void freeLexicalComponent(LexicalComponent *lexicalComponent);

#endif /* LEXYY_H */