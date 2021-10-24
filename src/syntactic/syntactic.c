#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../lexical/lex.yy.h"
#include "../common/definitions.h"
#include "../common/colors.h"
#include "../error/error.h"

/**
 * Reemplaza o salto de linea por \\n e o tab por \\t nun string
 */
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

#define FORMATTED_PRINT ANSI_COLOR_CYAN"<"ANSI_COLOR_MAGENTA"%d"ANSI_COLOR_CYAN", "ANSI_COLOR_RESET"%s"ANSI_COLOR_CYAN">"ANSI_COLOR_RESET"\n"

void startSyntacticAnalysis(){
    bool analyze = true;
    
    while(analyze){
        //pido o novo compoñente léxico
        LexicalComponent *lexicalComponent = nextLexicalComponent();
        
        if(lexicalComponent->lexicalComp != INVALID_LEXICAL_COMPONENT){  //componente lexico correcto
            char *escapedLexeme = _escapeChars(lexicalComponent->lexeme);
            printf(FORMATTED_PRINT, lexicalComponent->lexicalComp, escapedLexeme);
            free(escapedLexeme);

            //paro cando atopo o compoñente léxico $
            analyze = lexicalComponent->lexicalComp != EOF_COMP;
        }

        freeLexicalComponent(lexicalComponent);
    }
}