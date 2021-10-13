#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexico.h"
#include "../common/definiciones.h"

#define ESTADO_INICIAL 0
#define ESTADO_ERROR 99
#define ESTADO_ERROR_LEXICO 100
#define ESTADO_ERROR_TAM 101
#define ESTADO_FINAL 102
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

CompLexico* initCompLexico(){
    return (CompLexico*)malloc(sizeof(CompLexico));
}

void _initValuesCompLexico(CompLexico *compLexico, char *lexema, int compLexicoNum){
    compLexico->lexema = lexema;
    compLexico->compLexico = compLexicoNum;
}

bool _isOperatorOrDelimiter1Char(char element){
    const char matches[] = {'(', ')', '[', ']', '{', '}', ',', ';', '~'};
    
    for(int i=0; i<sizeof(matches)/sizeof(char); i++){
        if(matches[i] == element){
            return true;
        }
    }

    return false;
}

bool _isOperatorOrDelimiter2Char(char element){
    const char matches[] = {'!', '%', '&', '*', '+', '-', '/', ':', '<', '=', '>', '@', '^', '|'};
    
    for(int i=0; i<sizeof(matches)/sizeof(char); i++){
        if(matches[i] == element){
            return true;
        }
    }

    return false;
}

LexicalResult _automataCadenasAlfanumericas(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada, CompLexico *compLexico){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    char *lexema = NULL;
    int *tsValue;
    int compLexicoNum = IDENTIFICADOR;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                } else if(!isalnum(currentChar) && currentChar != '_'){
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_ERROR:
                _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                retroceder1caracter(sistemaEntrada);
                lexema = getCaracteresLeidos(sistemaEntrada);
                
                // TS
                tsValue = searchTS(*tablaSimbolos, lexema);
                if(tsValue == NULL){
                    insertTS(tablaSimbolos, lexema, compLexicoNum);
                }else{
                    compLexicoNum = *tsValue;
                }
                
                _initValuesCompLexico(compLexico, lexema, compLexicoNum);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automataComentario1Linea(SistemaEntrada *sistemaEntrada){
    LexicalResult lexicalResult = LEXICAL_ERROR;
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    
    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar != '\n' && currentChar != EOF){
                    emparellarPunteiros(sistemaEntrada);
                }else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_FINAL:
                retroceder1caracter(sistemaEntrada);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automataNewLine(SistemaEntrada *sistemaEntrada, CompLexico *compLexico){
    char *lexema = getCaracteresLeidos(sistemaEntrada);
    _initValuesCompLexico(compLexico, lexema, NEWLINE);
    return SUCCESS;
}

LexicalResult _automataOperatorOrDelimiter1Char(SistemaEntrada *sistemaEntrada, CompLexico *compLexico){
    char *lexema = getCaracteresLeidos(sistemaEntrada);
    _initValuesCompLexico(compLexico, lexema, (int)lexema[0]);
    return SUCCESS;
}

LexicalResult _automataStringsComillaSimple(SistemaEntrada *sistemaEntrada, CompLexico *compLexico){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    char *lexema = NULL;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR;
                } else if(currentChar == '\''){
                    state = ESTADO_FINAL;
                } else if(currentChar == EOF){
                    retroceder1caracter(sistemaEntrada);
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_ERROR:
                _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                lexema = getCaracteresLeidos(sistemaEntrada);
                _initValuesCompLexico(compLexico, lexema, STRING);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automataStringsComillaDoble(SistemaEntrada *sistemaEntrada, CompLexico *compLexico){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    char *lexema = NULL;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    while(keepSearching){
        if(state != ESTADO_FINAL && state != ESTADO_ERROR){
            currentChar = seguinteCaracter(sistemaEntrada);
        }
        
        if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
            _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
            return lexicalResult;
        }
        
        switch (state){
            case ESTADO_INICIAL:
                state = currentChar == '"'? ESTADO_STRING_COMILLA_DOBLE_2_OU_3 : ESTADO_STRING_COMILLA_DOBLE_2_NON_VACIA;
                break;
            case ESTADO_STRING_COMILLA_DOBLE_2_NON_VACIA: 
                // reconoce cadenas do estilo: "algo"
                if(currentChar == '"'){
                    state = ESTADO_FINAL;
                }else if(currentChar == EOF){
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_STRING_COMILLA_DOBLE_2_OU_3:
                // ata aqui levase reconocido ""
                // diferencia entre cadenas cadenas de triple """ e vacías ""
                state = currentChar == '"'? ESTADO_STRING_COMILLA_DOBLE_3_PASO1 : ESTADO_FINAL;
                break;
            case ESTADO_STRING_COMILLA_DOBLE_3_PASO1:
                //reconoce cadenas """ algo "
                if(currentChar == '"'){
                    state = ESTADO_STRING_COMILLA_DOBLE_3_PASO2;
                } else if (currentChar == EOF) {
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_STRING_COMILLA_DOBLE_3_PASO2:
                //reconoce """ algo ""
                state = currentChar == '"'? ESTADO_STRING_COMILLA_DOBLE_3_PASO3 : ESTADO_ERROR;
                break;
            case ESTADO_STRING_COMILLA_DOBLE_3_PASO3:
                //reconoce """ algo """
                state = currentChar == '"'? ESTADO_FINAL : ESTADO_ERROR;
                break;
            case ESTADO_ERROR:
                retroceder1caracter(sistemaEntrada);
                _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
                lexicalResult = LEXICAL_ERROR;
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                lexema = getCaracteresLeidos(sistemaEntrada);
                _initValuesCompLexico(compLexico, lexema, STRING);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automataFinFicheiro(SistemaEntrada *sistemaEntrada, CompLexico *compLexico){
    emparellarPunteiros(sistemaEntrada);
    char *dollar = (char*)malloc(sizeof(char)*2);
    strcpy(dollar, "$");
    _initValuesCompLexico(compLexico, dollar, FIN_FICHEIRO);
    return SUCCESS;
}

int _charToNumber(char c){
    return c - '0';
}

bool _charBetweenAandF(char c){
    return tolower(c) >= 'a' && tolower(c) <= 'f';
}

LexicalResult _automataPuntoAndNumeros(SistemaEntrada *sistemaEntrada, CompLexico *compLexico, char firstChar){
    bool keepSearching = true;
    unsigned int state = -1;
    char currentChar;
    char *lexema;
    int compLexicoNum = INTEGER;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    if(firstChar == '.'){
        state = ESTADO_LEIDO_PUNTO;
    } else if(isdigit(firstChar) && _charToNumber(firstChar) > 0){
        state = ESTADO_LEIDO_NUM_GT_0;
    } else if(isdigit(firstChar) && firstChar == '0'){
        state = ESTADO_LEIDO_0;
    }
    
    while(keepSearching){
        if(state != ESTADO_FINAL && state != ESTADO_ERROR){
            currentChar = seguinteCaracter(sistemaEntrada);
        }
        
        if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
            _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
            return lexicalResult;
        }

        switch (state){
            case ESTADO_LEIDO_PUNTO: //leido ., detecta . e .2
                if(isdigit(currentChar)){
                    state = ESTADO_LEIDO_NUM_PUNTO_NUM;
                }else{
                    compLexicoNum = (int)'.';
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_NUM_GT_0: //leido [1-9], detecta 6, 66, 6e, 6.
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
                if(isdigit(currentChar) || _charBetweenAandF(currentChar)){
                    state = ESTADO_LEIDO_0X_NUM;
                } else {
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_LEIDO_0X_NUM: //detecta 0x1...
                if(!isdigit(currentChar) && !_charBetweenAandF(currentChar)){
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_NUM_EXP: // leido 123e
                compLexicoNum = FLOAT;
                
                if(isdigit(currentChar)){
                    state = ESTADO_LEIDO_EXP_VALIDO;
                } else if(currentChar == '+' || currentChar == '-'){
                    state = ESTADO_LEIDO_EXP_MENOS_OU_MAS;
                } else {
                    state = ESTADO_ERROR;
                }
                break;
            case ESTADO_LEIDO_NUM_PUNTO: // leido 123.
                compLexicoNum = FLOAT;   
                state = isdigit(currentChar)? ESTADO_LEIDO_NUM_PUNTO_NUM : ESTADO_FINAL;
                break;
            case ESTADO_LEIDO_NUM_PUNTO_NUM: // leido 123.1 ou .23 ou 12.
                compLexicoNum = FLOAT;
                
                if(isdigit(currentChar)){
                    continue;
                } else if(currentChar == 'e'){
                    state = ESTADO_LEIDO_NUM_EXP;
                } else{
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_LEIDO_EXP_MENOS_OU_MAS: //leido 2e- ou 2e+
                state = isdigit(currentChar)? ESTADO_LEIDO_EXP_VALIDO : ESTADO_ERROR;
                break;
            case ESTADO_LEIDO_EXP_VALIDO: //detecta 2e+3 2e-1 2e123
                if(!isdigit(currentChar)){
                    state = ESTADO_FINAL;
                }
                break;
            case ESTADO_ERROR:
                retroceder1caracter(sistemaEntrada);
                _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
                lexicalResult = LEXICAL_ERROR;
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                retroceder1caracter(sistemaEntrada);
                lexema = getCaracteresLeidos(sistemaEntrada);
                _initValuesCompLexico(compLexico, lexema, compLexicoNum);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

int _checkOpOrDelim2Char(char c1, char c2){
    switch (c1){
        case '!': return c2 == '='? NOT_EQUALS : -2;
        case '%': return c2 == '='? MOD_EQUALS : -1;
        case '&': return c2 == '='? AND_EQUALS : -1;
        case '*':
            switch (c2){
                case '*': return POW;    
                case '=': return MULT_EQUALS;
                default: return -1;
            }
        case '+':
            switch (c2){
                case '+': return INCREMENT;    
                case '=': return ADD_EQUALS;
                default: return -1;
            }
        case '-':
            switch (c2){
                case '-': return DECREMENT;    
                case '=': return SUB_EQUALS;
                case '>': return FUNCTION_NOTATION;
                default: return -1;
            }
        case '/':
            switch (c2){
                case '/': return FLOOR_DIV;    
                case '=': return DIV_EQUALS;
                default: return -1;
            }
        case ':': return c2 == '='? ASIGN_EVALUATE : -1;
        case '<':
            switch (c2){
                case '<': return LEFT_SHIFT;    
                case '=': return LESS_EQUALS;
                default: return -1;
            }
        case '=': return c2 == '='? EQUALS : -1;
        case '>':
            switch (c2){
                case '>': return RIGTH_SHIFT;    
                case '=': return GREATER_EQUALS;
                default: return -1;
            }
        case '@': return c2 == '='? MATRIX_EQUALS : -1;
        case '^': return c2 == '='? XOR_EQUALS : -1;
        case '|': return c2 == '='? OR_EQUALS : -1;
    }

    return -2;
}

LexicalResult _automataOperatorOrDelimiter2Char(SistemaEntrada *sistemaEntrada, CompLexico *compLexico, char firstChar){
    char secondChar = seguinteCaracter(sistemaEntrada);
    char *lexema;
    int compLexicoNum = _checkOpOrDelim2Char(firstChar, secondChar);

    if(compLexicoNum == -2){
        retroceder1caracter(sistemaEntrada);
        _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
        return LEXICAL_ERROR;
    }
    
    if(compLexicoNum == -1){
        retroceder1caracter(sistemaEntrada);
    }
    
    lexema = getCaracteresLeidos(sistemaEntrada);
    _initValuesCompLexico(compLexico, lexema, compLexicoNum == -1? (int)firstChar : compLexicoNum);

    return SUCCESS;
}

LexicalResult seguinteCompLexico(TS *tablaSimbolos, SistemaEntrada *sistemaEntrada, CompLexico *compLexico){
    bool keepSearching = true;
    unsigned int state = ESTADO_INICIAL;
    char currentChar;
    LexicalResult lexicalResult = LEXICAL_ERROR;
    LexicalResult internalResult;

    while(keepSearching){
        switch (state){
            case ESTADO_INICIAL:
                currentChar = seguinteCaracter(sistemaEntrada);

                if(currentChar == ERR_LEXEMA_EXCEDE_TAM_MAX){
                    state = ESTADO_ERROR_TAM;
                } else if(isalpha(currentChar) || currentChar == '_'){
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
                    // simbolo non recoñecidos
                    _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), INVALID_LEXICAL_COMPONENT);
                    state = ESTADO_ERROR_LEXICO;
                }
                break;
            case ESTADO_ALFANUMERICAS:
                internalResult = _automataCadenasAlfanumericas(tablaSimbolos, sistemaEntrada, compLexico);
                state = internalResult == SUCCESS? ESTADO_FINAL : ESTADO_ERROR_TAM;
                break;
            case ESTADO_COMENTARIO_1_LINEA:
                internalResult = _automataComentario1Linea(sistemaEntrada);                
                state = internalResult == SUCCESS? ESTADO_INICIAL : ESTADO_ERROR_LEXICO;
                break;
            case ESTADO_NEWLINE:     
                _automataNewLine(sistemaEntrada, compLexico);
                state = ESTADO_FINAL;
                break;
            case ESTADO_OP_OR_DELIM_1_CHAR:     
                _automataOperatorOrDelimiter1Char(sistemaEntrada, compLexico);
                state = ESTADO_FINAL;
                break;
            case ESTADO_OP_OR_DELIM_2_CHAR:     
                internalResult = _automataOperatorOrDelimiter2Char(sistemaEntrada, compLexico, currentChar);
                state = internalResult == SUCCESS? ESTADO_FINAL : ESTADO_ERROR_LEXICO;
                break;
            case ESTADO_STRING_COMILLA_SIMPLE:     
                internalResult = _automataStringsComillaSimple(sistemaEntrada, compLexico);
                state = internalResult == SUCCESS? ESTADO_FINAL : ESTADO_ERROR_TAM;
                break;
            case ESTADO_STRING_COMILLA_DOBLE:     
                internalResult = _automataStringsComillaDoble(sistemaEntrada, compLexico);
                state = internalResult == SUCCESS? ESTADO_FINAL : internalResult == LEXEME_TOO_LONG? ESTADO_ERROR_TAM : ESTADO_ERROR_LEXICO;
                break;
            case ESTADO_PUNTO_E_NUMEROS:  
                internalResult = _automataPuntoAndNumeros(sistemaEntrada, compLexico, currentChar);
                state = internalResult == SUCCESS? ESTADO_FINAL : internalResult == LEXEME_TOO_LONG? ESTADO_ERROR_TAM : ESTADO_ERROR_LEXICO;
                break;
            case ESTADO_FIN_FICHEIRO:
                _automataFinFicheiro(sistemaEntrada, compLexico);
                state = ESTADO_FINAL;
                break;
            case ESTADO_ERROR_LEXICO:
                // _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), -1);
                lexicalResult = LEXICAL_ERROR;
                keepSearching = false;
                break;
            case ESTADO_ERROR_TAM:
                // _initValuesCompLexico(compLexico, getCaracteresLeidos(sistemaEntrada), -1);
                lexicalResult = LEXEME_TOO_LONG;
                keepSearching = false;
                break;
            case ESTADO_FINAL:
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

void liberarCompLexico(CompLexico *compLexico){
    if(compLexico != NULL){
        free(compLexico->lexema);
        free(compLexico);
    }
}