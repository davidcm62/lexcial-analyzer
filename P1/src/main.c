#include <stdio.h>
#include "TS.h"
#include "sisEntrada.h"
#include "sintactico.h"
#include "error.h"

int main(int argc, char const **argv){
    if(argc < 2){
        handleError(COMMAND_LINE_PARAMS);
        return 1;
    }
    
    const char *filename = argv[1];
    TS *ts = crearTS();
    SistemaEntrada *sistemaEntrada;
    if((sistemaEntrada = inicializarSistemaEntrada(filename)) == NULL){
        handleError(IO);
        return 1;
    }
    
    // imprimirTS(*ts);
    iniciarAnalisisSintactico(ts, sistemaEntrada);
    // imprimirTS(*ts);

    liberarSistemaEntrada(sistemaEntrada);
    liberarTS(ts);

    return 0;
}