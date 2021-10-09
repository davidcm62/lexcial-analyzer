#include <stdio.h>
#include <stdlib.h>
#include "sisEntrada.h"

void _cargarBuffer(char buffer[TAM_BUFFER], FILE *file){
    size_t totalRead = fread(buffer, sizeof(char), TAM_BUFFER - 1, file);

    if(totalRead < TAM_BUFFER - 1){
        buffer[totalRead+1] = EOF;
    }

    printf("[");
    for (size_t i = 0; i < TAM_BUFFER; i++){
        printf("'%c', ",buffer[i] != EOF? buffer[i] : '&');
    }
    printf("]\n");
    printf("%d %c\n",totalRead,buffer[144]);

}

SistemaEntrada* inicializarSistemaEntrada(const char *filename){
    SistemaEntrada *sistemaEntrada = (SistemaEntrada*)malloc(sizeof(SistemaEntrada));

    sistemaEntrada->bufferA[TAM_BUFFER-1] = EOF;
    sistemaEntrada->bufferB[TAM_BUFFER-1] = EOF;

    sistemaEntrada->file = fopen(filename, "r");

    _cargarBuffer(sistemaEntrada->bufferA, sistemaEntrada->file);

    sistemaEntrada->inicio = &(sistemaEntrada->bufferA[0]);
    sistemaEntrada->delantero = &(sistemaEntrada->bufferA[0]);

    return sistemaEntrada;
}

void liberarSistemaEntrada(SistemaEntrada *sistemaEntrada){
    free(sistemaEntrada);
}

char seguinteCaracter(SistemaEntrada *sistemaEntrada){
    char read = *(sistemaEntrada->delantero);

    sistemaEntrada->delantero++;

    if(*(sistemaEntrada->delantero) == EOF){
        printf("SEGUINTE BLOQUE? EOF?");
    }
    printf("diff pointers -> %d [1.%c 2.%c]\n", sistemaEntrada->delantero - sistemaEntrada->inicio,*(sistemaEntrada->inicio),*(sistemaEntrada->delantero));

    return read;
}

char* devolverLeidoTotal(SistemaEntrada *sistemaEntrada){
    int diffPointers = sistemaEntrada->delantero - sistemaEntrada->inicio;
    sistemaEntrada->inicio = sistemaEntrada->delantero;

    return NULL;
}

void retrocederNcaracteres(SistemaEntrada *sistemaEntrada, int n){

}

void retroceder1caracter(SistemaEntrada *sistemaEntrada){

}