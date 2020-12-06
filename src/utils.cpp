#include "utils.h"

#include <iostream>

void printTokens(const std::vector<Token *> &tokens)
{
    for (Token *token : tokens)
    {
        if (token->type == TokenTypes::Operator)
        {
            switch (((OperatorToken *)token)->op_type)
            {
            case OperatorType::Plus:
            {
                std::cout << "+ ";
                break;
            }
            case OperatorType::Minus:
            {
                std::cout << "- ";
                break;
            }
            case OperatorType::Multiplication:
            {
                std::cout << "* ";
                break;
            }
            case OperatorType::Division:
            {
                std::cout << "/ ";
                break;
            }
            }
        }
        else if (token->type == TokenTypes::Number)
        {
            std::cout << ((NumberToken *)token)->value << ' ';
        }
        else if (token->type == TokenTypes::Id)
        {
            std::cout << ((IdToken*)token)->name << ' ';
        }
        else if (token->type == TokenTypes::Bracket)
        {
            if(((BracketToken*)token)->bracket_type == BracketType::LeftBracket)
                std::cout << "( ";
            else if(((BracketToken*)token)->bracket_type == BracketType::RightBraket)
                std::cout << ") ";
        }
    }
    std::cout << std::endl;
}
