#include <stdio.h>
#include "./ts/TS.h"
#include "./inputSystem/inputSystem.h"
#include "./syntactic/syntactic.h"
#include "./error/error.h"

int main(int argc, char const **argv){
    if(argc < 2){
        handleError(COMMAND_LINE_PARAMS);
        return 1;
    }
    
    // Inicialización de estructuras
    const char *filename = argv[1];
    
    initTS();
    if(!initInputSystem(filename)){
        return 1;
    }
    
    //Execución do análisis
    // printTS();
    // printf("\n");
    startSyntacticAnalysis();
    // printf("\n");
    // printTS();

    // liberar recursos
    freeInputSystem();
    freeTS();

    return 0;
}