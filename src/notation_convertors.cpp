#include "notation_convertors.h"
#include <stack>
#include <exception>

std::vector<Token *> from_infix_to_postfix(std::vector<Token *> tokens)
{
    std::stack<Token *> operator_stack;
    std::vector<Token *> output;

    for (Token *token : tokens)
    {
        if (token->type == TokenTypes::Number)
            output.push_back(token);
        else if (token->type == TokenTypes::Id)
            output.push_back(token);
        else if (token->type == TokenTypes::Function)
            operator_stack.push(token);
        else if (token->type == TokenTypes::Operator)
        {
            while (operator_stack.size() > 0 &&
                   (((OperatorToken *)operator_stack.top())->operator_priority >= ((OperatorToken *)token)->operator_priority &&
                    operator_stack.top()->type != TokenTypes::Bracket))
            {
                Token *token = operator_stack.top();
                output.push_back(token);
                operator_stack.pop();
            }
            operator_stack.push(token);
        }
        else if (token->type == TokenTypes::Bracket && ((BracketToken *)token)->bracket_type == BracketType::LeftBracket)
            operator_stack.push(token);
        else if (token->type == TokenTypes::Bracket && ((BracketToken *)token)->bracket_type == BracketType::RightBraket)
        {
            while (operator_stack.top()->type != TokenTypes::Bracket)
            {
                Token *token = operator_stack.top();
                output.push_back(token);
                operator_stack.pop();
                if (operator_stack.size() == 0)
                    throw std::exception();
            }

            if (operator_stack.top()->type == TokenTypes::Bracket)
                operator_stack.pop();
        }
    }

    while (!operator_stack.empty())
    {
        Token *token = operator_stack.top();
        if (token->type == TokenTypes::Bracket)
            throw std::exception();
        output.push_back(token);
        operator_stack.pop();
    }

    return output;
}