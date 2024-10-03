#ifndef _PLAINTEXT_PARSER_ERROR_H
#define _PLAINTEXT_PARSER_ERROR_H

#include <string>
#include <exception>

class plaintext_parser_error : public std::exception {
public:
    plaintext_parser_error(const std::string& message) {
        this-> what_msg = message;
    }
    
    const char* what() const override {
        return what_msg.c_str();
    }
private:
    std::string what_msg;
};

#endif