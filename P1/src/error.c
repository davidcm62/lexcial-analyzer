#include <stdio.h>
#include "error.h"


// void printRed(const char *msg){
//     printf(ANSI_COLOR_RED"%s"ANSI_COLOR_RESET"\n",msg);
// }

// void printBlue(const char *msg){
//     printf(ANSI_COLOR_CYAN"%s"ANSI_COLOR_RESET"\n",msg);
// }
void handleError(ErrorType errorType){
    printf(ANSI_COLOR_RED"error"ANSI_COLOR_RESET" (%s)"ANSI_COLOR_RED": "ANSI_COLOR_RESET, error_strings[errorType]);
    
    switch (errorType){
        case COMMAND_LINE_PARAMS:
            printf("uso: compiler.exe filename");
            break;
        case FILE_ERROR:
            printf("file not found");
            break;
        case LEXICAL_ERROR:
            printf("lexical error found in file");
            break;
        default:
            printf("a error has ocurred");
            break;
    }
    printf("\n");
}