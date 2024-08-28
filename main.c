/*
Name : Pavithra C R
Descritption : Lexical Analyser
*/

#include <stdio.h>  
#include <string.h>
#include "types.h" 

int main(int argc, char *argv[])
{
    // Check if the number of command-line arguments is correct
    if (argc != 2)
    {
        // If no file is passed or more than one argument is passed, print usage instructions
        printf("Usage: %s <filename>\n", argv[0]);
        return 1; 
    }

    lexical_analyser LA; // Variable of type lexical_analyser to hold information during analysis

    // Check if the operation on the file is valid (assumed to check if the file is a valid .c file)
    if (check_Operation(argv[1], &LA) == SUCCESS)
    {
        // If the operation is valid, print the heading for the lexical analysis output
        printf("------------------------------------------- Lexical Analyzer -------------------------------------------\n");

        // Open the file for analysis
        if (open_file(argv[1], &LA) == SUCCESS)
        {
            // If the file is successfully opened, attempt to print the tokens
            if (print_tokens(&LA) == SUCCESS)
            {
                // If tokens are printed successfully, print the footer and return SUCCESS
                printf("--------------------------------------------------------------------------------------------------------\n");
                return SUCCESS; // Return SUCCESS code (assumed to be 0)
            }
            else
            {
                // If token printing fails, print an error message
                printf("Failed to print tokens\n");
            }
        }
    }
    else
    {
        
        printf("Please pass a valid .c file\n");
    }

    return 0;
}
