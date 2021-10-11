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
// === estados internos automata comillas dobles ===
#define ESTADO_STRING_COMILLA_DOBLE_2_NON_VACIA 7
#define ESTADO_STRING_COMILLA_DOBLE_2_OU_3 8
#define ESTADO_STRING_COMILLA_DOBLE_3_PASO1 9
#define ESTADO_STRING_COMILLA_DOBLE_3_PASO2 10
#define ESTADO_STRING_COMILLA_DOBLE_3_PASO3 11
// =================================================
#define ESTADO_FIN_FICHEIRO 12
#define ESTADO_PUNTO_E_NUMEROS 13
// === estados internos automata numeros ===
#define ESTADO_PUNTO 17
#define ESTADO_LEIDO_PUNTO 18
#define ESTADO_LEIDO_NUM_GT_0 19
#define ESTADO_LEIDO_0 20
#define ESTADO_LEIDO_0X 21
#define ESTADO_LEIDO_0X_NUM 22
#define ESTADO_LEIDO_NUM_EXP 23
#define ESTADO_LEIDO_NUM_PUNTO 24
#define ESTADO_LEIDO_NUM_PUNTO_NUM 25
#define ESTADO_LEIDO_EXP_MENOS_OU_MAS 26
#define ESTADO_LEIDO_EXP_VALIDO 27
// =========================================
#define ESTADO_OP_OR_DELIM_2_CHAR 28

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

bool _isOperatorOrDelimiter2Char(char element){
    const char matches[] = {'=','*','+','<'};
    
    for(int i=0; i<sizeof(matches)/sizeof(char); i++){
        if(matches[i] == element){
            return true;
        }
    }

    return false;
}

CompLexico* _automataCadenasAlfanumericas(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
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
                lexema = getCaracteresLeidos(sistemaEntrada);
                //TODO: consultar ts, se é unha keyword devolver o complexcio, se é un id devolver _IDENTIFICADOR
                compLexico = _initCompLexico(lexema, IDENTIFICADOR);
                free(lexema);
                keepSearching = false;
                break;
        }
    }

    return compLexico;
}

bool _automataComentario1Linea(SistemaEntrada *sistemaEntrada){
    bool success = false;
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
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
    char *lexema = getCaracteresLeidos(sistemaEntrada);
    CompLexico *compLexico = _initCompLexico(lexema, NEWLINE);
    free(lexema);
    return compLexico;
}

CompLexico* _automataOperatorOrDelimiter1Char(SistemaEntrada *sistemaEntrada){
    char *lexema = getCaracteresLeidos(sistemaEntrada);
    CompLexico *compLexico = _initCompLexico(lexema, (int)lexema[0]);
    free(lexema);
    return compLexico;
}

CompLexico* _automataStringsComillaSimple(SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
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
                lexema = getCaracteresLeidos(sistemaEntrada);
                compLexico = _initCompLexico(lexema, STRING);
                free(lexema);
                keepSearching = false;
                break;
        }
    }

    return compLexico;
}

CompLexico* _automataStringsComillaDoble(SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    char *lexema = NULL;
    CompLexico *compLexico = NULL;

    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == '"'){
                    state = ESTADO_STRING_COMILLA_DOBLE_2_OU_3;
                }else{
                    state = ESTADO_STRING_COMILLA_DOBLE_2_NON_VACIA;
                }
                break;
            case ESTADO_STRING_COMILLA_DOBLE_2_NON_VACIA:
                // reconoce cadenas do estilo: "algo"
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(currentChar == '"'){
                    state = ESTADO_FINAL;
                }
                //TODO: comprobar esto
                // else if(currentChar == EOF){
                //     state = ESTADO_ERROR;
                // }
                break;
            case ESTADO_STRING_COMILLA_DOBLE_2_OU_3:
                // ata aqui levase reconocido ""
                // diferencia entre cadenas vacías "" e cadenas de triple """
                currentChar = seguinteCaracter(sistemaEntrada);
                
                if(currentChar == '"'){
                    // triple """
                    state = ESTADO_STRING_COMILLA_DOBLE_3_PASO1;
                }else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_STRING_COMILLA_DOBLE_3_PASO1:
                //reconoce cadenas """ aglo "
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(currentChar == '"'){
                    state = ESTADO_STRING_COMILLA_DOBLE_3_PASO2;
                } 
                // else {
                //     state = ESTADO_ERROR;
                // }
                break;
            case ESTADO_STRING_COMILLA_DOBLE_3_PASO2:
                //reconoce """ algo ""
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(currentChar == '"'){
                    state = ESTADO_STRING_COMILLA_DOBLE_3_PASO3;
                }else{
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_STRING_COMILLA_DOBLE_3_PASO3:
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
                //necesito ter 2 tipos de error: tam e cadena mal formada
                //como fago para devolver ese error?
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                lexema = getCaracteresLeidos(sistemaEntrada);
                compLexico = _initCompLexico(lexema, STRING);
                free(lexema);
                keepSearching = false;
                break;
        }
    }

    return compLexico;
}

