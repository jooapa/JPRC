#include "../include/ATRC.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void InsertVar(wchar_t *line, const wchar_t *args[]) {
    int len = wcslen(line);
    bool _looking_for_var = false;
    wchar_t var[INSERTVAR_MAX] = L"";
    wchar_t _result[INSERTVAR_MAX] = L"";
    int _arg_counter = 0;
    for (int i = 0; i < len; i++) {
        if (line[i] == L'\0') break;
        if(line[i] == L'%' || _looking_for_var){
#ifdef _WIN32
            wcsncat_s(var, INSERTVAR_MAX, &line[i], 1);
#else
            wcscat(var, &line[i], INSERTVAR_MAX - wcslen(var) - 1);
#endif
            if (line[i] == L'%') {
                if(_looking_for_var){
                    wprintf(L"Looking for var: '%s'\n", var);

                    if(wcscmp(var, L"%*%") == 0){
#ifdef _WIN32
        wcsncat_s(_result, INSERTVAR_MAX, args[_arg_counter], _TRUNCATE);
#else
        wcscat(_result, args[_arg_counter], INSERTVAR_MAX - wcslen(_result) - 1);
#endif
                    _arg_counter++;
                    } else {
#ifdef _WIN32
        wcsncat_s(_result, INSERTVAR_MAX, var, _TRUNCATE);
#else
        wcscat(_result, var, INSERTVAR_MAX - wcslen(_result) - 1);
#endif
                    }
                    var[0] = L'\0';
                    _looking_for_var = false;
                } else {
                    _looking_for_var = true;
                }
            }
            continue;
        }        


#ifdef _WIN32
        wcsncat_s(_result, INSERTVAR_MAX, &line[i], 1);
#else
        wcscat(_result, &line[i], INSERTVAR_MAX - wcslen(_result) - 1);
#endif
    }
    
#ifdef _WIN32
    wcscpy_s(line, INSERTVAR_MAX, _result);
#else
    wcscpy(line, INSERTVAR_MAX, _result);
#endif
}
