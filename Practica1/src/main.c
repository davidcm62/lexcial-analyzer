#include <stdio.h>
// #include "hashmap.h"
#include "TS.h"
#include "lexico.h"



int main(int argc, char const **argv){
    
    TS *ts = crearTS();
    
    CompLexico *compLexico = seguinteCompLexico(ts);

    printf("<%d, \"%s\">\n",compLexico->compLexico,compLexico->lexema);

    liberarCompLexico(compLexico);

    // imprimirTS(*ts);

    // insertarTS(ts, "uno", 1);
    // insertarTS(ts, "dos", 2);
    // insertarTS(ts, "tres", 3);
    // insertarTS(ts, "cuatro", 4);

    // // imprimirTS(*ts);

    // int *uno = buscarTS(*ts, "uno");
    // printf("%s -> %d\n", "uno", uno != NULL? *uno: -121221);
    // int *catorce = buscarTS(*ts, "catorce");
    // printf("%s -> %d\n", "catorce", catorce != NULL? *catorce: -121221);

    // insertarTS(ts, "uno", -1);
    // insertarTS(ts, "dos", -2);
    // insertarTS(ts, "tres", -3);
    // insertarTS(ts, "cuatro", -4);
    // insertarTS(ts, "cinco", 5);

    // insertarTS(ts, "seis", 6);

    // insertarTS(ts, "sete", 7);
    // insertarTS(ts, "oito", 8);
    // insertarTS(ts, "nove", 9);
    // insertarTS(ts, "dez", 10);
    // insertarTS(ts, "once", 11);
    // insertarTS(ts, "doce", 12);
    // insertarTS(ts, "trece", 13);
    // insertarTS(ts, "catorce", 14);
    // insertarTS(ts, "quince", 15);
    // insertarTS(ts, "dezaseis", 16);
    // insertarTS(ts, "dezasete", 17);

    // imprimirTS(*ts);
    // printf("%s -> %d\n", "uno", *buscarTS(*ts, "uno"));

    // liberarTS(ts);

    return 0;
}
