#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexico.h"
#include "definiciones.h"

#define ESTADO_INICIAL 0
#define ESTADO_ERROR 99
#define ESTADO_FINAL 100
#define ESTADO_ALFANUMERICAS 1
#define ESTADO_COMENTARIO_1_LINEA 2
#define ESTADO_NEWLINE 3
#define ESTADO_OP_OR_DELIM_1_CHAR 4
#define ESTADO_STRING_COMILLA_SIMPLE 5
#define ESTADO_STRING_COMILLA_DOBLE 6

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

CompLexico* _automataOperatorOrDelimiter1Char(SistemaEntrada *sistemaEntrada){
    char *lexema = devolverLeidoTotal(sistemaEntrada);
    CompLexico *compLexico = _initCompLexico(lexema, (int)lexema[0]);
    free(lexema);
    return compLexico;
}

CompLexico* _automataStringsComillaSimple(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada){
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

                if(currentChar == '\''){
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_ERROR:
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                lexema = devolverLeidoTotal(sistemaEntrada);
                compLexico = _initCompLexico(lexema, STRING);
                free(lexema);
                keepSearching = false;
                break;
        }
    }

    return compLexico;
}

CompLexico* _automataStringsComillaDoble(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = 0;
    char currentChar;
    char *lexema = NULL;
    CompLexico *compLexico = NULL;

    //TODO: cambiar os estados 1,etc.. a ctes
    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == '"'){
                    state = 2;
                }else{
                    state = 1;
                }
                break;
            case 1:
                // reconoce cadenas do estilo: "algo"
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(currentChar == '"'){
                    state = ESTADO_FINAL;
                }
                break;
            case 2:
                // ata aqui levase reconocido ""
                // diferencia entre cadenas vacías "" e cadenas de triple """
                currentChar = seguinteCaracter(sistemaEntrada);
                
                if(currentChar == '"'){
                    // triple """
                    state = 3;
                }else{
                    state = ESTADO_FINAL;
                }
                break;
            case 3:
                //reconoce cadenas """ aglo "
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(currentChar == '"'){
                    state = 4;
                }
                break;
            case 4:
                //reconoce """ algo ""
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(currentChar == '"'){
                    state = 5;
                }else{
                    state = ESTADO_ERROR;
                }
                break;
            case 5:
                //reconoce """ algo """
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(currentChar == '"'){
                    state = ESTADO_FINAL;
                }else{
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_ERROR:
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                lexema = devolverLeidoTotal(sistemaEntrada);
                compLexico = _initCompLexico(lexema, STRING);
                free(lexema);
                keepSearching = false;
                break;
        }
    }

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
                // printf("::==[%c]\n",currentChar);

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
                } else if (_isOperatorOrDelimiter1Char(currentChar)){
                    state = ESTADO_OP_OR_DELIM_1_CHAR;
                } else if (currentChar == '\''){
                    state = ESTADO_STRING_COMILLA_SIMPLE;
                } else if (currentChar == '"'){
                    state = ESTADO_STRING_COMILLA_DOBLE;
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
            case ESTADO_OP_OR_DELIM_1_CHAR:     
                compLexico = _automataOperatorOrDelimiter1Char(sistemaEntrada);
                state = ESTADO_FINAL;
                break;
            case ESTADO_STRING_COMILLA_SIMPLE:     
                compLexico = _automataStringsComillaSimple(tablaSimbolos, sistemaEntrada);
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_STRING_COMILLA_DOBLE:     
                compLexico = _automataStringsComillaDoble(tablaSimbolos, sistemaEntrada);
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
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