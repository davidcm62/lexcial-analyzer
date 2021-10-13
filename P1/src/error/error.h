#ifndef ERROR_H
#define ERROR_H

#include "../inputSystem/sisEntrada.h"

#define FOREACH_ERROR(ERROR) \
        ERROR(COMMAND_LINE_PARAMS) \
        ERROR(IO) \
        ERROR(LEXICAL) \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define ANSI_COLOR_RED     "\x1b[91m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum errors {
    FOREACH_ERROR(GENERATE_ENUM)
} ErrorType;

void handleError(ErrorType errorType);
void handleErrorWithFileStats(ErrorType errorType, char *msg, SE_Stats seStats, char *filename);

#endif /* ERROR_H */