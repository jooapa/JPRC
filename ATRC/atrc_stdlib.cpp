#define _CRT_SECURE_NO_DEPRECATE
#include "./include/ATRC.h"
#include "./include/filer.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <cctype>
#include <stack>
#include <cmath>

uint64_t atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
bool atrc_std::atrc_stdlib_writecheck = false;

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


void *atrc_free_list(C_PString_Arr list) {
    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    if (list == NULL){ 
        return NULL;
    }

    for (size_t i = 0; i < list->count; i++) {
        free(list->list[i]);
    }
    free(list->list);
    list->count = 0; // Reset count
    if(list != NULL) free(list);
    atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    return NULL;
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
    } catch (const std::invalid_argument&) {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        if(atrc_std::atrc_stdlib_writecheck) atrc::errormsg(ERR_STDLIB_CAST_ERROR, -1, value, "atrc_stdlib");
    } catch (const std::out_of_range&) {
        if(atrc_std::atrc_stdlib_writecheck) atrc::errormsg(ERR_STDLIB_CAST_ERROR, -1, value, "atrc_stdlib");
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

int64_t atrc_std::atrc_to_int64_t(const char* value){
    int64_t res = 0;
    try {
        res = std::stoll(value);
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument&) {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        if(atrc_std::atrc_stdlib_writecheck) atrc::errormsg(ERR_STDLIB_CAST_ERROR, -1, value, "atrc_stdlib");
    } catch (const std::out_of_range&) {
        if(atrc_std::atrc_stdlib_writecheck) atrc::errormsg(ERR_STDLIB_CAST_ERROR, -1, value, "atrc_stdlib");
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}

double atrc_std::atrc_to_double(const char* value){
    double res = 0;
    try {
        res = std::stod(value);
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
    } catch (const std::invalid_argument&) {
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        if(atrc_std::atrc_stdlib_writecheck) atrc::errormsg(ERR_STDLIB_CAST_ERROR, -1, value, "atrc_stdlib");
    } catch (const std::out_of_range&) {
        if(atrc_std::atrc_stdlib_writecheck) atrc::errormsg(ERR_STDLIB_CAST_ERROR, -1, value, "atrc_stdlib");
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
    }
    return res;
}



static const std::unordered_map<std::string, atrc::MATH_OPERATOR_TYPE> operatorMap = {
    {"+", atrc::MATH_OPERATOR_TYPE::ADD}, {"-", atrc::MATH_OPERATOR_TYPE::SUBTRACT},
    {"*", atrc::MATH_OPERATOR_TYPE::MULTIPLY}, {"/", atrc::MATH_OPERATOR_TYPE::DIVIDE},
    {"%", atrc::MATH_OPERATOR_TYPE::MODULO},   {"^", atrc::MATH_OPERATOR_TYPE::POWER},
    {"(", atrc::MATH_OPERATOR_TYPE::OPEN_PAREN}, {")", atrc::MATH_OPERATOR_TYPE::CLOSE_PAREN},
    {"SQRT", atrc::MATH_OPERATOR_TYPE::SQRT}, {"ABS", atrc::MATH_OPERATOR_TYPE::ABS},
    {"LOG", atrc::MATH_OPERATOR_TYPE::LOG},   {"LOG10", atrc::MATH_OPERATOR_TYPE::LOG10},
    {"SIN", atrc::MATH_OPERATOR_TYPE::SIN},   {"COS", atrc::MATH_OPERATOR_TYPE::COS},
    {"TAN", atrc::MATH_OPERATOR_TYPE::TAN},   {"ASIN", atrc::MATH_OPERATOR_TYPE::ASIN},
    {"ACOS", atrc::MATH_OPERATOR_TYPE::ACOS}, {"ATAN", atrc::MATH_OPERATOR_TYPE::ATAN},
};

double atrc::MathParser::evaluate(const std::string& expression) {
    auto tokens = tokenize(expression);
    auto rpn = toRPN(tokens);
    return evaluateRPN(rpn);
}

// Tokenizer
std::vector<atrc::MathToken> atrc::MathParser::tokenize(const std::string& input) {
    std::vector<atrc::MathToken> tokens;
    size_t i = 0;
    while (i < input.size()) {
        if (std::isspace(input[i])) {
            i++;
        } else if (std::isdigit(input[i]) || input[i] == '.') {
            size_t len = 0;
            double val = std::stod(&input[i], &len);
            tokens.push_back({MATH_TOKEN_TYPE::NUMBER, val, MATH_NUMBER_TYPE::FLOAT});
            i += len;
        } else if (input[i] == '0' && (i+1 < input.size()) && (input[i+1] == 'x' || input[i+1] == 'X')) {
            i += 2;
            size_t len = 0;
            int val = std::stoi(&input[i], &len, 16);
            tokens.push_back({MATH_TOKEN_TYPE::NUMBER, static_cast<double>(val), MATH_NUMBER_TYPE::HEXADECIMAL});
            i += len;
        } else if (input[i] == '0' && (i+1 < input.size()) && (input[i+1] == 'b' || input[i+1] == 'B')) {
            i += 2;
            size_t len = 0;
            int val = std::stoi(&input[i], &len, 2);
            tokens.push_back({MATH_TOKEN_TYPE::NUMBER, static_cast<double>(val), MATH_NUMBER_TYPE::BINARY});
            i += len;
        } else {
            bool matched = false;
            for (const auto& [str, op] : operatorMap) {
                if (input.compare(i, str.length(), str) == 0) {
                    tokens.push_back({MATH_TOKEN_TYPE::OPERATOR, op});
                    i += str.length();
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                if (input.compare(i, 2, "PI") == 0) {
                    tokens.push_back({MATH_TOKEN_TYPE::NUMBER, ATRC_PI, MATH_NUMBER_TYPE::PI});
                    i += 2;
                } else if (input.compare(i, 1, "E") == 0) {
                    tokens.push_back({MATH_TOKEN_TYPE::NUMBER, ATRC_E, MATH_NUMBER_TYPE::E});
                    i += 1;
                } else if (input.compare(i, 2, "GR") == 0) {
                    tokens.push_back({MATH_TOKEN_TYPE::NUMBER, 1.61803398875, MATH_NUMBER_TYPE::GOLDEN_RATIO});
                    i += 2;
                } else {
                    throw std::runtime_error("Unknown token at: " + input.substr(i));
                }
            }
        }
    }
    return tokens;
}

// Simple operator precedence
int precedence(atrc::MATH_OPERATOR_TYPE op) {
    switch (op) {
        case atrc::MATH_OPERATOR_TYPE::ADD:
        case atrc::MATH_OPERATOR_TYPE::SUBTRACT: return 1;
        case atrc::MATH_OPERATOR_TYPE::MULTIPLY:
        case atrc::MATH_OPERATOR_TYPE::DIVIDE:
        case atrc::MATH_OPERATOR_TYPE::MODULO: return 2;
        case atrc::MATH_OPERATOR_TYPE::POWER: return 3;
        default: return 0;
    }
}

// Shunting Yard
std::vector<atrc::MathToken> atrc::MathParser::toRPN(const std::vector<atrc::MathToken>& tokens) {
    std::vector<atrc::MathToken> output;
    std::stack<atrc::MathToken> operators;

    for (const auto& token : tokens) {
        if (token.type == MATH_TOKEN_TYPE::NUMBER) {
            output.push_back(token);
        } else {
            atrc::MATH_OPERATOR_TYPE op = std::get<atrc::MATH_OPERATOR_TYPE>(token.value);
            if (op == atrc::MATH_OPERATOR_TYPE::OPEN_PAREN) {
                operators.push(token);
            } else if (op == atrc::MATH_OPERATOR_TYPE::CLOSE_PAREN) {
                while (!operators.empty() && std::get<atrc::MATH_OPERATOR_TYPE>(operators.top().value) != atrc::MATH_OPERATOR_TYPE::OPEN_PAREN) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // Remove '('
            } else {
                while (!operators.empty()) {
                    auto top = operators.top();
                    if (top.type == MATH_TOKEN_TYPE::OPERATOR &&
                        precedence(std::get<atrc::MATH_OPERATOR_TYPE>(top.value)) >= precedence(op)) {
                        output.push_back(top);
                        operators.pop();
                    } else {
                        break;
                    }
                }
                operators.push(token);
            }
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

// RPN Evaluator
double atrc::MathParser::evaluateRPN(const std::vector<atrc::MathToken>& rpn) {
    std::stack<double> stack;

    for (const auto& token : rpn) {
        if (token.type == MATH_TOKEN_TYPE::NUMBER) {
            stack.push(std::get<double>(token.value));
        } else {
            atrc::MATH_OPERATOR_TYPE op = std::get<atrc::MATH_OPERATOR_TYPE>(token.value);

            if (stack.size() < (op == atrc::MATH_OPERATOR_TYPE::SQRT || op == atrc::MATH_OPERATOR_TYPE::LOG ? 1 : 2))
                throw std::runtime_error("Insufficient operands for operator");

            double b = stack.top(); stack.pop();
            double a = (stack.empty() ? 0 : stack.top());
            if (op != atrc::MATH_OPERATOR_TYPE::SQRT && op != atrc::MATH_OPERATOR_TYPE::LOG) stack.pop();

            switch (op) {
                case atrc::MATH_OPERATOR_TYPE::ADD: stack.push(a + b); break;
                case atrc::MATH_OPERATOR_TYPE::SUBTRACT: stack.push(a - b); break;
                case atrc::MATH_OPERATOR_TYPE::MULTIPLY: stack.push(a * b); break;
                case atrc::MATH_OPERATOR_TYPE::DIVIDE: stack.push(a / b); break;
                case atrc::MATH_OPERATOR_TYPE::MODULO: stack.push(fmod(a, b)); break;
                case atrc::MATH_OPERATOR_TYPE::POWER: stack.push(pow(a, b)); break;

                case atrc::MATH_OPERATOR_TYPE::SQRT: stack.push(sqrt(b)); break;
                case atrc::MATH_OPERATOR_TYPE::ABS: stack.push(fabs(b)); break;
                case atrc::MATH_OPERATOR_TYPE::LOG: stack.push(log(b)); break;
                case atrc::MATH_OPERATOR_TYPE::LOG10: stack.push(log10(b)); break;

                case atrc::MATH_OPERATOR_TYPE::SIN: stack.push(sin(b)); break;
                case atrc::MATH_OPERATOR_TYPE::COS: stack.push(cos(b)); break;
                case atrc::MATH_OPERATOR_TYPE::TAN: stack.push(tan(b)); break;

                default:
                    throw std::runtime_error("Operator not implemented");
            }
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Invalid RPN expression");

    return stack.top();
}



double atrc_std::atrc_math_exp(const char* value) {
    atrc::MathParser parser;
    try {
        double result = parser.evaluate(value);
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_SUCCESSFULL_ACTION;
        return result;
    } catch (const std::exception&) {
        if(atrc_std::atrc_stdlib_writecheck) atrc::errormsg(ERR_INVALID_EXPRESSION, -1, value, "atrc_stdlib");
        atrc_std::atrc_stdlib_errval = atrc_std::_ATRC_UNSUCCESSFULL_ACTION;
        return 0.0; // Return a default value on error
    }
}


