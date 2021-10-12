#ifndef SISENTRADA_H
#define SISENTRADA_H

#include <stdbool.h>

#define TAM_BUFFER 83
#define BUFFER_A 0
#define BUFFER_B 1
#define ERR_LEXEMA_EXCEDE_TAM_MAX -2

typedef struct {
    FILE *file;
    
    char bufferA[TAM_BUFFER];
    char bufferB[TAM_BUFFER];
    
    char *delantero;
    char *inicio;
    
    int diffPunteros;
    int bufferActual;

    bool cargarBuffer;
} SistemaEntrada;

SistemaEntrada* inicializarSistemaEntrada(const char *filename);
void liberarSistemaEntrada(SistemaEntrada *sistemaEntrada);

/*
    return char co seguinte caracter
    return ERR_LEXEMA_EXCEDE_TAM_MAX si se leen mas de TAM_BUFFER - 1 caracteres seguidos
    return EOF si se atopa o fin do ficheiro
*/
char seguinteCaracter(SistemaEntrada *sistemaEntrada);
char* getCaracteresLeidos(SistemaEntrada *sistemaEntrada);

/*
    Funciones para retroceder delantero e poder recuperar caracteres xa leidos
*/
void retrocederNcaracteres(SistemaEntrada *sistemaEntrada, int n);
void retroceder1caracter(SistemaEntrada *sistemaEntrada);

// Funcion para adiantar inicio ata delantero e ignorar os caracteres leidos
void emparellarPunteiros(SistemaEntrada *sistemaEntrada);

#endif /* SISENTRADA_H */