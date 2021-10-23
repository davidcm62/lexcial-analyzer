#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexical.h"
#include "../inputSystem/inputSystem.h"
#include "../ts/TS.h"
#include "../common/definitions.h"
#include "../error/error.h"

/**
 * Reconoce cadeas alfanuméricas
 */
LexicalResult _automatonAlphanumericStrings(LexicalComponent *lexicalComponent){
    bool keepSearching = true;
    unsigned int state = INITIAL_STATE;
    char currentChar;
    char *lexeme = NULL;
    int *tsValue = NULL;
    int lexicalCompNum = IDENTIFIER;
    LexicalResult lexicalResult = LEXEME_TOO_LONG;

    while(keepSearching){
        switch (state){
            case INITIAL_STATE: //estado inicial
                currentChar = nextCharFromSourceInputSys();

                if(currentChar == ERR_LEXEME_MAX_SIZE){ //superase o tamaño máximo
                    state = ERROR_STATE;
                } else if(!isalnum(currentChar) && currentChar != '_'){ //se non é un caracter válido, cadea recoñecida
                    state = FINAL_STATE;
                }
                break;
            case ERROR_STATE:
                //en caso de erro, devolvo un compoñente léxico inválido e o valor leído
                _initValuesLexicalComponent(lexicalComponent, getReadCharactersInputSys(), INVALID_LEXICAL_COMPONENT);
                keepSearching = false;
                break;
            case FINAL_STATE:
                //retrocedo 1 caracter
                goBack1CharacterInputSys();
                lexeme = getReadCharactersInputSys();
                
                // TS
                tsValue = searchTS(lexeme);
                if(tsValue == NULL){    //non existe na TS, introduzoo
                    insertTS(lexeme, lexicalCompNum);
                }else{  //xa existe, devolvo o compoñente léxico gardado
                    lexicalCompNum = *tsValue;
                    free(tsValue);
                }
                
                _initValuesLexicalComponent(lexicalComponent, lexeme, lexicalCompNum);
                lexicalResult = SUCCESS;
                keepSearching = false;
                break;
        }
    }

    return lexicalResult;
}

LexicalResult nextLexicalComponent(LexicalComponent *lexicalComponent){
    bool keepSearching = true;
    unsigned int state = INITIAL_STATE;
    char currentChar;
    LexicalResult lexicalResult = LEXICAL_ERROR;
    LexicalResult internalResult;

    while(keepSearching){
        switch (state){
            case INITIAL_STATE: //distingo o autómata que é necesario chamar en función do seguinte caracter que se lee
                currentChar = nextCharFromSourceInputSys();

                if(currentChar == ERR_LEXEME_MAX_SIZE){
                    state = SIZE_ERROR_STATE; //error
                } else if(isalpha(currentChar) || currentChar == '_'){
                    state = ALPHANUMERIC_STATE; //cadeas alfanuméricas
                } else if (currentChar == '#'){
                    state = ONE_LINE_COMMENT_STATE; //cometarios de unha línea
                } else if (currentChar == '\n'){
                    state = NEWLINE_STATE; //salto de línea
                } else if (_isOperatorOrDelimiter1Char(currentChar)){
                    state = OP_OR_DELIM_1_CHAR_STATE; //operadores e delimitadores de 1 char
                } else if (currentChar == '\''){
                    state = SINGLE_QUOTE_STRING_STATE; //cadeas ''
                } else if (currentChar == '"'){
                    state = DOUBLE_QUOTE_STRING_STATE; //cadeas "" e """
                } else if (isdigit(currentChar) || currentChar == '.'){
                    state = POINT_AND_NUMBER_STATE; //numeros e punto
                } else if (_isOperatorOrDelimiter2Char(currentChar)){
                    state = OP_OR_DELIM_2_CHAR_STATE; //operadores e delimitadores de 2 char
                } else if (currentChar == ' ' || currentChar == '\t' || currentChar == '\\'){
                    matchPointersInputSys(); //ignorase o caracter
                } else if (currentChar == EOF){
                    state = EOF_STATE; //fin de ficheiro
                } else{
                    // simbolo non recoñecidos -> error
                    _initValuesLexicalComponent(lexicalComponent, getReadCharactersInputSys(), INVALID_LEXICAL_COMPONENT);
                    state = LEXICAL_ERROR_STATE;
                }
                break;
            case ALPHANUMERIC_STATE:
                internalResult = _automatonAlphanumericStrings(lexicalComponent);
                state = internalResult == SUCCESS? FINAL_STATE : SIZE_ERROR_STATE;
                break;
            case ONE_LINE_COMMENT_STATE:
                internalResult = _automatonOneLineComments();                
                state = internalResult == SUCCESS? INITIAL_STATE : LEXICAL_ERROR_STATE;
                break;
            case NEWLINE_STATE:     
                _automatonNewLine(lexicalComponent);
                state = FINAL_STATE;
                break;
            case OP_OR_DELIM_1_CHAR_STATE:     
                _automatonOperatorOrDelimiter1Char(lexicalComponent);
                state = FINAL_STATE;
                break;
            case OP_OR_DELIM_2_CHAR_STATE:     
                internalResult = _automatonOperatorOrDelimiter2Char(lexicalComponent, currentChar);
                state = internalResult == SUCCESS? FINAL_STATE : LEXICAL_ERROR_STATE;
                break;
            case SINGLE_QUOTE_STRING_STATE:     
                internalResult = _automatonSingleQuoteString(lexicalComponent);
                state = internalResult == SUCCESS? FINAL_STATE : internalResult == LEXEME_TOO_LONG? SIZE_ERROR_STATE : LEXICAL_ERROR_STATE;
                break;
            case DOUBLE_QUOTE_STRING_STATE:     
                internalResult = _automatonDoubleQuoteString(lexicalComponent);
                state = internalResult == SUCCESS? FINAL_STATE : internalResult == LEXEME_TOO_LONG? SIZE_ERROR_STATE : LEXICAL_ERROR_STATE;
                break;
            case POINT_AND_NUMBER_STATE:  
                internalResult = _automatonPointAndNumbers(lexicalComponent, currentChar);
                state = internalResult == SUCCESS? FINAL_STATE : internalResult == LEXEME_TOO_LONG? SIZE_ERROR_STATE : LEXICAL_ERROR_STATE;
                break;
            case EOF_STATE:
                _automatonEOF(lexicalComponent);
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
    
    //se se produce un erro, imprimo o erro
    if(lexicalResult != SUCCESS){
        handleErrorWithFileStats(LEXICAL, lexicalComponent->lexeme, getFilenameInputSys(), getCurrentLineInputSys());
    }

    return lexicalResult;
}

void freeLexicalComponent(LexicalComponent *lexicalComponent){
    if(lexicalComponent != NULL){
        free(lexicalComponent->lexeme);
        free(lexicalComponent);
    }
}