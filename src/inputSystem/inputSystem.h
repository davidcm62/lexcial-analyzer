#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 83
#define BUFFER_A 0
#define BUFFER_B 1
#define ERR_LEXEME_MAX_SIZE -2

/**
 * Estadísticas do sistema de entrada
 */
typedef struct {
    unsigned long line;
} IS_Stats;

/**
 * Sistema de estrada
 */
typedef struct {
    char *filename;
    FILE *file;
    
    char bufferA[BUFFER_SIZE];
    char bufferB[BUFFER_SIZE];
    
    char *frontPointer;     //delantero
    char *startPointer;     //inicio
    
    int diffPointers;       //distancia entre punteiros
    int currentBuffer;      //buffer actual (A ou B)

    bool loadNextBuffer;    //flag para cargar ou non o seguinte buffer

    IS_Stats *stats;
} InputSystem;

/**
 * Inicializa o sistema de entrada
 */
InputSystem* initInputSystem(const char *filename);

/**
 * Libera o sistema de entrada
 */
void freeInputSystem(InputSystem *inputSystem);

/**
 *  return char co seguinte caracter
 *  return ERR_LEXEME_MAX_SIZE si se leen mas de BUFFER_SIZE - 1 caracteres seguidos
 *  return EOF si se atopa o fin do ficheiro
 */
char nextCharFromSource(InputSystem *inputSystem);

/**
 * Devolve os caracteres leidos entre startPointer e dianteiro
 */
char* getReadCharacters(InputSystem *inputSystem);

/**
 * Funciones para retroceder inicio e poder recuperar caracteres xa leidos
 */
void goBackNCharacters(InputSystem *inputSystem, int n);
void goBack1Character(InputSystem *inputSystem);

/**
 * Adianta inicio ata delantero e permite ignorar os caracteres que queden no medio
 */
void matchPointers(InputSystem *inputSystem);

#endif /* INPUTSYSTEM_H */