#include <stdlib.h>
#include <string.h>
#include "inputSystem.h"

void _loadBuffer(char *buffer, FILE *file){
    size_t totalRead = fread(buffer, sizeof(char), BUFFER_SIZE - 1, file);

    if(totalRead < BUFFER_SIZE - 1){
        buffer[totalRead+1] = EOF;
    }
}

InputSystem* initInputSystem(const char *filename){
    InputSystem *inputSystem = (InputSystem*)malloc(sizeof(InputSystem));
    inputSystem->filename = (char*)malloc((strlen(filename) + 1)*sizeof(char));
    inputSystem->stats = (SE_Stats*)malloc(sizeof(SE_Stats));

    strcpy(inputSystem->filename, filename);
    inputSystem->file = fopen(filename, "r");

    if(inputSystem->file == NULL){
        return NULL;
    }
    
    inputSystem->bufferA[BUFFER_SIZE-1] = EOF;
    inputSystem->bufferB[BUFFER_SIZE-1] = EOF;

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

char* _currentBuffer(InputSystem *inputSystem){
    int currentBuffer = inputSystem->currentBuffer;
    return currentBuffer == BUFFER_A? inputSystem->bufferA: inputSystem->bufferB;
}

char* _nextBuffer(InputSystem *inputSystem){
    int currentBuffer = inputSystem->currentBuffer;
    return currentBuffer == BUFFER_A? inputSystem->bufferB: inputSystem->bufferA;
}

char nextCharFromSource(InputSystem *inputSystem){
    char *buffer = _currentBuffer(inputSystem);

    char currentChar = *(inputSystem->frontPointer);

    //stats
    if(currentChar == '\n'){
        inputSystem->stats->line++;
    }

    inputSystem->frontPointer++;
    inputSystem->diffPointers++;

    if(inputSystem->diffPointers > (BUFFER_SIZE - 1)){
        return ERR_LEXEME_MAX_SIZE;
    }

    if(*(inputSystem->frontPointer) == EOF){
        if(inputSystem->frontPointer == (buffer + BUFFER_SIZE - 1)){
            if(inputSystem->loadNextBuffer){
                _loadBuffer(_nextBuffer(inputSystem), inputSystem->file);
            }
            inputSystem->frontPointer = _nextBuffer(inputSystem);
            inputSystem->currentBuffer = inputSystem->currentBuffer == BUFFER_A? BUFFER_B: BUFFER_A;
            inputSystem->loadNextBuffer = true;
        }else{
            return EOF;
        }
    }
    
    return currentChar;
}

char* getReadCharacters(InputSystem *inputSystem){
    int diffPointers = inputSystem->diffPointers;
    char *str = (char*)malloc(sizeof(char)*diffPointers + 1);
    
    char *frontPointerBuffer = _currentBuffer(inputSystem);
    int frontPointerPosition = inputSystem->frontPointer - frontPointerBuffer;
    int nBytesFromFrontBuffer = frontPointerPosition + 1;

    char *startPointerBuffer = frontPointerPosition - diffPointers >= 0? frontPointerBuffer: _nextBuffer(inputSystem);
    int startPointerPosition = inputSystem->startPointer - startPointerBuffer;
    int nBytesFromStartBuffer = BUFFER_SIZE - 1 - startPointerPosition;

    if(diffPointers <= nBytesFromStartBuffer){
        memcpy(str, inputSystem->startPointer, diffPointers);
        str[diffPointers] = '\0';
    }else{
        memcpy(str, inputSystem->startPointer, nBytesFromStartBuffer);
        memcpy(str + nBytesFromStartBuffer, frontPointerBuffer, nBytesFromFrontBuffer);
        str[diffPointers] = '\0';
    }
    
    inputSystem->startPointer = inputSystem->frontPointer;
    inputSystem->diffPointers = 0;
    
    return str;
}

void goBackNCharacters(InputSystem *inputSystem, int n){    
    char *frontPointerBuffer = _currentBuffer(inputSystem);
    
    if(inputSystem->frontPointer - n - frontPointerBuffer < 0){
        char *nextBuffer = _nextBuffer(inputSystem);
        inputSystem->currentBuffer = inputSystem->currentBuffer == BUFFER_A? BUFFER_B: BUFFER_A;
        int offset = BUFFER_SIZE - 1 + (inputSystem->frontPointer - n - frontPointerBuffer);
        inputSystem->frontPointer = nextBuffer + offset;
        inputSystem->loadNextBuffer = false;
    }else{
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