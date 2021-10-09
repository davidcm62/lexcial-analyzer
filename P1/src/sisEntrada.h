#ifndef SISENTRADA_H
#define SISENTRADA_H

#define TAM_BUFFER 300
#define BUFFER_A 0
#define BUFFER_B 1

typedef struct {
    char bufferA[TAM_BUFFER];
    char bufferB[TAM_BUFFER];
    char *delantero;
    char *inicio;
    FILE *file;
    int bufferActual;
} SistemaEntrada;

SistemaEntrada* inicializarSistemaEntrada(const char *filename);
void liberarSistemaEntrada(SistemaEntrada *sistemaEntrada);

char seguinteCaracter(SistemaEntrada *sistemaEntrada);
char* devolverLeidoTotal(SistemaEntrada *sistemaEntrada);

void retrocederNcaracteres(SistemaEntrada *sistemaEntrada, int n);
void retroceder1caracter(SistemaEntrada *sistemaEntrada);

#endif /* SISENTRADA_H */