#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexico.h"
#include "definiciones.h"

// \ explicit line joining
// cadenas alfanumericas (ids keywords)
// integers
    // floating point 
// operadores
    // delimitadores
// strings simples
// sintngs dobles
// comentarios 1 linea e 3 lineas
// tabs?

// import scipy.stats as st
//     tmp = sorted(differences)
//     if N <= 30:

// char* alfanum(const char *input){
//     int estado = 0;
//     int actual = 0;
//     int analize = 1;

//     while (analize){
//         printf("%c[a:%d e:%d] ",input[actual],actual, estado);
//         switch (estado){
//             case 0:
//                 if(isalpha(input[actual]) || input[actual] == '_'){
//                     actual++;
//                     estado = 1;
//                 }else{
//                     estado = 2;
//                 }
//                 break;
//             case 1:
//                 if(isalnum(input[actual]) || input[actual] == '_'){
//                     actual++;
//                 }else{
//                     estado=3;
//                 }
//                 break;
//             case 2:
//                 //error
//                 analize = 0;
//                 break;
//             case 3:
//                 //fin
//                 analize = 0;
//                 actual--;
//                 break;
//         }
//     }
//     printf("\nactual %d   estado %d (%s)\n",actual,estado, estado==3? "final": "no final");
//     for(int i=0;i<=actual;i++){
//         printf("%c",input[i]);
//     }
//     printf("\n");
//     printf("\n");
    
//     // printf("%s\n",input);
//     return NULL;
// }

// void comentario1linea(const char *input){
//     int estado = 0;
//     int actual = 0;
//     int analize = 1;

//     printf("comentario?\n");
//     while (analize){
//         // printf("%c[a:%d e:%d] ",input[actual],actual, estado);
//         switch (estado){
//             case 0:
//                 if(input[actual] == '#'){
//                     actual++;
//                     estado = 1;
//                 }else{
//                     estado = 2;
//                 }
//                 break;
//             case 1:
//                 if(input[actual] == '\n'){
//                     estado = 3;
//                 }else{
//                     actual++;
//                 }
//                 break;
//             case 2:
//                 //error
//                 analize = 0;
//                 break;
//             case 3:
//                 //fin
//                 analize = 0;
//                 actual--;
//                 break;
//         }
//     }
//     printf("\nactual %d   estado %d (%s)\n",actual,estado, estado==3? "final": "no final");
//     for(int i=0;i<=actual;i++){
//         printf("%c",input[i]);
//     }
//     printf("\n");
//     printf("\n");
    
//     // printf("%s\n",input);
// }

// char* nextCharFromSourceFile(){
//     //opcion: ir devolvendo o char e o offset e co offset poderia volver ao principio
//     /*
//         poderia ir lendo o archivo, recibir ese offset como parametro
//         que cada automata controle canto lee
//         en caso de fallo retrocedo SEEK_CUR -posicion leidas
//         asi a proxima vez que chame a seguinteComp o cursor estará onde quedou a ultima vez
//     */
//     //poderia ter unha funcion en cada automata que sexa retrocedeXoffset no archivo cada vez que acabe
//     return NULL;
// }

// void automata(FILE *file){
//     bool found = false;
//     int estado = 0;
//     char *currentChar;
//     int compLexico = -1;
//     char *lexema = NULL;

//     while(!found){
//         switch (estado){
//             case 0:
//                 currentChar = seguinteCaracterCodigoFonte(file);

//                 //se é EOF levar a un estado error que acabe o bucle
//                 //if c==null error

//                 if(currentChar == NULL){
//                     estado = 11;
//                 }else{
//                     printf("%c\n",*currentChar);
//                 }
//                 free(currentChar);
//                 // if(isalpha(*currentChar) || *currentChar == '_'){
//                 //     estado = 1;
//                 // }else if(isdigit(*currentChar)){
//                 //     estado = 2;
//                 // }
//                 //etc
//                 break;
//             case 1:
//                 lexema = alfanum("asdasda");
//                 if(lexema != NULL){
//                     compLexico = 123123123;
//                     estado = 10; //final
//                 }else{
//                     //probaría outro caso (ainda que se chega aqui xa sei que sí vale)
//                 }
//                 break;
//             case 10:
//                 //meter comprobar e eso na TS
//                 found = true;
//                 break;
//             case 11:
//                 printf("ACABOSE\n");
//                 found = true;
//                 break;
//         }
//     }

