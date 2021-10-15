#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <stdio.h>
#include <stdbool.h>

#define ERR_LEXEME_MAX_SIZE -2

/**
 * Inicializa o sistema de entrada
 * Devolve true se se puido abrir o ficheiro
 */
bool initInputSystem(const char *filename);

/**
 * Libera o sistema de entrada
 */
void freeInputSystem();

/**
 *  return char co seguinte caracter
 *  return ERR_LEXEME_MAX_SIZE si se leen mas de BUFFER_SIZE - 1 caracteres seguidos
 *  return EOF si se atopa o fin do ficheiro
 */
char nextCharFromSourceInputSys();

/**
 * Devolve os caracteres leidos entre startPointer e dianteiro
 */
char* getReadCharactersInputSys();

/**
 * Funciones para retroceder inicio e poder recuperar caracteres xa leidos
 */
void goBackNCharactersInputSys(int n);
void goBack1CharacterInputSys();

/**
 * Adianta inicio ata delantero e permite ignorar os caracteres que queden no medio
 */
void matchPointersInputSys();

/**
 * Devolve a línea actual que se está lendo
 */
int getCurrentLineInputSys();

/**
 * Devolve o nome do ficheiro que se está lendo
 */
char* getFilenameInputSys();

#endif /* INPUTSYSTEM_H */