#pragma once

#include "token_recognizers.h"
#include "token.h"

#include <string>
#include <exception>

class Lexer
{
private:
    std::string buffer;
    std::string::iterator lexemeBegin, forward;
    const char& getNextChar(); 
    TokenRecognizer *number_recognizer, 
                    *ops_recognizer,
                    *id_recognizer, 
                    *bracket_recognizer,
                    *func_recognizer;
public:
    Lexer(std::string input_text);
    Token* getNextToken();
};

struct LexerError : std::exception
{
private:
    std::string msg;
public:
    LexerError(std::string msg) : std::exception()
    {
        this->msg = msg;
    }

    const char* what() const throw()
    {
        return msg.c_str();
    }
};