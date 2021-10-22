#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"


int main(int argc, char const *argv[]){
    
    init();
    printf("\n\n:%d:\n\n",yylex());
    closeeeee();

    return 0;
}
