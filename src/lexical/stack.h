#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/**
 * Estructura nodo da pila
 */ 
typedef struct stack_node{
    char data;
    struct stack_node* next;
} StackNode;

/**
 * Estructura pila
 */
typedef struct {
    StackNode *topNode;
} Stack;

/**
 * Inicializa a pila
 */
Stack* initStack();

/**
 * Libera os recursos da pila
 */
void freeStack(Stack *stack);

/**
 * Imprime a pila
 */
void printStack(Stack stack);
 
/**
 * Comproba se a pila está vacía
 */
bool isEmptyStack(Stack stack);

/**
 * Mete un novo valor na pila
 */
void pushStack(Stack *stack, char data);

/**
 * Elimina o nodo do tope da pila
 */
char popStack(Stack *stack);

/**
 * Recupera o valor do tope
 */
char getTopStack(Stack stack);

#endif /* STACK_H */