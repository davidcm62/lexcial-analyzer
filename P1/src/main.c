#include <stdio.h>
#include "TS.h"
#include "sisEntrada.h"
#include "sintactico.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void printRed(const char *msg){
    printf(ANSI_COLOR_RED"%s"ANSI_COLOR_RESET"\n",msg);
}

void printBlue(const char *msg){
    printf(ANSI_COLOR_CYAN"%s"ANSI_COLOR_RESET"\n",msg);
}

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

    printRed("Hola en rojo");
    printBlue("Hola en azul");
    printf("Hola normal\n");

    return 0;
}