#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 83
#define BUFFER_A 0
#define BUFFER_B 1
#define ERR_LEXEME_MAX_SIZE -2

typedef struct {
    unsigned long line;
} SE_Stats;

typedef struct {
    char *filename;
    FILE *file;
    
    char bufferA[BUFFER_SIZE];
    char bufferB[BUFFER_SIZE];
    
    char *frontPointer;
    char *startPointer;
    
    int diffPointers;
    int currentBuffer;

    bool loadNextBuffer;

    SE_Stats *stats;
} InputSystem;

InputSystem* initInputSystem(const char *filename);
void freeInputSystem(InputSystem *inputSystem);

/*
    return char co seguinte caracter
    return ERR_LEXEME_MAX_SIZE si se leen mas de BUFFER_SIZE - 1 caracteres seguidos
    return EOF si se atopa o fin do ficheiro
*/
char nextCharFromSource(InputSystem *inputSystem);

/**
 * Devolve os caracteres leidos entre startPointer e dianteiro
 */
char* getReadCharacters(InputSystem *inputSystem);

/*
    Funciones para retroceder frontPointer e poder recuperar caracteres xa leidos
*/
void goBackNCharacters(InputSystem *inputSystem, int n);
void goBack1Character(InputSystem *inputSystem);

// Funcion para adiantar startPointer ata frontPointer e ignorar os caracteres leidos
void matchPointers(InputSystem *inputSystem);

#endif /* INPUTSYSTEM_H */