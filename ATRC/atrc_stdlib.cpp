#define INCLUDE_ATRC_STDLIB
#include <ATRC.hpp>
#include <filer.h>

int atrc_stdlib_errval = SUCCESSFULL_ACTION;

std::vector<std::string> atrc_to_list(char separator, const std::string &value){
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
    atrc_stdlib_errval = SUCCESSFULL_ACTION;
    return res;
}

bool atrc_to_bool(const std::string &value){
    std::string temp=str_to_lower(value);
    trim(temp);
    if(temp == "true" || temp == "1") {
        atrc_stdlib_errval = SUCCESSFULL_ACTION;
        return true;
    } else if(temp == "false" || temp == "0"){
        atrc_stdlib_errval = SUCCESSFULL_ACTION;
        return false;
    } else {
        atrc_stdlib_errval = UNSUCCESFULL_ACTION;
        return false;
    }
}

uint64_t atrc_to_uint64_t(const std::string &value){
    uint64_t res = 0;
    try {
        res = std::stoull(value);
        atrc_stdlib_errval = SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc_stdlib_errval = UNSUCCESFULL_ACTION;
    } catch (const std::out_of_range& e) {
        atrc_stdlib_errval = UNSUCCESFULL_ACTION;
    }
    return res;
}

int64_t atrc_to_int64_t(const std::string &value){
    int64_t res = 0;
    try {
        res = std::stoll(value);
        atrc_stdlib_errval = SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc_stdlib_errval = UNSUCCESFULL_ACTION;
    } catch (const std::out_of_range& e) {
        atrc_stdlib_errval = UNSUCCESFULL_ACTION;
    }
    return res;
}

double atrc_to_double(const std::string &value){
    double res = 0;
    try {
        res = std::stod(value);
        atrc_stdlib_errval = SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument& e) {
        atrc_stdlib_errval = UNSUCCESFULL_ACTION;
    } catch (const std::out_of_range& e) {
        atrc_stdlib_errval = UNSUCCESFULL_ACTION;
    }
    return res;
}
