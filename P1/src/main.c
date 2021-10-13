#include <stdio.h>
#include "./ts/TS.h"
#include "./inputSystem/sisEntrada.h"
#include "./syntactic/syntactic.h"
#include "./error/error.h"

int main(int argc, char const **argv){
    if(argc < 2){
        handleError(COMMAND_LINE_PARAMS);
        return 1;
    }
    
    const char *filename = argv[1];
    TS *ts = initTS();
    SistemaEntrada *inputSystem;
    if((inputSystem = inicializarSistemaEntrada(filename)) == NULL){
        handleError(IO);
        return 1;
    }
    
    printTS(*ts);
    printf("\n");
    startSyntacticAnalysis(ts, inputSystem);
    printf("\n");
    printTS(*ts);

    liberarSistemaEntrada(inputSystem);
    freeTS(ts);

    return 0;
}