#include <stdlib.h>
#include <string.h>
#include "inputSystem.h"
#include "../error/error.h"

#define BUFFER_SIZE 83
#define BUFFER_A 0
#define BUFFER_B 1

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

//variable sistema de entrada
InputSystem *_inputSystem = NULL;

/**
 * Carga BUFFER_SIZE - 1 caracteres do ficheiro en *buffer
 */
void _loadBuffer(char *buffer){
    // leo BUFFER_SIZE - 1 chars do ficheiro
    size_t totalRead = fread(buffer, sizeof(char), BUFFER_SIZE - 1, _inputSystem->file);

    // se se leron menos chars dos solicitados, enton chegouse ao EOF
    // neste caso introduzo o EOF ao final dos caracteres leidos, no buffer
    if(totalRead < BUFFER_SIZE - 1){
        buffer[totalRead+1] = EOF;
    }
}

bool initInputSystem(const char *filename){
    //reserva de memoria
    _inputSystem = (InputSystem*)malloc(sizeof(InputSystem));
    _inputSystem->filename = (char*)malloc((strlen(filename) + 1)*sizeof(char));
    _inputSystem->stats = (IS_Stats*)malloc(sizeof(IS_Stats));

    //apertura do ficheiro
    strcpy(_inputSystem->filename, filename);
    _inputSystem->file = fopen(filename, "r");

    if(_inputSystem->file == NULL){
        //error ficheiro
        handleError(IO);
        return false;
    }
    
    //centinelas
    _inputSystem->bufferA[BUFFER_SIZE-1] = EOF;
    _inputSystem->bufferB[BUFFER_SIZE-1] = EOF;

    //cargo o buffer A
    _loadBuffer(_inputSystem->bufferA);

    //os punteiros incio e dianteiro apuntan ao inicio do primeiro buffer
    _inputSystem->startPointer = &(_inputSystem->bufferA[0]);
    _inputSystem->frontPointer = &(_inputSystem->bufferA[0]);

    _inputSystem->diffPointers = 0;
    _inputSystem->currentBuffer = BUFFER_A;

    _inputSystem->loadNextBuffer = true;

    _inputSystem->stats->line = 1;

    return true;
}

void freeInputSystem(){
    fclose(_inputSystem->file);
    free(_inputSystem->filename);
    free(_inputSystem->stats);
    free(_inputSystem);
}

/**
 * Devolve o buffer que se esta usando actualmente
 */
char* _currentBuffer(){
    int currentBuffer = _inputSystem->currentBuffer;
    return currentBuffer == BUFFER_A? _inputSystem->bufferA: _inputSystem->bufferB;
}

/**
 * Devolve o seguinte buffer que se ten que cargar
 */
char* _nextBuffer(){
    int currentBuffer = _inputSystem->currentBuffer;
    return currentBuffer == BUFFER_A? _inputSystem->bufferB: _inputSystem->bufferA;
}

char nextCharFromSourceInputSys(){
    //array buffer actual
    char *buffer = _currentBuffer();

    //caracter leido
    char currentChar = *(_inputSystem->frontPointer);

    //stats
    if(currentChar == '\n'){
        _inputSystem->stats->line++;
    }

    //avanzo delantero e a diferencia entre punteiros
    _inputSystem->frontPointer++;
    _inputSystem->diffPointers++;

    //superase o tamaño máximo de lexema? -> error
    if(_inputSystem->diffPointers > (BUFFER_SIZE - 1)){
        return ERR_LEXEME_MAX_SIZE;
    }

    //o caracter seguinte é EOF?
    if(*(_inputSystem->frontPointer) == EOF){
        if(_inputSystem->frontPointer == (buffer + BUFFER_SIZE - 1)){    //caso EOF é o fin de buffer
            
            if(_inputSystem->loadNextBuffer){   //en caso de ter que cargar o buffer
                _loadBuffer(_nextBuffer()); //cargo o buffer seguinte ao actual: actual A? B else A 
            }
            _inputSystem->frontPointer = _nextBuffer(); //delantero apunta ao incio do seguinte buffer
            _inputSystem->currentBuffer = _inputSystem->currentBuffer == BUFFER_A? BUFFER_B: BUFFER_A;  //actualizo a flag buffer actual
            _inputSystem->loadNextBuffer = true; //o seguinte EOF carga o buffer
        }else{  //caso EOF do ficheiro
            return EOF;
        }
    }
    
    return currentChar;
}

char* getReadCharactersInputSys(){
    int diffPointers = _inputSystem->diffPointers;
    // string para copiar os caracteres entre inicio e delantero
    char *str = (char*)malloc(sizeof(char)*diffPointers + 1);
    
    //calculo o buffer de delantero, o índice dentro do array e os bytes dende o inicio do array ata delantero
    char *frontPointerBuffer = _currentBuffer();
    int frontPointerPosition = _inputSystem->frontPointer - frontPointerBuffer;
    int nBytesFromFrontBuffer = frontPointerPosition + 1;

    //calculo o buffer onde está inicio, o índice de inicio e cantos bytes libres hai nese buffer
    char *startPointerBuffer = frontPointerPosition - diffPointers >= 0? frontPointerBuffer: _nextBuffer();
    int startPointerPosition = _inputSystem->startPointer - startPointerBuffer;
    int nBytesFromStartBuffer = BUFFER_SIZE - 1 - startPointerPosition;

    if(diffPointers <= nBytesFromStartBuffer){  //caso no que inicio e delantero estan no mesmo buffer
        memcpy(str, _inputSystem->startPointer, diffPointers);
        str[diffPointers] = '\0';
    }else{  //caso no que inicio esttá nun buffer e delantero no seguinte
        memcpy(str, _inputSystem->startPointer, nBytesFromStartBuffer);
        memcpy(str + nBytesFromStartBuffer, frontPointerBuffer, nBytesFromFrontBuffer);
        str[diffPointers] = '\0';
    }
    
    //avanzo incio ata dianteiro
    _inputSystem->startPointer = _inputSystem->frontPointer;
    _inputSystem->diffPointers = 0;
    
    return str;
}

void goBackNCharactersInputSys(int n){
    //buffer actual 
    char *frontPointerBuffer = _currentBuffer();

    if(_inputSystem->frontPointer - n - frontPointerBuffer < 0){ //caso no que retrocendo e cambio de buffer
        char *nextBuffer = _nextBuffer();
        _inputSystem->currentBuffer = _inputSystem->currentBuffer == BUFFER_A? BUFFER_B: BUFFER_A;
        // calculo a posición dentro do buffer anterior tendo en conta o retroceso dende o actual
        int offset = BUFFER_SIZE - 1 + (_inputSystem->frontPointer - n - frontPointerBuffer);
        _inputSystem->frontPointer = nextBuffer + offset;
        //o seguinte EOF non carga o buffer outra vez
        _inputSystem->loadNextBuffer = false;
    }else{  //caso no que retrocedo e quedo no mesmo buffer
        _inputSystem->frontPointer -= n;
    }
    _inputSystem->diffPointers -= n;

    //stats
    if(*(_inputSystem->frontPointer) == '\n'){
        _inputSystem->stats->line--;
    }
}

void goBack1CharacterInputSys(){
    goBackNCharactersInputSys(1);
}

void matchPointersInputSys(){
    //avanza inicio ata delantero
    _inputSystem->startPointer = _inputSystem->frontPointer;
    _inputSystem->diffPointers = 0;
}


int getCurrentLineInputSys(){
    return _inputSystem->stats->line;
}

char* getFilenameInputSys(){
    return _inputSystem->filename;
}