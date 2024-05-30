#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "types.h"

const char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do",
                          "double", "else", "enum", "extern", "float", "for", "goto", "if",
                          "int", "long", "register", "return", "short", "signed", "sizeof", "static",
                          "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", NULL};

int is_keyword(const char *str)
{
    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_special_char(char ch)
{
    return strchr("(){}[];,\\\".", ch) != NULL;
}

Token get_next_token(FILE *file)
{
    Token token;
    token.type = UNKNOWN;
    int ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if (isspace(ch))
        {
            continue;
        }
        else if (isalpha(ch))
        {
            int i = 0;
            token.lexeme[i++] = ch;
            while (isalnum(ch = fgetc(file)) || ch == '_')
            {
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0';
            ungetc(ch, file);
            if (is_keyword(token.lexeme))
            {
                token.type = KEYWORD;
            }
            else
            {
                token.type = IDENTIFIER;
            }
            return token;
        }
        else if (isdigit(ch))
        {
            int i = 0;
            token.lexeme[i++] = ch;
            while (isdigit(ch = fgetc(file)))
            {
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0';
            ungetc(ch, file);
            token.type = CONSTANT;
            return token;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == ';')
        {
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            token.type = OPERATOR;
            return token;
        }
        else if (is_special_char(ch))
        {
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            token.type = SPECIAL_CHAR;
            return token;
        }
        else if (ch == '#')
        {
            int i = 0;
            token.lexeme[i++] = ch;
            while ((ch = fgetc(file)) != EOF && ch != '\n')
            {
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0';
            token.type = PREPROCESSOR;
            return token;
        }
        else
        {
            printf("Error: Unrecognized token: %c\n", ch);
            token.type = UNKNOWN;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            return token;
        }
    }
    token.type = EOF_TOKEN;
    token.lexeme[0] = '\0';
    return token;
}

void print_token(Token token)
{
    switch (token.type)
    {
    case KEYWORD:
        printf("KEYWORD: %s\n", token.lexeme);
        break;
    case IDENTIFIER:
        printf("IDENTIFIER: %s\n", token.lexeme);
        break;
    case CONSTANT:
        printf("CONSTANT: %s\n", token.lexeme);
        break;
    case OPERATOR:
        printf("OPERATOR: %s\n", token.lexeme);
        break;
    case SPECIAL_CHAR:
        printf("SPECIAL_CHAR: %s\n", token.lexeme);
        break;
    case PREPROCESSOR:
        printf("PREPROCESSOR: %s\n", token.lexeme);
        break;
    case EOF_TOKEN:
        printf("EOF\n");
        break;
    case UNKNOWN:
        printf("UNKNOWN: %s\n", token.lexeme);
        break;
    }
}

Status check_Operation(char *filename, lexical_analyser *LA)
{
    if (filename != NULL && strcmp(strrchr(filename, '.'), ".c") == 0)
    {
        LA->Filename = filename;
        return SUCCESS;
    }
    return FAILURE;
}

Status open_file(char *argv, lexical_analyser *LA)
{
    LA->fptr_fname = fopen(LA->Filename, "r");
    if (LA->fptr_fname == NULL)
    {
        fprintf(stderr, "Failed to open .c file\n");
        return FAILURE;
    }
    return SUCCESS;
}

Status print_tokens(lexical_analyser *LA)
{
    Token token;
    while (1)
    {
        token = get_next_token(LA->fptr_fname);
        if (token.type == EOF_TOKEN)
        {
            break;
        }
        print_token(token);
    }
    fclose(LA->fptr_fname);
    return SUCCESS;
}
