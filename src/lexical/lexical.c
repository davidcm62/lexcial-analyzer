#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexical.h"
#include "../common/definitions.h"

#define INITIAL_STATE 0
#define ERROR_STATE 99
#define LEXICAL_ERROR_STATE 100
#define SIZE_ERROR_STATE 101
#define FINAL_STATE 102
#define ALPHANUMERIC_STATE 1
#define ONE_LINE_COMMENT_STATE 2
#define NEWLINE_STATE 3
#define OP_OR_DELIM_1_CHAR_STATE 4
#define SINGLE_QUOTE_STRING_STATE 5
#define DOUBLE_QUOTE_STRING_STATE 6
// === estados internos automata comillas dobles ===
#define DOUBLE_QUOTE_STRING_2_NOT_EMPTY_STATE 7
#define DOUBLE_QUOTE_STRING_2_OR_3_STATE 8
#define DOUBLE_QUOTE_STRING_3_STEP1_STATE 9
#define DOUBLE_QUOTE_STRING_3_STEP2_STATE 10
#define DOUBLE_QUOTE_STRING_3_STEP3_STATE 11
// =================================================
#define EOF_STATE 12
#define POINT_AND_NUMBER_STATE 13
// === estados internos automata numeros ===
#define POINT_READ_STATE 18
#define NUM_GT_0_READ_STATE 19
#define ZERO_READ_STATE 20
#define ZEROX_READ_STATE 21
#define ZEROX_NUM_READ_STATE 22
#define NUM_EXP_READ_STATE 23
#define NUM_POINT_READ_STATE 24
#define NUM_POINT_NUM_READ_STATE 25
#define EXP_NEGATIVE_POSITIVE_READ_STATE 26
#define VALID_EXP_READ_STATE 27
// =========================================
#define OP_OR_DELIM_2_CHAR_STATE 28

LexicalComponent* initLexicalComponent(){
    return (LexicalComponent*)malloc(sizeof(LexicalComponent));
}

