#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

typedef enum {
    SUCCESS,
    FAILURE
} Status;

typedef enum {
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    OPERATOR,
    SPECIAL_CHAR,
    PREPROCESSOR,
    EOF_TOKEN,
    UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[256]; // Adjust size as needed
} Token;

typedef struct {
    char *Filename;
    FILE *fptr_fname;
} lexical_analyser;

Status check_Operation(char *filename, lexical_analyser *LA);
Status open_file(char *argv, lexical_analyser *LA);
Status print_tokens(lexical_analyser *LA);

#endif // TYPES_H
