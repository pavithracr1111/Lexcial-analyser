#include <stdio.h>  
#include <ctype.h>
#include <string.h> 
#include "types.h"  

// List of C language keywords
const char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do",
                          "double", "else", "enum", "extern", "float", "for", "goto", "if",
                          "int", "long", "register", "return", "short", "signed", "sizeof", "static",
                          "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", NULL};

// Function to check if a given string is a C keyword
int is_keyword(const char *str)
{
    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(str, keywords[i]) == 0) // Compare input string with keywords
        {
            return 1; // Return 1 if it's a keyword
        }
    }
    return 0; // Return 0 if it's not a keyword
}

// Function to check if a character is a special character in C
int is_special_char(char ch)
{
    // Check if the character is one of the predefined special characters
    return strchr("(){}[];,\\\".", ch) != NULL;
}

// Function to get the next token from the source file
Token get_next_token(FILE *file)
{
    Token token;
    token.type = UNKNOWN; // Initialize token type as UNKNOWN
    char ch;

    // Read file character by character
    while ((ch = fgetc(file)) != EOF)
    {
        if (isspace(ch))
        {
            continue; // Skip whitespaces
        }
        else if (isalpha(ch))
        {
            // Handle identifier or keyword
            int i = 0;
            token.lexeme[i++] = ch;                        // Start storing lexeme
            while (isalnum(ch = fgetc(file)) || ch == '_') // Allow alphanumeric and underscore
            {
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0'; // Null-terminate the string
            ungetc(ch, file);       // Push the last non-alphanumeric character back to the stream
            if (is_keyword(token.lexeme))
            {
                token.type = KEYWORD; // Mark as KEYWORD if it matches any keyword
            }
            else
            {
                token.type = IDENTIFIER; // Otherwise, mark as IDENTIFIER
            }
            return token; // Return the token
        }
        else if (isdigit(ch))
        {
            // Handle numeric constants
            int i = 0;
            token.lexeme[i++] = ch;
            while (isdigit(ch = fgetc(file))) // Continue reading digits
            {
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0'; // Null-terminate the string
            ungetc(ch, file);       // Push the non-digit back to the stream
            token.type = CONSTANT;  // Mark as CONSTANT
            return token;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == ';')
        {
            // Handle operators
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0'; // Store operator as a single character string
            token.type = OPERATOR;  // Mark as OPERATOR
            return token;
        }
        else if (is_special_char(ch))
        {
            // Handle special characters
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';    // Store the special character
            token.type = SPECIAL_CHAR; // Mark as SPECIAL_CHAR
            return token;
        }
        else if (ch == '#')
        {
            // Handle preprocessor directives
            int i = 0;
            token.lexeme[i++] = ch;                         // Start storing lexeme
            while ((ch = fgetc(file)) != EOF && ch != '\n') // Continue until newline
            {
                token.lexeme[i++] = ch;
            }
            token.lexeme[i] = '\0';    // Null-terminate the string
            token.type = PREPROCESSOR; // Mark as PREPROCESSOR
            return token;
        }
        else
        {
            // Handle unrecognized tokens
            printf("Error: Unrecognized token: %c\n", ch); // Print error message
            token.type = UNKNOWN;                          // Mark as UNKNOWN
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0'; // Store unrecognized character
            return token;
        }
    }
    // End of file
    token.type = EOF_TOKEN; // Mark as EOF
    token.lexeme[0] = '\0'; // Empty lexeme
    return token;
}

// Function to print a token based on its type
void print_token(Token token)
{
    switch (token.type)
    {
    case KEYWORD:
        printf("KEYWORD: %s\n", token.lexeme); // Print if it's a keyword
        break;
    case IDENTIFIER:
        printf("IDENTIFIER: %s\n", token.lexeme); // Print if it's an identifier
        break;
    case CONSTANT:
        printf("CONSTANT: %s\n", token.lexeme); // Print if it's a constant
        break;
    case OPERATOR:
        printf("OPERATOR: %s\n", token.lexeme); // Print if it's an operator
        break;
    case SPECIAL_CHAR:
        printf("SPECIAL_CHAR: %s\n", token.lexeme); // Print if it's a special character
        break;
    case PREPROCESSOR:
        printf("PREPROCESSOR: %s\n", token.lexeme); // Print if it's a preprocessor directive
        break;
    case EOF_TOKEN:
        printf("EOF\n"); // Print end of file token
        break;
    case UNKNOWN:
        printf("UNKNOWN: %s\n", token.lexeme); // Print unknown tokens
        break;
    }
}

// Function to check if the file passed is a valid C file
Status check_Operation(char *filename, lexical_analyser *LA)
{
    // Check if the file has a .c extension
    if (filename != NULL && strcmp(strrchr(filename, '.'), ".c") == 0)
    {
        LA->Filename = filename; // Store filename in the lexical analyzer struct
        return SUCCESS;          // Return success if it's a .c file
    }
    return FAILURE; // Return failure if not a valid file
}

// Function to open the file for reading
Status open_file(char *argv, lexical_analyser *LA)
{
    LA->fptr_fname = fopen(LA->Filename, "r"); // Open the file in read mode

    if (LA->fptr_fname == NULL)
    {
        // If file opening fails, print error message
        fprintf(stderr, "Failed to open .c file\n");
        return FAILURE; // Return failure if file can't be opened
    }
    return SUCCESS; // Return success if file opened successfully
}

// Function to read and print all tokens from the file
Status print_tokens(lexical_analyser *LA)
{
    Token token;
    while (1)
    {
        token = get_next_token(LA->fptr_fname); // Get the next token
        if (token.type == EOF_TOKEN)            // Break the loop when EOF is reached
        {
            break;
        }
        print_token(token); // Print each token
    }
    fclose(LA->fptr_fname); // Close the file after reading all tokens
    return SUCCESS;         // Return success after printing all tokens
}
