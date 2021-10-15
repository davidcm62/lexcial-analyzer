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
    InputSystem *inputSystem;
    if((inputSystem = initInputSystem(filename)) == NULL){
        return 1;
    }
    
    //Execución do análisis
    printTS();
    printf("\n");
    startSyntacticAnalysis(inputSystem);
    printf("\n");
    printTS();

    // liberar recursos
    freeInputSystem(inputSystem);
    freeTS();

    return 0;
}