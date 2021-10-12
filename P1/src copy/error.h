#ifndef ERROR_H
#define ERROR_H

typedef enum automatonResult {
    SUCCESS = 1,
    LEXEME_TOO_LONG = -1,
    LEXICAL_ERROR = -2,
} AutomatonResult;

#endif /* ERROR_H */