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
    
    const char *filename = "./python/test4.py";//argv[1];
    TS *ts = crearTS();
    SistemaEntrada *sistemaEntrada;
    if((sistemaEntrada = inicializarSistemaEntrada(filename)) == NULL){
        handleError(FILE_ERROR);
        return 1;
    }
    
    // imprimirTS(*ts);
    iniciarAnalisisSintactico(ts, sistemaEntrada);
    // imprimirTS(*ts);

    liberarSistemaEntrada(sistemaEntrada);
    liberarTS(ts);

    // printRed("Hola en rojo");
    // printBlue("Hola en azul");
    // printf("Hola normal\n");

    return 0;
}