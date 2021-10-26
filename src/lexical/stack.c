#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./stack.h"

StackNode* _initStackNode(char data){
    StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

void _freeStackNode(StackNode *node){
    free(node);
}

Stack* initStack(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    //inicializa seguinte a null
    stack->topNode = NULL;
    return stack;
}

void freeStack(Stack *stack){
    StackNode *temp = stack->topNode;
    StackNode *node;

    //recorremos a pila liberando os nodos atopados
    while(temp != NULL){
        node = temp;
        temp = temp->next;

        _freeStackNode(node);
    }

    free(stack);
}

void printStack(Stack stack){
    StackNode *node = stack.topNode;
    printf("Pila: top[");
    while(node != NULL){
        printf("%c, ", node->data);
        node = node->next;
    }
    printf("]last\n");
}
 
bool isEmptyStack(Stack stack){
    return stack.topNode == NULL;
}
 
void pushStack(Stack *stack, char data){
    //creamos o nodo
    StackNode *node = _initStackNode(data);
    //tope pasa a ser o seguinte do novo nodo
    node->next = stack->topNode;
    //actualizo o tope da pila
    stack->topNode = node;
}
 
char popStack(Stack *stack){
    if (isEmptyStack(*stack)) {
        return -1;
    }

    //nodo do tope da pila
    StackNode *temp = stack->topNode;
    //actualizo o tope da pila
    stack->topNode = temp->next;
    //valor do nodo retirado
    char topValue = temp->data;
 
    //libero o nodo borrado
    _freeStackNode(temp);

    return topValue;
}
 
char getTopStack(Stack stack){
    if (isEmptyStack(stack)){
        return -1;
    }

    return stack.topNode->data;
}