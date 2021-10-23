#ifndef ERROR_H
#define ERROR_H

/**
 * Valores do enum ErrorType
 */
#define FOREACH_ERROR(ERROR) \
        ERROR(COMMAND_LINE_PARAMS) \
        ERROR(IO) \
        ERROR(LEXICAL) \

//defines que permiten auto xerar o enum a partir do define anterior
#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define ANSI_COLOR_RED     "\x1b[91m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum errors {
    FOREACH_ERROR(GENERATE_ENUM)
} ErrorType;

/**
 * Imprime unha mensaxe de erro en función do código errorType
 */
void handleError(ErrorType errorType);

/**
 * Imprime unha mensaxe de erro en función do código, co contido de msg e indicando a línea do ficheiro onde se produciu
 */
void handleErrorWithFileStats(ErrorType errorType, char *msg, char *filename, unsigned long line);

#endif /* ERROR_H */