CompLexico* _automataFinFicheiro(SistemaEntrada *sistemaEntrada){
    emparellarPunteiros(sistemaEntrada);
    CompLexico *compLexico = _initCompLexico("$", FIN_FICHEIRO);
    return compLexico;
}

int _charToNumber(char c){
    return c - '0';
}

int _charBetweenAandF(char c){
    return tolower(c) >= 'a' && tolower(c) <= 'f';
}

CompLexico* _automataPuntoAndNumeros(SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    char *lexema;
    int compLexicoNum = INTEGER;
    CompLexico *compLexico = NULL;

    retroceder1caracter(sistemaEntrada);
    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == '.'){
                    state = ESTADO_LEIDO_PUNTO;
                } else if(isdigit(currentChar) && _charToNumber(currentChar) > 0){
                    state = ESTADO_LEIDO_NUM_GT_0;
                } else if(isdigit(currentChar) && currentChar == '0'){
                    state = ESTADO_LEIDO_0;
                }
                break;
            case ESTADO_LEIDO_PUNTO: //leido ., detecta . e .2
                currentChar = seguinteCaracter(sistemaEntrada);

                if(isdigit(currentChar)){
                    state = ESTADO_LEIDO_NUM_PUNTO_NUM;
                }else{
                    compLexicoNum = (int)'.';
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_NUM_GT_0: //leido [1-9], detecta 6, 66, 6e, 6.
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }
                
                if(isdigit(currentChar)){
                    continue;
                } else if(currentChar == 'e'){
                    state = ESTADO_LEIDO_NUM_EXP;
                } else if(currentChar == '.'){
                    state = ESTADO_LEIDO_NUM_PUNTO;
                } else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_0: //leido 0, detecta 0, 0x, 0. ou 0e
                currentChar = seguinteCaracter(sistemaEntrada);
                
                if(tolower(currentChar) == 'x'){
                    state = ESTADO_LEIDO_0X;
                }else if(currentChar == 'e'){
                    state = ESTADO_LEIDO_NUM_EXP;
                }else if(currentChar == '.'){
                    state = ESTADO_LEIDO_NUM_PUNTO;
                }else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_0X: //leido 0x, detecta 0x[a-fA-F0-9]
                currentChar = seguinteCaracter(sistemaEntrada);
                
                if(isdigit(currentChar) || _charBetweenAandF(currentChar)){
                    state = ESTADO_LEIDO_0X_NUM;
                } else {
                    //mal formada
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_LEIDO_0X_NUM: //detecta 0x1...
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(!isdigit(currentChar) && !_charBetweenAandF(currentChar)){
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_NUM_EXP: // leido 123e
                compLexicoNum = FLOAT;
                
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(isdigit(currentChar)){
                    state = ESTADO_LEIDO_EXP_VALIDO;
                }else if(currentChar == '+' || currentChar == '-'){
                    state = ESTADO_LEIDO_EXP_MENOS_OU_MAS;
                } else {
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_LEIDO_NUM_PUNTO: // leido 123.
                compLexicoNum = FLOAT;
                
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(isdigit(currentChar)){
                    state = ESTADO_LEIDO_NUM_PUNTO_NUM;
                }else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_NUM_PUNTO_NUM: // leido 123.1 ou .23 ou 12.
                compLexicoNum = FLOAT;
                
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(isdigit(currentChar)){
                    continue;
                } else if(currentChar == 'e'){
                    state = ESTADO_LEIDO_NUM_EXP;
                } else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_EXP_MENOS_OU_MAS: //leido 2e- ou 2e+
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(isdigit(currentChar)){
                    state = ESTADO_LEIDO_EXP_VALIDO;
                } else {
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_LEIDO_EXP_VALIDO: //detecta 2e+3 2e-1 2e123
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                    continue;
                }

                if(!isdigit(currentChar)){
                    state = ESTADO_FINAL;
                }
                break;
            // case 60:
            //     break;
            // case 60:
            //     break;
            // case 60:
            //     break;
            // case 60:
            //     break;
            // case 60:
            //     break;
            case ESTADO_ERROR:
                keepSearching = false;
                break;
            // case ESTADO_FINAL_INTEGERS:
            //     //estado final ints
            //     compLexicoNum = INTEGER;
            //     state = ESTADO_FINAL;
            //     break;
            // case ESTADO_FINAL_FLOATS:
            //     //estado final floats
            //     compLexicoNum = FLOAT;
            //     state = ESTADO_FINAL;
            //     break;
            // case ESTADO_FINAL_PUNTO:
            //     //estado final punto
            //     compLexicoNum = (int)'.';
            //     state = ESTADO_FINAL;
            //     break;
            case ESTADO_FINAL:
                retroceder1caracter(sistemaEntrada);
                lexema = getCaracteresLeidos(sistemaEntrada);
                compLexico = _initCompLexico(lexema, compLexicoNum);
                free(lexema);
                keepSearching = false;
                break;
        }
    }

    return compLexico;
}

