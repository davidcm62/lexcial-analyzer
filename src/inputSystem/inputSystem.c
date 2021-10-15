#include <stdlib.h>
#include <string.h>
#include "inputSystem.h"
#include "../error/error.h"

/**
 * Carga BUFFER_SIZE - 1 caracteres do ficheiro en *buffer
 */
void _loadBuffer(char *buffer, FILE *file){
    size_t totalRead = fread(buffer, sizeof(char), BUFFER_SIZE - 1, file);

    if(totalRead < BUFFER_SIZE - 1){
        buffer[totalRead+1] = EOF;
    }
}

InputSystem* initInputSystem(const char *filename){
    //reserva de memoria
    InputSystem *inputSystem = (InputSystem*)malloc(sizeof(InputSystem));
    inputSystem->filename = (char*)malloc((strlen(filename) + 1)*sizeof(char));
    inputSystem->stats = (IS_Stats*)malloc(sizeof(IS_Stats));

    //apertura do ficheiro
    strcpy(inputSystem->filename, filename);
    inputSystem->file = fopen(filename, "r");

    if(inputSystem->file == NULL){
        //error ficheiro
        handleError(IO);
        return NULL;
    }
    
    //centinelas
    inputSystem->bufferA[BUFFER_SIZE-1] = EOF;
    inputSystem->bufferB[BUFFER_SIZE-1] = EOF;

    //cargo o buffer A
    _loadBuffer(inputSystem->bufferA, inputSystem->file);

    inputSystem->startPointer = &(inputSystem->bufferA[0]);
    inputSystem->frontPointer = &(inputSystem->bufferA[0]);

    inputSystem->diffPointers = 0;
    inputSystem->currentBuffer = BUFFER_A;

    inputSystem->loadNextBuffer = true;

    inputSystem->stats->line = 1;

    return inputSystem;
}

void freeInputSystem(InputSystem *inputSystem){
    fclose(inputSystem->file);
    free(inputSystem->filename);
    free(inputSystem->stats);
    free(inputSystem);
}

/**
 * Devolve o buffer que se esta usando actualmente
 */
char* _currentBuffer(InputSystem *inputSystem){
    int currentBuffer = inputSystem->currentBuffer;
    return currentBuffer == BUFFER_A? inputSystem->bufferA: inputSystem->bufferB;
}

/**
 * Devolve o seguinte buffer que se ten que cargar
 */
char* _nextBuffer(InputSystem *inputSystem){
    int currentBuffer = inputSystem->currentBuffer;
    return currentBuffer == BUFFER_A? inputSystem->bufferB: inputSystem->bufferA;
}

char nextCharFromSource(InputSystem *inputSystem){
    char *buffer = _currentBuffer(inputSystem);

    //caracter leido
    char currentChar = *(inputSystem->frontPointer);

    //stats
    if(currentChar == '\n'){
        inputSystem->stats->line++;
    }

    //avanzo delantero
    inputSystem->frontPointer++;
    inputSystem->diffPointers++;

    //superase o tamaño máximo de lexema? -> error
    if(inputSystem->diffPointers > (BUFFER_SIZE - 1)){
        return ERR_LEXEME_MAX_SIZE;
    }

    if(*(inputSystem->frontPointer) == EOF){
        if(inputSystem->frontPointer == (buffer + BUFFER_SIZE - 1)){    //caso EOF é o fin de buffer
            if(inputSystem->loadNextBuffer){
                _loadBuffer(_nextBuffer(inputSystem), inputSystem->file);
            }
            inputSystem->frontPointer = _nextBuffer(inputSystem);
            inputSystem->currentBuffer = inputSystem->currentBuffer == BUFFER_A? BUFFER_B: BUFFER_A;
            inputSystem->loadNextBuffer = true;
        }else{  //caso EOF do ficheiro
            return EOF;
        }
    }
    
    return currentChar;
}

char* getReadCharacters(InputSystem *inputSystem){
    int diffPointers = inputSystem->diffPointers;
    // string para copiar os caracteres entre inicio e delantero
    char *str = (char*)malloc(sizeof(char)*diffPointers + 1);
    
    //calculo o buffer de delantero, o índice dentro do array e os bytes dende o inicio do array ata delantero
    char *frontPointerBuffer = _currentBuffer(inputSystem);
    int frontPointerPosition = inputSystem->frontPointer - frontPointerBuffer;
    int nBytesFromFrontBuffer = frontPointerPosition + 1;

    //calculo o buffer onde está inicio, o índice de inicio e cantos bytes libres hai nese buffer
    char *startPointerBuffer = frontPointerPosition - diffPointers >= 0? frontPointerBuffer: _nextBuffer(inputSystem);
    int startPointerPosition = inputSystem->startPointer - startPointerBuffer;
    int nBytesFromStartBuffer = BUFFER_SIZE - 1 - startPointerPosition;

    if(diffPointers <= nBytesFromStartBuffer){  //caso no que inicio e delantero estan no mesmo buffer
        memcpy(str, inputSystem->startPointer, diffPointers);
        str[diffPointers] = '\0';
    }else{  //caso no que inicio esttá nun buffer e delantero no seguinte
        memcpy(str, inputSystem->startPointer, nBytesFromStartBuffer);
        memcpy(str + nBytesFromStartBuffer, frontPointerBuffer, nBytesFromFrontBuffer);
        str[diffPointers] = '\0';
    }
    
    inputSystem->startPointer = inputSystem->frontPointer;
    inputSystem->diffPointers = 0;
    
    return str;
}

void goBackNCharacters(InputSystem *inputSystem, int n){
    //buffer actual 
    char *frontPointerBuffer = _currentBuffer(inputSystem);

    if(inputSystem->frontPointer - n - frontPointerBuffer < 0){ //caso no que retrocedo e cambio de buffer
        char *nextBuffer = _nextBuffer(inputSystem);
        inputSystem->currentBuffer = inputSystem->currentBuffer == BUFFER_A? BUFFER_B: BUFFER_A;
        // calculo a posición dentro do buffer anterior tendo en conta o retroceso dende o actual
        int offset = BUFFER_SIZE - 1 + (inputSystem->frontPointer - n - frontPointerBuffer);
        inputSystem->frontPointer = nextBuffer + offset;
        //o seguinte EOF non carga o buffer outra vez
        inputSystem->loadNextBuffer = false;
    }else{  //caso no que retrocedo e quedo no mesmo buffer
        inputSystem->frontPointer -= n;
    }
    inputSystem->diffPointers -= n;

    //stats
    if(*(inputSystem->frontPointer) == '\n'){
        inputSystem->stats->line--;
    }
}

void goBack1Character(InputSystem *inputSystem){
    goBackNCharacters(inputSystem, 1);
}

void matchPointers(InputSystem *inputSystem){
    inputSystem->startPointer = inputSystem->frontPointer;
    inputSystem->diffPointers = 0;
}