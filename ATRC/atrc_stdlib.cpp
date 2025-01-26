#define _CRT_SECURE_NO_DEPRECATE
#include "./include/ATRC.h"
#include "./include/filer.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

uint64_t atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;

std::string atrc::str_to_lower(const char *str){
    std::string res = "";
    for(size_t i = 0; i < strlen(str); i++){
        res += std::tolower(str[i]);
    }
    return res;
}
void atrc::str_to_lower_s(std::string &str){
    for(size_t i = 0; i < str.size(); i++){
        str[i] = std::tolower(str[i]);
    }
}

std::vector<std::string> atrc::split(const std::string &str, char separator){
    std::vector<std::string> res;
    std::string buf = "";
    for(const char &c : str){
        if(c == separator){
            res.push_back(buf);
            buf = "";
            continue;
        }
        buf += c;
    }
    if(buf.size() > 0){
        res.push_back(buf);
    }
    return res;
}

std::string atrc::str_to_upper(const char *str){
    std::string res = "";
    for(size_t i = 0; i < strlen(str); i++){
        res += std::toupper(str[i]);
    }
    return res;
}

void atrc::str_to_upper_s(std::string &str){
    for(size_t i = 0; i < str.size(); i++){
        str[i] = std::toupper(str[i]);
    }
}

std::vector<std::string> atrc_std::atrc_to_vector(char separator, const std::string &value){
    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    std::vector<std::string> res;
    if(value.size() == 0){
        return res;
    }
    std::string temp_value = value;
    if(temp_value.at(temp_value.size()-1) != separator){
        temp_value += separator;
    }
    std::string buf = "";
    for(const char &c : temp_value){
        if(c == separator){
            res.push_back(buf);
            buf = "";
            continue;
        }
        buf += c;
    }
    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    return res;
}

namespace atrc_std {

C_PString_Arr atrc_to_list(const char separator, const char* value) {
    if (value == NULL) return NULL;

    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;

    C_PString_Arr res = (C_PString_Arr)malloc(sizeof(C_String_Arr));
    if (res == NULL) return NULL;

    std::vector<std::string> temp = atrc_std::atrc_to_vector(separator, value);
    if (temp.empty()) {
        free(res);
        return NULL;
    }

    res->list = (char**)malloc(temp.size() * sizeof(char*));
    if (res->list == NULL) {
        free(res);
        return NULL;
    }

    res->count = temp.size();
    for (size_t i = 0; i < temp.size(); i++) {
        res->list[i] = (char*)malloc(temp[i].size() + 1);
        if (res->list[i] == NULL) {
            // Cleanup previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(res->list[j]);
            }
            free(res->list);
            free(res);
            return NULL;
        }
        std::strcpy(res->list[i], temp[i].c_str());
    }

    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    return res;
}


void atrc_free_list(C_PString_Arr list) {
    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    if (list == NULL){ 
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        free(list->list[i]);
    }
    free(list->list);
    list->count = 0; // Reset count
    if(list != NULL) free(list);
    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
}

} // namespace atrc_std


bool atrc_std::atrc_to_bool(const char* value){
    std::string temp=atrc::str_to_lower(value);
    atrc::trim(temp);
    if(temp == "true" || temp == "1") {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
        return true;
    } else if(temp == "false" || temp == "0"){
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
        return false;
    } else {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        return false;
    }
}

uint64_t atrc_std::atrc_to_uint64_t(const char* value){
    uint64_t res = 0;
    try {
        res = std::stoull(value);
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        std::cerr << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

int64_t atrc_std::atrc_to_int64_t(const char* value){
    int64_t res = 0;
    try {
        res = std::stoll(value);
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        std::cerr << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

double atrc_std::atrc_to_double(const char* value){
    double res = 0;
    try {
        res = std::stod(value);
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        std::cerr << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

