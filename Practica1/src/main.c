#include <stdio.h>
#include "TS.h"

void f(FILE *file){
    char c = fgetc(file);
    printf("%c\n",c);
}

int main(int argc, char const **argv){
    TS *ts = crearTS();
    imprimirTS(*ts);
    
    // imprimirTS(*ts);

    liberarTS(ts);


    FILE *file = fopen("./test.txt", "r");

    f(file);
    f(file);


    fclose(file);


    return 0;
}
