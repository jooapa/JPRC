#include "../include/ATRC.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void InsertVar(char *line, const char *args[]) {
    int len = strlen(line);
    bool _looking_for_var = false;
    char var[INSERTVAR_MAX] = "";
    char _result[INSERTVAR_MAX] = "";
    int _arg_counter = 0;
    for (int i = 0; i < len; i++) {
        if (line[i] == '\0') break;
        if(line[i] == '%' || _looking_for_var){
#ifdef _WIN32
            strncat_s(var, INSERTVAR_MAX, &line[i], 1);
#else
            strncat(var, &line[i], INSERTVAR_MAX - strlen(var) - 1);
#endif
            if (line[i] == '%') {
                if(_looking_for_var){
                    printf("Looking for var: '%s'\n", var);

                    if(strcmp(var, "%*%") == 0){
#ifdef _WIN32
        strncat_s(_result, INSERTVAR_MAX, args[_arg_counter], _TRUNCATE);
#else
        strncat(_result, args[_arg_counter], INSERTVAR_MAX - strlen(_result) - 1);
#endif
                    _arg_counter++;
                    } else {
#ifdef _WIN32
        strncat_s(_result, INSERTVAR_MAX, var, _TRUNCATE);
#else
        strncat(_result, var, INSERTVAR_MAX - strlen(_result) - 1);
#endif
                    }
                    var[0] = '\0';
                    _looking_for_var = false;
                } else {
                    _looking_for_var = true;
                }
            }
            continue;
        }        


#ifdef _WIN32
        strncat_s(_result, INSERTVAR_MAX, &line[i], 1);
#else
        strncat(_result, &line[i], INSERTVAR_MAX - strlen(_result) - 1);
#endif
    }
    

#ifdef _WIN32
    strcpy_s(line, INSERTVAR_MAX, _result);
#else
    strcpy(line, _result);
#endif
}
