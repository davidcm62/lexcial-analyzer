#include <stdio.h>
#include "TS.h"
#include "sisEntrada.h"
#include "sintactico.h"


// #include <stdlib.h>

// typedef enum automataResult {
//     SUCCESS = 1,
//     ERROR = -10
// } AutomataResult;

// void f2(int param, int *result){
//     *result = param * 2;
// }

// AutomataResult f(int param, int *result){
//     result = (int*)malloc(sizeof(int));
//     f2(param, result);
//     printf("%d %d %d\n",*result,*result, *result);
//     return param%2 == 0?SUCCESS:ERROR;
// }

int main(int argc, char const **argv){
    if(argc < 2){
        //TODO: imprimir error
        return 1;
    }
    // printf("%s\n",argv[1]);

    const char *filename = argv[1];
    TS *ts = crearTS();
    SistemaEntrada *sistemaEntrada = inicializarSistemaEntrada(filename);
    
    // imprimirTS(*ts);
    iniciarAnalisisSintactico(ts, sistemaEntrada);
    // imprimirTS(*ts);

    liberarSistemaEntrada(sistemaEntrada);
    liberarTS(ts);

    // int *xd;
    // AutomataResult res;
    // res = f(2,xd);
    // printf("%d %d %d\n",res,*xd, res == SUCCESS);
    // // free(xd);
    // res = f(5, xd);
    // printf("%d %d %d\n",res,*xd, res == SUCCESS);

    return 0;
}