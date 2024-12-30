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
    C_String_Arr atrc_to_list(char separator, const char* value) {
        // TODO
		C_String_Arr res;
        return res;
    }
} // namespace atrc


void atrc::atrc_free_list(C_String_Arr *list){
    for(size_t i = 0; i < list->count; i++){
        delete[] list->list[i];
    }
    delete[] list->list;
    list->count = 0;
    atrc::atrc_stdlib_errval = atrc::_ATRC_SUCCESSFULL_ACTION;
}


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

