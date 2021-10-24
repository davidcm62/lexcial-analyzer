#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.h"
#include "definitions.h"

char* _escapeChars(const char *str) {
    //reemplaza \n por \\n e \t por \\t
    int i, j;
    char *newStr;

    for (i = j = 0; i < strlen(str); i++) {
        if (str[i] == '\n' || str[i] == '\t') j++;
    }
    newStr = malloc(i + j + 1);

    for (i = j = 0; i < strlen(str); i++) {
        if(str[i] == '\n' || str[i] == '\t'){
                newStr[i+j] = '\\';
                newStr[i+j+1] = str[i] == '\n'? 'n':'t'; 
                j++; 
        }else{
            newStr[i+j] = str[i];
        }
    }
    newStr[i+j] = '\0';
    return newStr;
}

int main(int argc, char const *argv[]){
    
    initFlex("input.py");
    
    bool analyze = true;
    
    while(analyze){
        //pido o novo compoñente léxico
        LexicalComponent *lexicalComponent = nextLexicalComponent();
        

        if(lexicalComponent->lexicalComp != INVALID_LEXICAL_COMPONENT){  //componente lexico correcto
            char *escapedLexeme = _escapeChars(lexicalComponent->lexeme);
            printf("<%d, %s>\n", lexicalComponent->lexicalComp, escapedLexeme);
            free(escapedLexeme);

            //paro cando atopo o compoñente léxico $
            analyze = lexicalComponent->lexicalComp != EOF_COMP;
        }

        freeLexicalComponent(lexicalComponent);
    }

    freeFlex();

    return 0;
}
