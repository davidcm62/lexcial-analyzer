#ifndef SISENTRADA_H
#define SISENTRADA_H

#include <stdbool.h>

#define TAM_BUFFER 35
#define BUFFER_A 0
#define BUFFER_B 1
#define ERR_MAX_LEXEMA_SIS_ENTRADA -2

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
    return ERR_MAX_LEXEMA_SIS_ENTRADA si se leen mas de TAM_BUFFER - 1 caracteres seguidos
    return EOF si se atopa o fin do ficheiro
*/
char seguinteCaracter(SistemaEntrada *sistemaEntrada);
char* devolverLeidoTotal(SistemaEntrada *sistemaEntrada);

void retrocederNcaracteres(SistemaEntrada *sistemaEntrada, int n);
void retroceder1caracter(SistemaEntrada *sistemaEntrada);

#endif /* SISENTRADA_H */