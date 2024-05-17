#include "./include/ATRC.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#define INSERT_VAR_MAX_LINE_LENGTH 256

char* InsertVar(char *line, ...) {
    va_list args;
    va_start(args, line);
    char result[INSERT_VAR_MAX_LINE_LENGTH] = "";
    
    // Loop through the arguments
    while (*line != '\0') {
        // Concatenate the current argument to result
        strncat_s(result, INSERT_VAR_MAX_LINE_LENGTH, line, strlen(line));

        // Move to the next argument
        line = va_arg(args, char*);
    }

    va_end(args);

    // Copy the concatenated string to line
    strcpy_s(line, INSERT_VAR_MAX_LINE_LENGTH, result);

    // Return the concatenated string
    return line;
}
