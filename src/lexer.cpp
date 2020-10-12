#include "lexer.h"

#include <string>

Lexer::Lexer(std::string input_text) : buffer(input_text), lexemeBegin(buffer.begin()), forward(buffer.begin())
{
    number_recognizer = new NumberRecognizer();
    ops_recognizer = new OperationRecognizer();
    id_recognizer = new IdRecognizer();
    bracket_recognizer = new BracketsRecognizer();
    func_recognizer = new FunctionRecognizer();
}

const char &Lexer::getNextChar()
{
    return *forward++;
}

Token *Lexer::getNextToken()
{
    Token* token = nullptr;

    while(1)
    {
        const char& ch = getNextChar();

        if(*lexemeBegin == '\0')
            return nullptr;

        if(!number_recognizer->failed() && !number_recognizer->accepted()) 
            number_recognizer->putChar(ch);
        if(!ops_recognizer->failed() && !ops_recognizer->accepted())
            ops_recognizer->putChar(ch);
        if(!id_recognizer->failed() && !id_recognizer->accepted())
            id_recognizer->putChar(ch);
        if(!bracket_recognizer->failed() && !bracket_recognizer->accepted())
            bracket_recognizer->putChar(ch);
        if(!func_recognizer->failed() && !func_recognizer->accepted())
            func_recognizer->putChar(ch);

        if(number_recognizer->accepted())
        {
            delete token;
            NumberToken *t = new NumberToken();
            t->type = TokenTypes::Number;
            t->value = std::stod(std::string(lexemeBegin, forward - 1));
            token = t;
        }
        if(ops_recognizer->accepted())
        {
            delete token;
            OperatorType operator_type;
            int operator_priority;
            switch (*lexemeBegin)
            {
            case '+':
                operator_type = OperatorType::Plus;
                operator_priority = 1;
                break;
            case '-':
                operator_type = OperatorType::Minus;
                operator_priority = 1;
                break;
            case '*':
                operator_type = OperatorType::Multiplication;
                operator_priority = 2;
                break;
            case '/':
                operator_type = OperatorType::Division;
                operator_priority = 2;
                break;
            case '^':
                operator_type = OperatorType::Exponent;
                operator_priority = 3;
                break;
            };
            OperatorToken *t = new OperatorToken();
            t->type = TokenTypes::Operator;
            t->op_type = operator_type;
            token = t;
        }
        if(id_recognizer->accepted())
        {
            delete token;
            token = new Token { TokenTypes::Id };
        }
        if(func_recognizer->accepted())
        {
            delete token;
            FunctionToken *t = new FunctionToken();
            t->type = TokenTypes::Function;
            std::string token_value = std::string(lexemeBegin, forward - 1);
            if(token_value.compare("sin") == 0)
                t->func_type = FunctionType::Sin;
            else if(token_value.compare("cos") == 0)
                t->func_type = FunctionType::Cos;
            else
                throw new LexerError("Undefined function!");
            token = t;
        }
        if(bracket_recognizer->accepted())
        {
            delete token;
            BracketToken *t = new BracketToken();
            t->type = TokenTypes::Bracket;

            if(*lexemeBegin == '(')
                t->bracket_type = BracketType::LeftBracket;
            else if(*lexemeBegin == ')')
                t->bracket_type = BracketType::RightBraket;
            token = t;
        }

        bool activeRecognizerExists = false;
        if(!number_recognizer->failed() && !number_recognizer->accepted()) activeRecognizerExists = true;
        if(!ops_recognizer->failed() && !ops_recognizer->accepted()) activeRecognizerExists = true;
        if(!id_recognizer->failed() && !id_recognizer->accepted()) activeRecognizerExists = true;
        if(!func_recognizer->failed() && !func_recognizer->accepted()) activeRecognizerExists = true;
        if(!bracket_recognizer->failed() && !bracket_recognizer->accepted()) activeRecognizerExists = true;

        if(!activeRecognizerExists && token == nullptr)
            throw LexerError("Error at position " + std::to_string(forward - buffer.begin()));
        else if(!activeRecognizerExists && token != nullptr)
        {
            number_recognizer->reset();
            ops_recognizer->reset();
            id_recognizer->reset();
            func_recognizer->reset();
            bracket_recognizer->reset();

            forward--;
            lexemeBegin = forward;
            return token;
        }
    }
}