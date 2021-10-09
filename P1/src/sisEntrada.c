#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sisEntrada.h"

void _printSistemaEntrada(SistemaEntrada sistemaEntrada){    
    printf("=SISTEMA ENTRADA=\n");
    printf("[");
    for (size_t i = 0; i < sizeof(sistemaEntrada.bufferA)/sizeof(char); i++){
        if(sistemaEntrada.bufferA[i] != EOF){
            printf("%c,",sistemaEntrada.bufferA[i] == '\n'? '?':sistemaEntrada.bufferA[i]);
        }else{
            printf("EOF, ");
        }
    }
    printf("]\n");
    printf("[");
    for (size_t i = 0; i < sizeof(sistemaEntrada.bufferB)/sizeof(char); i++){
        if(sistemaEntrada.bufferB[i] != EOF){
            printf("%c,",sistemaEntrada.bufferB[i] == '\n'? '?':sistemaEntrada.bufferB[i]);
        }else{
            printf("EOF, ");
        }
    }
    printf("]\n");
    if(*sistemaEntrada.inicio != EOF){
        printf("inicio: [%c]\n",*sistemaEntrada.inicio);
    }else{
        printf("inicio: [EOF]\n");
    }
    if(*sistemaEntrada.delantero != EOF){
        printf("delantero: [%c]\n",*sistemaEntrada.delantero);
    }else{
        printf("delantero: [EOF]\n");
    }
    printf("diff: [%d]\n",sistemaEntrada.diffPunteros);
    printf("buffer actual: [%d]\n",sistemaEntrada.bufferActual);
    printf("=================\n");
    
}

void _cargarBuffer(char *buffer, FILE *file){
    size_t totalRead = fread(buffer, sizeof(char), TAM_BUFFER - 1, file);

    if(totalRead < TAM_BUFFER - 1){
        buffer[totalRead+1] = EOF;
    }
}

SistemaEntrada* inicializarSistemaEntrada(const char *filename){
    SistemaEntrada *sistemaEntrada = (SistemaEntrada*)malloc(sizeof(SistemaEntrada));

    sistemaEntrada->file = fopen(filename, "r");
    
    sistemaEntrada->bufferA[TAM_BUFFER-1] = EOF;
    sistemaEntrada->bufferB[TAM_BUFFER-1] = EOF;

    _cargarBuffer(sistemaEntrada->bufferA, sistemaEntrada->file);

    sistemaEntrada->inicio = &(sistemaEntrada->bufferA[0]);
    sistemaEntrada->delantero = &(sistemaEntrada->bufferA[0]);

    sistemaEntrada->diffPunteros = 0;
    sistemaEntrada->bufferActual = BUFFER_A;

    return sistemaEntrada;
}

void liberarSistemaEntrada(SistemaEntrada *sistemaEntrada){
    fclose(sistemaEntrada->file);
    free(sistemaEntrada);
}

char seguinteCaracter(SistemaEntrada *sistemaEntrada){
    char *buffer = sistemaEntrada->bufferActual == BUFFER_A? sistemaEntrada->bufferA: sistemaEntrada->bufferB;

    char charActual = *(sistemaEntrada->delantero);

    sistemaEntrada->delantero++;
    sistemaEntrada->diffPunteros++;

    // printf("-----------------------\n");
    // printf("[%c]\n",*(sistemaEntrada->delantero));
    if(*(sistemaEntrada->delantero) == EOF){
        if(sistemaEntrada->delantero == (buffer + TAM_BUFFER - 1)){
            // printf("\n\n\ncargar bloque\n\n\n");
            _cargarBuffer(sistemaEntrada->bufferActual == BUFFER_A? sistemaEntrada->bufferB: sistemaEntrada->bufferA, sistemaEntrada->file);
            sistemaEntrada->delantero = sistemaEntrada->bufferActual == BUFFER_A? sistemaEntrada->bufferB: sistemaEntrada->bufferA;
            sistemaEntrada->bufferActual = sistemaEntrada->bufferActual == BUFFER_A? BUFFER_B: BUFFER_A;
        }else{
            printf("\n\n\nFIN FICHERO\n\n\n");
            return EOF;
        }
        // printf("%p %p %d\n",sistemaEntrada->delantero, sistemaEntrada->bufferA + TAM_BUFFER - 1,sistemaEntrada->delantero == (sistemaEntrada->bufferA + TAM_BUFFER - 1));
    }
    // _printSistemaEntrada(*sistemaEntrada);
    // printf("diff pointers -> %d [1.%c 2.%c]\n", sistemaEntrada->delantero - sistemaEntrada->inicio,*(sistemaEntrada->inicio),*(sistemaEntrada->delantero));
    // 0 1 2 3
    return charActual;
}

