#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sisEntrada.h"

void _cargarBuffer(char *buffer, FILE *file){
    size_t totalRead = fread(buffer, sizeof(char), TAM_BUFFER - 1, file);

    if(totalRead < TAM_BUFFER - 1){
        buffer[totalRead+1] = EOF;
    }
}

SistemaEntrada* inicializarSistemaEntrada(const char *filename){
    SistemaEntrada *sistemaEntrada = (SistemaEntrada*)malloc(sizeof(SistemaEntrada));
    sistemaEntrada->filename = (char*)malloc((strlen(filename) + 1)*sizeof(char));
    sistemaEntrada->stats = (SE_Stats*)malloc(sizeof(SE_Stats));

    strcpy(sistemaEntrada->filename, filename);
    sistemaEntrada->file = fopen(filename, "r");

    if(sistemaEntrada->file == NULL){
        return NULL;
    }
    
    sistemaEntrada->bufferA[TAM_BUFFER-1] = EOF;
    sistemaEntrada->bufferB[TAM_BUFFER-1] = EOF;

    _cargarBuffer(sistemaEntrada->bufferA, sistemaEntrada->file);

    sistemaEntrada->inicio = &(sistemaEntrada->bufferA[0]);
    sistemaEntrada->delantero = &(sistemaEntrada->bufferA[0]);

    sistemaEntrada->diffPunteros = 0;
    sistemaEntrada->bufferActual = BUFFER_A;

    sistemaEntrada->cargarBuffer = true;

    sistemaEntrada->stats->line = 1;

    return sistemaEntrada;
}

void liberarSistemaEntrada(SistemaEntrada *sistemaEntrada){
    fclose(sistemaEntrada->file);
    free(sistemaEntrada->filename);
    free(sistemaEntrada->stats);
    free(sistemaEntrada);
}

char* _currentBuffer(SistemaEntrada *sistemaEntrada){
    int currentBuffer = sistemaEntrada->bufferActual;
    return currentBuffer == BUFFER_A? sistemaEntrada->bufferA: sistemaEntrada->bufferB;
}

char* _nextBuffer(SistemaEntrada *sistemaEntrada){
    int currentBuffer = sistemaEntrada->bufferActual;
    return currentBuffer == BUFFER_A? sistemaEntrada->bufferB: sistemaEntrada->bufferA;
}

char seguinteCaracter(SistemaEntrada *sistemaEntrada){
    char *buffer = _currentBuffer(sistemaEntrada);

    char charActual = *(sistemaEntrada->delantero);

    //stats
    if(charActual == '\n'){
        sistemaEntrada->stats->line++;
    }

    sistemaEntrada->delantero++;
    sistemaEntrada->diffPunteros++;

    if(sistemaEntrada->diffPunteros > (TAM_BUFFER - 1)){
        return ERR_LEXEMA_EXCEDE_TAM_MAX;
    }

    if(*(sistemaEntrada->delantero) == EOF){
        if(sistemaEntrada->delantero == (buffer + TAM_BUFFER - 1)){
            if(sistemaEntrada->cargarBuffer){
                _cargarBuffer(_nextBuffer(sistemaEntrada), sistemaEntrada->file);
            }
            sistemaEntrada->delantero = _nextBuffer(sistemaEntrada);
            sistemaEntrada->bufferActual = sistemaEntrada->bufferActual == BUFFER_A? BUFFER_B: BUFFER_A;
            sistemaEntrada->cargarBuffer = true;
        }else{
            return EOF;
        }
    }
    
    return charActual;
}

char* getCaracteresLeidos(SistemaEntrada *sistemaEntrada){
    int diffPointers = sistemaEntrada->diffPunteros;
    char *str = (char*)malloc(sizeof(char)*diffPointers + 1);
    
    char *bufferDelantero = _currentBuffer(sistemaEntrada);
    int positionDelantero = sistemaEntrada->delantero - bufferDelantero;
    int nBytesFromBufferDelantero = positionDelantero + 1;

    char *bufferInicio = positionDelantero - diffPointers >= 0? bufferDelantero: _nextBuffer(sistemaEntrada);
    int positionInicio = sistemaEntrada->inicio - bufferInicio;
    int nBytesFromBufferInicio = TAM_BUFFER - 1 - positionInicio;

    if(diffPointers <= nBytesFromBufferInicio){
        memcpy(str, sistemaEntrada->inicio, diffPointers);
        str[diffPointers] = '\0';
    }else{
        memcpy(str, sistemaEntrada->inicio, nBytesFromBufferInicio);
        memcpy(str + nBytesFromBufferInicio, bufferDelantero, nBytesFromBufferDelantero);
        str[diffPointers] = '\0';
    }
    
    sistemaEntrada->inicio = sistemaEntrada->delantero;
    sistemaEntrada->diffPunteros = 0;
    
    return str;
}

void retrocederNcaracteres(SistemaEntrada *sistemaEntrada, int n){    
    char *bufferDelantero = _currentBuffer(sistemaEntrada);
    
    if(sistemaEntrada->delantero - n - bufferDelantero < 0){
        char *nextBuffer = _nextBuffer(sistemaEntrada);
        sistemaEntrada->bufferActual = sistemaEntrada->bufferActual == BUFFER_A? BUFFER_B: BUFFER_A;
        int offset = TAM_BUFFER - 1 + (sistemaEntrada->delantero - n - bufferDelantero);
        sistemaEntrada->delantero = nextBuffer + offset;
        sistemaEntrada->cargarBuffer = false;
    }else{
        sistemaEntrada->delantero -= n;
    }
    sistemaEntrada->diffPunteros -= n;

    //stats
    if(*(sistemaEntrada->delantero) == '\n'){
        sistemaEntrada->stats->line--;
    }
}

void retroceder1caracter(SistemaEntrada *sistemaEntrada){
    retrocederNcaracteres(sistemaEntrada, 1);
}

void emparellarPunteiros(SistemaEntrada *sistemaEntrada){
    sistemaEntrada->inicio = sistemaEntrada->delantero;
    sistemaEntrada->diffPunteros = 0;
}