#include <stdio.h>
#include "TS.h"
#include "sisEntrada.h"
#include "sintactico.h"

int main(int argc, char const **argv){
    if(argc < 2){
        //TODO: imprimir error
        return 1;
    }
    // printf("%s\n",argv[1]);

    const char *filename = argv[1];
    TS *ts = crearTS();
    SistemaEntrada *sistemaEntrada = inicializarSistemaEntrada(filename);
    
    imprimirTS(*ts);
    iniciarAnalisisSintactico(ts, sistemaEntrada);
    imprimirTS(*ts);

    liberarSistemaEntrada(sistemaEntrada);
    liberarTS(ts);

    return 0;
}