CompLexico* _automataOperatorOrDelimiter2Char(SistemaEntrada *sistemaEntrada, char firstChar){
    return NULL;
}

CompLexico* seguinteCompLexico(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    CompLexico *compLexico = NULL;

    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);
                // printf("::==[%c]\n",currentChar);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
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
                } else if (isdigit(currentChar) || currentChar == '.'){
                    state = ESTADO_PUNTO_E_NUMEROS;
                } else if (_isOperatorOrDelimiter2Char(currentChar)){
                    state = ESTADO_OP_OR_DELIM_2_CHAR;
                } else if (currentChar == ' ' || currentChar == '\t' || currentChar == '\\'){
                    emparellarPunteiros(sistemaEntrada);
                } else if (currentChar == EOF){
                    state = ESTADO_FIN_FICHEIRO;
                } else{
                    // emparellarPunteiros(sistemaEntrada);
                    // simbolo non recoñecido
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_ALFANUMERICAS:
                compLexico = _automataCadenasAlfanumericas(tablaSimbolos, sistemaEntrada);
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_COMENTARIO_1_LINEA:                
                state = _automataComentario1Linea(sistemaEntrada)? ESTADO_INICIAL : ESTADO_ERROR;
                break;
            case ESTADO_NEWLINE:     
                compLexico = _automataNewLine(sistemaEntrada);
                state = ESTADO_FINAL;
                break;
            case ESTADO_OP_OR_DELIM_1_CHAR:     
                compLexico = _automataOperatorOrDelimiter1Char(sistemaEntrada);
                state = ESTADO_FINAL;
                break;
            case ESTADO_OP_OR_DELIM_2_CHAR:     
                compLexico = _automataOperatorOrDelimiter2Char(sistemaEntrada, currentChar);
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_STRING_COMILLA_SIMPLE:     
                compLexico = _automataStringsComillaSimple(sistemaEntrada);
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_STRING_COMILLA_DOBLE:     
                compLexico = _automataStringsComillaDoble(sistemaEntrada);
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_PUNTO_E_NUMEROS:  
                compLexico = _automataPuntoAndNumeros(sistemaEntrada);
                state = compLexico != NULL? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_FIN_FICHEIRO:
                compLexico = _automataFinFicheiro(sistemaEntrada);
                state = ESTADO_FINAL;
                break;
            // case 22:

            //     break;
            // case 22:

            //     break;
            // case 22:

            //     break;
            // case 22:

            //     break;
            // case 22:

            //     break;
            // case 22:

            //     break;
            case ESTADO_ERROR:
                //combinar errores mal formacion con tam?
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