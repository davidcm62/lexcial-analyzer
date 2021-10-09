#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sisEntrada.h"

void _printSistemaEntrada(SistemaEntrada sistemaEntrada){    
    printf("=SISTEMA ENTRADA=\n");
    printf("[");
    for (size_t i = 0; i < sizeof(sistemaEntrada.bufferA)/sizeof(char); i++){
        if(sistemaEntrada.bufferA[i] != EOF){
            // printf("%c,",sistemaEntrada.bufferA[i] == '\n'? '?':sistemaEntrada.bufferA[i]);
        }else{
            // printf("EOF, ");
        }
    }
    printf("]\n");
    printf("inicio: [%c]\n",*sistemaEntrada.inicio);
    printf("delantero: [%c]\n",*sistemaEntrada.delantero);
    printf("diff: [%d]\n",sistemaEntrada.delantero - sistemaEntrada.inicio);
    printf("buffer actual: [%d]\n",sistemaEntrada.bufferActual);
    printf("=================\n");
    
}

void _cargarBuffer(char *buffer, FILE *file){
    size_t totalRead = fread(buffer, sizeof(char), TAM_BUFFER - 1, file);

    if(totalRead < TAM_BUFFER - 1){
        buffer[totalRead+1] = EOF;
    }

    // printf("[");
    // for (size_t i = 0; i < TAM_BUFFER; i++){
    //     printf("'%c', ",buffer[i] != EOF? buffer[i] : '&');
    // }
    // printf("]\n");
    // printf("%d %c\n",totalRead,buffer[144]);

}

SistemaEntrada* inicializarSistemaEntrada(const char *filename){
    SistemaEntrada *sistemaEntrada = (SistemaEntrada*)malloc(sizeof(SistemaEntrada));

    sistemaEntrada->bufferA[TAM_BUFFER-1] = EOF;
    sistemaEntrada->bufferB[TAM_BUFFER-1] = EOF;

    sistemaEntrada->file = fopen(filename, "r");

    _cargarBuffer(sistemaEntrada->bufferA, sistemaEntrada->file);

    sistemaEntrada->inicio = &(sistemaEntrada->bufferA[0]);
    sistemaEntrada->delantero = &(sistemaEntrada->bufferA[0]);

    sistemaEntrada->bufferActual = BUFFER_A;

    return sistemaEntrada;
}

void liberarSistemaEntrada(SistemaEntrada *sistemaEntrada){
    fclose(sistemaEntrada->file);
    free(sistemaEntrada);
}

char seguinteCaracter(SistemaEntrada *sistemaEntrada){
    char read = *(sistemaEntrada->delantero);

    sistemaEntrada->delantero++;

    // printf("[%c]\n",*(sistemaEntrada->delantero));
    // _printSistemaEntrada(*sistemaEntrada);
    if(*(sistemaEntrada->delantero) == EOF){
        if(sistemaEntrada->delantero == (sistemaEntrada->bufferA + TAM_BUFFER - 1)){
            printf("cargar bloque\n");
        }else{
            printf("FIN FICHERO\n");
        }
        // printf("%p %p %d\n",sistemaEntrada->delantero, sistemaEntrada->bufferA + TAM_BUFFER - 1,sistemaEntrada->delantero == (sistemaEntrada->bufferA + TAM_BUFFER - 1));
    }
    // printf("diff pointers -> %d [1.%c 2.%c]\n", sistemaEntrada->delantero - sistemaEntrada->inicio,*(sistemaEntrada->inicio),*(sistemaEntrada->delantero));
    // 0 1 2 3
    return read;
}

char* devolverLeidoTotal(SistemaEntrada *sistemaEntrada){
    int diffPointers = sistemaEntrada->delantero - sistemaEntrada->inicio;
    char *str = (char*)malloc(sizeof(char)*diffPointers + 1);
    
    // printf("diff pointers -> %d [1.%c 2.%c]\n", sistemaEntrada->delantero - sistemaEntrada->inicio,*(sistemaEntrada->inicio),*(sistemaEntrada->delantero));
    memcpy(str, sistemaEntrada->inicio, diffPointers);
    str[diffPointers] = '\0';

    // printf("%d %d %d\n",sistemaEntrada->inicio < sistemaEntrada->delantero,sistemaEntrada->inicio == sistemaEntrada->delantero,sistemaEntrada->inicio > sistemaEntrada->delantero);
    sistemaEntrada->inicio = sistemaEntrada->delantero;
    
    
    return str;
}

void retrocederNcaracteres(SistemaEntrada *sistemaEntrada, int n){
    sistemaEntrada->delantero -= n;
}

void retroceder1caracter(SistemaEntrada *sistemaEntrada){
    retrocederNcaracteres(sistemaEntrada, 1);
}