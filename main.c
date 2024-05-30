#include <stdio.h>
#include <string.h>
#include "types.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    lexical_analyser LA;
    if (check_Operation(argv[1], &LA) == SUCCESS) {
        printf("------------------------------------------- Lexical Analyzer -------------------------------------------\n");

        if (open_file(argv[1], &LA) == SUCCESS) {
            if (print_tokens(&LA) == SUCCESS) {
                printf("--------------------------------------------------------------------------------------------------------\n");
                return SUCCESS;
            } else {
                printf("Failed to print tokens\n");
            }
        }
    } else {
        printf("Please pass a valid .c file\n");
    }
    return 0;
}
