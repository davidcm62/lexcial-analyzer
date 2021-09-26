#include <stdio.h>
// #include "hashmap.h"
#include "TS.h"



int main(int argc, char const **argv){
    TS *ts = crearTS();
    imprimirTS(*ts);

    insertarTS(ts, "uno", 1);
    insertarTS(ts, "dos", 2);
    insertarTS(ts, "tres", 3);
    insertarTS(ts, "cuatro", 4);

    // imprimirTS(*ts);

    int *uno = buscarTS(*ts, "uno");
    printf("%s -> %d\n", "uno", uno != NULL? *uno: -121221);
    int *catorce = buscarTS(*ts, "catorce");
    printf("%s -> %d\n", "catorce", catorce != NULL? *catorce: -121221);

    insertarTS(ts, "uno", -1);
    insertarTS(ts, "dos", -2);
    insertarTS(ts, "tres", -3);
    insertarTS(ts, "cuatro", -4);
    insertarTS(ts, "cinco", 5);

    insertarTS(ts, "seis", 6);

    insertarTS(ts, "sete", 7);
    insertarTS(ts, "oito", 8);
    insertarTS(ts, "nove", 9);
    insertarTS(ts, "dez", 10);
    insertarTS(ts, "once", 11);
    insertarTS(ts, "doce", 12);
    insertarTS(ts, "trece", 13);
    insertarTS(ts, "catorce", 14);
    insertarTS(ts, "quince", 15);
    insertarTS(ts, "dezaseis", 16);
    insertarTS(ts, "dezasete", 17);

    imprimirTS(*ts);
    printf("%s -> %d\n", "uno", *buscarTS(*ts, "uno"));

    liberarTS(ts);


    // HashMap *hashMap = crearHashMap(TS_SIZE);
    // imprimirHashMap(*hashMap);

    // insertarHashMap(hashMap, "uno", 1);
    // insertarHashMap(hashMap, "dos", 2);
    // insertarHashMap(hashMap, "tres", 3);
    // insertarHashMap(hashMap, "cuatro", 4);

    // imprimirHashMap(*hashMap);

    // int *uno = buscarHashMap(*hashMap, "uno");
    // printf("%s -> %d\n", "uno", uno != NULL? *uno: -121221);
    // int *catorce = buscarHashMap(*hashMap, "catorce");
    // printf("%s -> %d\n", "catorce", catorce != NULL? *catorce: -121221);

    // insertarHashMap(hashMap, "uno", -1);
    // insertarHashMap(hashMap, "dos", -2);
    // insertarHashMap(hashMap, "tres", -3);
    // insertarHashMap(hashMap, "cuatro", -4);
    // insertarHashMap(hashMap, "a", 22);
    // insertarHashMap(hashMap, "csuatro", 22);
    // insertarHashMap(hashMap, "cusatro", 22);
    // insertarHashMap(hashMap, "cuadtro", 22);
    // insertarHashMap(hashMap, "cuatdro", 33);
    // insertarHashMap(hashMap, "cuatrso", 22);
    // insertarHashMap(hashMap, "cuatrao", 22);
    // insertarHashMap(hashMap, "cuatrao", 22);
    // insertarHashMap(hashMap, "cuatrso", 22);
    // insertarHashMap(hashMap, "cuatrso", 22);
    // insertarHashMap(hashMap, "cuatrdo", 11);
    // insertarHashMap(hashMap, "cuat2ro", 22);
    // insertarHashMap(hashMap, "cuatrsdo", 22);

    // imprimirHashMap(*hashMap);
    // printf("%s -> %d\n", "uno", *buscarHashMap(*hashMap, "uno"));


    // borrarHashMap(hashMap, "cuatdro");
    // borrarHashMap(hashMap, "cuatrdo");
    // borrarHashMap(hashMap, "aaaaaaaaaaaaaaa");
    // printf("borrar dos -> %d\n", *borrarHashMap(hashMap, "dos"));
    // imprimirHashMap(*hashMap);
    // printf("borrar dos null? -> %d\n", borrarHashMap(hashMap, "dos") == NULL);
    // imprimirHashMap(*hashMap);

    // liberarHashMap(hashMap);

    return 0;
}