void _initValuesLexicalComponent(LexicalComponent *lexicalComponent, char *lexeme, int lexicalCompNum){
    lexicalComponent->lexeme = lexeme;
    lexicalComponent->lexicalComp = lexicalCompNum;
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

LexicalResult _automatonAlphanumericStrings(TS *ts, InputSystem *inputSystem, LexicalComponent *lexicalComponent){
    bool keepSearching = true;
    unsigned int state = INITIAL_STATE;
    char currentChar;
    char *lexeme = NULL;
    int *tsValue;
    int lexicalCompNum = IDENTIFIER;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    while(keepSearching){
        switch (state){
            case INITIAL_STATE:
                currentChar = nextCharFromSource(inputSystem);

                if(currentChar == ERR_LEXEME_MAX_SIZE){
                    state = ERROR_STATE;
                } else if(!isalnum(currentChar) && currentChar != '_'){
                    state = FINAL_STATE;
                }
                break;
            case ERROR_STATE:
                _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
                keepSearching = false;
                break;
            case FINAL_STATE:
                goBack1Character(inputSystem);
                lexeme = getReadCharacters(inputSystem);
                
                // TS
                tsValue = searchTS(*ts, lexeme);
                if(tsValue == NULL){
                    insertTS(ts, lexeme, lexicalCompNum);
                }else{
                    lexicalCompNum = *tsValue;
                }
                
                _initValuesLexicalComponent(lexicalComponent, lexeme, lexicalCompNum);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automatonOneLineComments(InputSystem *inputSystem){
    LexicalResult lexicalResult = LEXICAL_ERROR;
    bool keepSearching = true;
    unsigned int state = INITIAL_STATE;
    char currentChar;
    
    while(keepSearching){
        switch (state){
            case INITIAL_STATE:
                currentChar = nextCharFromSource(inputSystem);

                if(currentChar != '\n' && currentChar != EOF){
                    matchPointers(inputSystem);
                }else{
                    state = FINAL_STATE;
                }
                break;
            case FINAL_STATE:
                goBack1Character(inputSystem);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automatonNewLine(InputSystem *inputSystem, LexicalComponent *lexicalComponent){
    char *lexeme = getReadCharacters(inputSystem);
    _initValuesLexicalComponent(lexicalComponent, lexeme, NEWLINE);
    return SUCCESS;
}

LexicalResult _automatonOperatorOrDelimiter1Char(InputSystem *inputSystem, LexicalComponent *lexicalComponent){
    char *lexeme = getReadCharacters(inputSystem);
    _initValuesLexicalComponent(lexicalComponent, lexeme, (int)lexeme[0]);
    return SUCCESS;
}

LexicalResult _automatonSingleQuoteString(InputSystem *inputSystem, LexicalComponent *lexicalComponent){
    bool keepSearching = true;
    unsigned int state = INITIAL_STATE;
    char currentChar;
    char *lexeme = NULL;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    while(keepSearching){
        switch (state){
            case INITIAL_STATE:
                currentChar = nextCharFromSource(inputSystem);

                if(currentChar == ERR_LEXEME_MAX_SIZE){
                    state = ERROR_STATE;
                } else if(currentChar == '\''){
                    state = FINAL_STATE;
                } else if(currentChar == EOF){
                    goBack1Character(inputSystem);
                    state = FINAL_STATE;
                }
                break;
            case ERROR_STATE:
                _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
                keepSearching = false;
                break;
            case FINAL_STATE:
                lexeme = getReadCharacters(inputSystem);
                _initValuesLexicalComponent(lexicalComponent, lexeme, STRING);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automatonDoubleQuoteString(InputSystem *inputSystem, LexicalComponent *lexicalComponent){
    bool keepSearching = true;
    unsigned int state = INITIAL_STATE;
    char currentChar;
    char *lexeme = NULL;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    while(keepSearching){
        if(state != FINAL_STATE && state != ERROR_STATE){
            currentChar = nextCharFromSource(inputSystem);
        }
        
        if(currentChar == ERR_LEXEME_MAX_SIZE){
            _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
            return lexicalResult;
        }
        
        switch (state){
            case INITIAL_STATE:
                state = currentChar == '"'? DOUBLE_QUOTE_STRING_2_OR_3_STATE : DOUBLE_QUOTE_STRING_2_NOT_EMPTY_STATE;
                break;
            case DOUBLE_QUOTE_STRING_2_NOT_EMPTY_STATE: 
                // reconoce cadenas do estilo: "algo"
                if(currentChar == '"'){
                    state = FINAL_STATE;
                }else if(currentChar == EOF){
                    state = ERROR_STATE;
                }
                break;
            case DOUBLE_QUOTE_STRING_2_OR_3_STATE:
                // ata aqui levase reconocido ""
                // diferencia entre cadenas cadenas de triple """ e vacías ""
                state = currentChar == '"'? DOUBLE_QUOTE_STRING_3_STEP1_STATE : FINAL_STATE;
                break;
            case DOUBLE_QUOTE_STRING_3_STEP1_STATE:
                //reconoce cadenas """ algo "
                if(currentChar == '"'){
                    state = DOUBLE_QUOTE_STRING_3_STEP2_STATE;
                } else if (currentChar == EOF) {
                    state = ERROR_STATE;
                }
                break;
            case DOUBLE_QUOTE_STRING_3_STEP2_STATE:
                //reconoce """ algo ""
                state = currentChar == '"'? DOUBLE_QUOTE_STRING_3_STEP3_STATE : ERROR_STATE;
                break;
            case DOUBLE_QUOTE_STRING_3_STEP3_STATE:
                //reconoce """ algo """
                state = currentChar == '"'? FINAL_STATE : ERROR_STATE;
                break;
            case ERROR_STATE:
                goBack1Character(inputSystem);
                _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
                lexicalResult = LEXICAL_ERROR;
                keepSearching = false;
                break;
            case FINAL_STATE:
                lexeme = getReadCharacters(inputSystem);
                _initValuesLexicalComponent(lexicalComponent, lexeme, STRING);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult _automatonEOF(InputSystem *inputSystem, LexicalComponent *lexicalComponent){
    matchPointers(inputSystem);
    char *dollar = (char*)malloc(sizeof(char)*2);
    strcpy(dollar, "$");
    _initValuesLexicalComponent(lexicalComponent, dollar, EOF_COMP);
    return SUCCESS;
}

int _charToNumber(char c){
    return c - '0';
}

bool _charBetweenAandF(char c){
    return tolower(c) >= 'a' && tolower(c) <= 'f';
}

LexicalResult _automatonPointAndNumbers(InputSystem *inputSystem, LexicalComponent *lexicalComponent, char firstChar){
    bool keepSearching = true;
    unsigned int state = -1;
    char currentChar;
    char *lexeme;
    int lexicalCompNum = INTEGER;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    if(firstChar == '.'){
        state = POINT_READ_STATE;
    } else if(isdigit(firstChar) && _charToNumber(firstChar) > 0){
        state = NUM_GT_0_READ_STATE;
    } else if(isdigit(firstChar) && firstChar == '0'){
        state = ZERO_READ_STATE;
    }
    
    while(keepSearching){
        if(state != FINAL_STATE && state != ERROR_STATE){
            currentChar = nextCharFromSource(inputSystem);
        }
        
        if(currentChar == ERR_LEXEME_MAX_SIZE){
            _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
            return lexicalResult;
        }

        switch (state){
            case POINT_READ_STATE: //leido ., detecta . e .2
                if(isdigit(currentChar)){
                    state = NUM_POINT_NUM_READ_STATE;
                }else{
                    lexicalCompNum = (int)'.';
                    state = FINAL_STATE;
                }
                break;
            case NUM_GT_0_READ_STATE: //leido [1-9], detecta 6, 66, 6e, 6.
                if(isdigit(currentChar)){
                    continue;
                } else if(currentChar == 'e'){
                    state = NUM_EXP_READ_STATE;
                } else if(currentChar == '.'){
                    state = NUM_POINT_READ_STATE;
                } else{
                    state = FINAL_STATE;
                }
                break;
            case ZERO_READ_STATE: //leido 0, detecta 0, 0x, 0. ou 0e
                if(tolower(currentChar) == 'x'){
                    state = ZEROX_READ_STATE;
                }else if(currentChar == 'e'){
                    state = NUM_EXP_READ_STATE;
                }else if(currentChar == '.'){
                    state = NUM_POINT_READ_STATE;
                }else{
                    state = FINAL_STATE;
                }
                break;
            case ZEROX_READ_STATE: //leido 0x, detecta 0x[a-fA-F0-9]
                if(isdigit(currentChar) || _charBetweenAandF(currentChar)){
                    state = ZEROX_NUM_READ_STATE;
                } else {
                    state = ERROR_STATE;
                }
                break;
            case ZEROX_NUM_READ_STATE: //detecta 0x1...
                if(!isdigit(currentChar) && !_charBetweenAandF(currentChar)){
                    state = FINAL_STATE;
                }
                break;
            case NUM_EXP_READ_STATE: // leido 123e
                lexicalCompNum = FLOAT;
                
                if(isdigit(currentChar)){
                    state = VALID_EXP_READ_STATE;
                } else if(currentChar == '+' || currentChar == '-'){
                    state = EXP_NEGATIVE_POSITIVE_READ_STATE;
                } else {
                    state = ERROR_STATE;
                }
                break;
            case NUM_POINT_READ_STATE: // leido 123.
                lexicalCompNum = FLOAT;   
                state = isdigit(currentChar)? NUM_POINT_NUM_READ_STATE : FINAL_STATE;
                break;
            case NUM_POINT_NUM_READ_STATE: // leido 123.1 ou .23 ou 12.
                lexicalCompNum = FLOAT;
                
                if(isdigit(currentChar)){
                    continue;
                } else if(currentChar == 'e'){
                    state = NUM_EXP_READ_STATE;
                } else{
                    state = FINAL_STATE;
                }
                break;
            case EXP_NEGATIVE_POSITIVE_READ_STATE: //leido 2e- ou 2e+
                state = isdigit(currentChar)? VALID_EXP_READ_STATE : ERROR_STATE;
                break;
            case VALID_EXP_READ_STATE: //detecta 2e+3 2e-1 2e123
                if(!isdigit(currentChar)){
                    state = FINAL_STATE;
                }
                break;
            case ERROR_STATE:
                goBack1Character(inputSystem);
                _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
                lexicalResult = LEXICAL_ERROR;
                keepSearching = false;
                break;
            case FINAL_STATE:
                goBack1Character(inputSystem);
                lexeme = getReadCharacters(inputSystem);
                _initValuesLexicalComponent(lexicalComponent, lexeme, lexicalCompNum);
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

LexicalResult _automatonOperatorOrDelimiter2Char(InputSystem *inputSystem, LexicalComponent *lexicalComponent, char firstChar){
    char secondChar = nextCharFromSource(inputSystem);
    char *lexeme;
    int lexicalCompNum = _checkOpOrDelim2Char(firstChar, secondChar);

    if(lexicalCompNum == -2){
        goBack1Character(inputSystem);
        _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
        return LEXICAL_ERROR;
    }
    
    if(lexicalCompNum == -1){
        goBack1Character(inputSystem);
    }
    
    lexeme = getReadCharacters(inputSystem);
    _initValuesLexicalComponent(lexicalComponent, lexeme, lexicalCompNum == -1? (int)firstChar : lexicalCompNum);

    return SUCCESS;
}

LexicalResult nextLexicalComponent(TS *ts, InputSystem *inputSystem, LexicalComponent *lexicalComponent){
    bool keepSearching = true;
    unsigned int state = INITIAL_STATE;
    char currentChar;
    LexicalResult lexicalResult = LEXICAL_ERROR;
    LexicalResult internalResult;

    while(keepSearching){
        switch (state){
            case INITIAL_STATE:
                currentChar = nextCharFromSource(inputSystem);

                if(currentChar == ERR_LEXEME_MAX_SIZE){
                    state = SIZE_ERROR_STATE;
                } else if(isalpha(currentChar) || currentChar == '_'){
                    state = ALPHANUMERIC_STATE;
                } else if (currentChar == '#'){
                    state = ONE_LINE_COMMENT_STATE;
                } else if (currentChar == '\n'){
                    state = NEWLINE_STATE;
                } else if (_isOperatorOrDelimiter1Char(currentChar)){
                    state = OP_OR_DELIM_1_CHAR_STATE;
                } else if (currentChar == '\''){
                    state = SINGLE_QUOTE_STRING_STATE;
                } else if (currentChar == '"'){
                    state = DOUBLE_QUOTE_STRING_STATE;
                } else if (isdigit(currentChar) || currentChar == '.'){
                    state = POINT_AND_NUMBER_STATE;
                } else if (_isOperatorOrDelimiter2Char(currentChar)){
                    state = OP_OR_DELIM_2_CHAR_STATE;
                } else if (currentChar == ' ' || currentChar == '\t' || currentChar == '\\'){
                    matchPointers(inputSystem);
                } else if (currentChar == EOF){
                    state = EOF_STATE;
                } else{
                    // simbolo non recoñecidos
                    _initValuesLexicalComponent(lexicalComponent, getReadCharacters(inputSystem), INVALID_LEXICAL_COMPONENT);
                    state = LEXICAL_ERROR_STATE;
                }
                break;
            case ALPHANUMERIC_STATE:
                internalResult = _automatonAlphanumericStrings(ts, inputSystem, lexicalComponent);
                state = internalResult == SUCCESS? FINAL_STATE : SIZE_ERROR_STATE;
                break;
            case ONE_LINE_COMMENT_STATE:
                internalResult = _automatonOneLineComments(inputSystem);                
                state = internalResult == SUCCESS? INITIAL_STATE : LEXICAL_ERROR_STATE;
                break;
            case NEWLINE_STATE:     
                _automatonNewLine(inputSystem, lexicalComponent);
                state = FINAL_STATE;
                break;
            case OP_OR_DELIM_1_CHAR_STATE:     
                _automatonOperatorOrDelimiter1Char(inputSystem, lexicalComponent);
                state = FINAL_STATE;
                break;
            case OP_OR_DELIM_2_CHAR_STATE:     
                internalResult = _automatonOperatorOrDelimiter2Char(inputSystem, lexicalComponent, currentChar);
                state = internalResult == SUCCESS? FINAL_STATE : LEXICAL_ERROR_STATE;
                break;
            case SINGLE_QUOTE_STRING_STATE:     
                internalResult = _automatonSingleQuoteString(inputSystem, lexicalComponent);
                state = internalResult == SUCCESS? FINAL_STATE : SIZE_ERROR_STATE;
                break;
            case DOUBLE_QUOTE_STRING_STATE:     
                internalResult = _automatonDoubleQuoteString(inputSystem, lexicalComponent);
                state = internalResult == SUCCESS? FINAL_STATE : internalResult == LEXEME_TOO_LONG? SIZE_ERROR_STATE : LEXICAL_ERROR_STATE;
                break;
            case POINT_AND_NUMBER_STATE:  
                internalResult = _automatonPointAndNumbers(inputSystem, lexicalComponent, currentChar);
                state = internalResult == SUCCESS? FINAL_STATE : internalResult == LEXEME_TOO_LONG? SIZE_ERROR_STATE : LEXICAL_ERROR_STATE;
                break;
            case EOF_STATE:
                _automatonEOF(inputSystem, lexicalComponent);
                state = FINAL_STATE;
                break;
            case LEXICAL_ERROR_STATE:
                lexicalResult = LEXICAL_ERROR;
                keepSearching = false;
                break;
            case SIZE_ERROR_STATE:
                lexicalResult = LEXEME_TOO_LONG;
                keepSearching = false;
                break;
            case FINAL_STATE:
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

void freeLexicalComponent(LexicalComponent *lexicalComponent){
    if(lexicalComponent != NULL){
        free(lexicalComponent->lexeme);
        free(lexicalComponent);
    }
}