char* devolverLeidoTotal(SistemaEntrada *sistemaEntrada){
    // int diffPointers = sistemaEntrada->delantero - sistemaEntrada->inicio;
    // char *str = (char*)malloc(sizeof(char)*diffPointers + 1);
    
    // // printf("diff pointers -> %d [1.%c 2.%c]\n", sistemaEntrada->delantero - sistemaEntrada->inicio,*(sistemaEntrada->inicio),*(sistemaEntrada->delantero));
    // memcpy(str, sistemaEntrada->inicio, diffPointers);
    // str[diffPointers] = '\0';

    // // printf("%d %d %d\n",sistemaEntrada->inicio < sistemaEntrada->delantero,sistemaEntrada->inicio == sistemaEntrada->delantero,sistemaEntrada->inicio > sistemaEntrada->delantero);
    // sistemaEntrada->inicio = sistemaEntrada->delantero;
    // // printf("%d %d %d\n",sistemaEntrada->inicio < sistemaEntrada->delantero,sistemaEntrada->inicio == sistemaEntrada->delantero,sistemaEntrada->inicio > sistemaEntrada->delantero);
    
    int diffPointers = sistemaEntrada->diffPunteros;
    char *str = (char*)malloc(sizeof(char)*diffPointers + 1);
    
    // memcpy(str, sistemaEntrada->inicio, diffPointers);
    // str[diffPointers] = '\0';

    // ===============
    // _printSistemaEntrada(*sistemaEntrada);
    char *bufferDelantero = sistemaEntrada->bufferActual == BUFFER_A? sistemaEntrada->bufferA: sistemaEntrada->bufferB;
    int positionDelantero = sistemaEntrada->delantero - bufferDelantero;
    int nBytesFromBufferDelantero = positionDelantero + 1;

    char *bufferInicio = positionDelantero - diffPointers >= 0? bufferDelantero: (sistemaEntrada->bufferActual == BUFFER_A? sistemaEntrada->bufferB: sistemaEntrada->bufferA);
    int positionInicio = sistemaEntrada->inicio - bufferInicio;
    int nBytesFromBufferInicio = TAM_BUFFER - 1 - positionInicio;

    // printf("%p %p\n",bufferInicio, bufferDelantero);
    // printf("\n%d %d %d %d\n",positionInicio, positionDelantero, diffPointers,nBytesFromBufferInicio);
    if(diffPointers <= nBytesFromBufferInicio){
        memcpy(str, sistemaEntrada->inicio, diffPointers);
        str[diffPointers] = '\0';
    }else{
        memcpy(str, sistemaEntrada->inicio, nBytesFromBufferInicio);
        memcpy(str + nBytesFromBufferInicio, bufferDelantero, nBytesFromBufferDelantero);
        str[diffPointers] = '\0';
    }
    // ===============

    sistemaEntrada->inicio = sistemaEntrada->delantero;
    sistemaEntrada->diffPunteros = 0;
    
    return str;
}

void retrocederNcaracteres(SistemaEntrada *sistemaEntrada, int n){    
    sistemaEntrada->delantero -= n;
}

void retroceder1caracter(SistemaEntrada *sistemaEntrada){
    retrocederNcaracteres(sistemaEntrada, 1);
}