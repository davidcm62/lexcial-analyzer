#include <stdio.h>
#include "stack.h"


int main(int argc, char const *argv[]){
    Stack *stack = initStack();
    printStack(*stack);
    pushStack(stack, 'a');
    pushStack(stack, 'b');
    pushStack(stack, 'c');
    printf("%c\n",getTopStack(*stack));
    printStack(*stack);
    pushStack(stack, 'x');
    printStack(*stack);
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printf("%c\n",popStack(stack));
    printStack(*stack);
    pushStack(stack, 'a');
    pushStack(stack, 'b');
    pushStack(stack, 'c');
    printStack(*stack);

    freeStack(stack);
    return 0;
}
