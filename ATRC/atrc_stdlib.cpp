#define _CRT_SECURE_NO_DEPRECATE
#include <filer.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <ATRC.h>
#include <sstream>

uint64_t atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;

std::string str_to_lower(const char *str){
    std::string res = "";
    for(size_t i = 0; i < strlen(str); i++){
        res += std::tolower(str[i]);
    }
    return res;
}
std::vector<std::string> atrc::atrc_to_vector(char separator, const std::string &value){
    atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
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
    atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
    return res;
}

namespace atrc {

C_PString_Arr atrc_to_list(const char separator, const char* value) {
    if (value == NULL) return NULL;

    atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;

    C_PString_Arr res = (C_PString_Arr)malloc(sizeof(C_String_Arr));
    if (res == NULL) return NULL;

    std::vector<std::string> temp = atrc::atrc_to_vector(separator, value);
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

    atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
    return res;
}


void atrc_free_list(C_PString_Arr list) {
    atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
    if (list == NULL){ 
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        free(list->list[i]);
    }
    free(list->list);
    list->count = 0; // Reset count
    if(list != NULL) free(list);
    atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
}

} // namespace atrc


bool atrc::atrc_to_bool(const char* value){
    std::string temp=str_to_lower(value);
    trim(temp);
    if(temp == "true" || temp == "1") {
        atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
        return true;
    } else if(temp == "false" || temp == "0"){
        atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
        return false;
    } else {
        atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
        return false;
    }
}

uint64_t atrc::atrc_to_uint64_t(const char* value){
    uint64_t res = 0;
    try {
        res = std::stoull(value);
        atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
        std::cerr << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

int64_t atrc::atrc_to_int64_t(const char* value){
    int64_t res = 0;
    try {
        res = std::stoll(value);
        atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
        std::cerr << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

double atrc::atrc_to_double(const char* value){
    double res = 0;
    try {
        res = std::stod(value);
        atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
        std::cerr << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        atrc::atrc_stdlib_errval = atrc::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

