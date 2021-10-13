#include <stdio.h>
#include "./ts/TS.h"
#include "./inputSystem/sisEntrada.h"
#include "./syntactical/sintactico.h"
#include "./error/error.h"

int main(int argc, char const **argv){
    if(argc < 2){
        handleError(COMMAND_LINE_PARAMS);
        return 1;
    }
    
    const char *filename = argv[1];
    TS *ts = initTS();
    SistemaEntrada *sistemaEntrada;
    if((sistemaEntrada = inicializarSistemaEntrada(filename)) == NULL){
        handleError(IO);
        return 1;
    }
    
    printTS(*ts);
    printf("\n");
    iniciarAnalisisSintactico(ts, sistemaEntrada);
    printf("\n");
    printTS(*ts);

    liberarSistemaEntrada(sistemaEntrada);
    freeTS(ts);

    return 0;
}