//     // return do comp lexico ou null se houbo error
// }

#define ESTADO_INICIAL 0
#define ESTADO_ERROR 99
#define ESTADO_FINAL 100
#define ESTADO_ALFANUMERICAS 1
#define ESTADO_COMENTARIO_1_LINEA 2
#define ESTADO_NEWLINE 3

CompLexico* _initCompLexico(char *lexema, int compLexicoNum){
    CompLexico *compLexico = (CompLexico*)malloc(sizeof(CompLexico));
    compLexico->lexema = (char *)malloc(strlen(lexema) + 1);
    strcpy(compLexico->lexema, lexema);
    compLexico->compLexico = compLexicoNum;
    return compLexico;
}

bool _isOperatorOrDelimiter1Char(char element){
    const char matches[] = {'{','}',':','(',')',',','[',']','-','>','/'};
    
    for(int i=0; i<sizeof(matches)/sizeof(char); i++){
        if(matches[i] == element){
            return true;
        }
    }

    return false;
}

CompLexico* _automataCadenasAlfanumericas(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = 0;
    char currentChar;
    char *lexema = NULL;
    CompLexico *compLexico = NULL;

    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(!isalnum(currentChar) && currentChar != '_'){
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_ERROR:
                printf("ERROR\n");
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                retroceder1caracter(sistemaEntrada);
                lexema = devolverLeidoTotal(sistemaEntrada);
                //TODO: consultar ts, se é unha keyword devolver o complexcio, se é un id devolver _IDENTIFICADOR
                compLexico = _initCompLexico(lexema, IDENTIFICADOR);
                free(lexema);
                keepSearching = false;
                break;
        }
    }

    return compLexico;
}

bool _automataComentario1Linea(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada){
    bool success = false;
    bool keepSearching = true;
    unsigned int state = 0;
    char currentChar;
    
    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar != '\n'){
                    emparellarPunteiros(sistemaEntrada);
                }else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_FINAL:
                retroceder1caracter(sistemaEntrada);
                success = true;
                keepSearching = false;
                break;
        }
    }

    return success;
}

CompLexico* _automataNewLine(SistemaEntrada *sistemaEntrada){
    char *lexema = devolverLeidoTotal(sistemaEntrada);
    CompLexico *compLexico = _initCompLexico(lexema, NEWLINE);
    free(lexema);
    return compLexico;
}



CompLexico* seguinteCompLexico(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = 0;
    char currentChar;
    CompLexico *compLexico = NULL;

    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);
                printf("::==[%c]\n",currentChar);

                if(currentChar == EOF || currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(isalpha(currentChar) || currentChar == '_'){
                    state = ESTADO_ALFANUMERICAS;
                } else if (currentChar == '#'){
                    state = ESTADO_COMENTARIO_1_LINEA;
                } else if (currentChar == '\n'){
                    state = ESTADO_NEWLINE;
                } else if (currentChar == ' ' || currentChar == '\t' || currentChar == '\\'){
                    emparellarPunteiros(sistemaEntrada);
                } else{
                    // ignora
                    emparellarPunteiros(sistemaEntrada);
                    // state = ESTADO_ERROR;
                }
                break;
            case ESTADO_ALFANUMERICAS:
                compLexico = _automataCadenasAlfanumericas(tablaSimbolos, sistemaEntrada);
                
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_COMENTARIO_1_LINEA:                
                state = _automataComentario1Linea(tablaSimbolos, sistemaEntrada)? ESTADO_INICIAL : ESTADO_ERROR;
                break;
            case ESTADO_NEWLINE:     
                compLexico = _automataNewLine(sistemaEntrada);
                state = ESTADO_FINAL;
                break;
            //TODO: diferenciar estado_error de finde fichero, que non é error
            case ESTADO_ERROR:
                printf("error\n");
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                keepSearching = false;
                break;
        }
    }

    return compLexico;
}

void liberarCompLexico(CompLexico *compLexico){
    if(compLexico != NULL){
        free(compLexico->lexema);
        free(compLexico);
    }
}