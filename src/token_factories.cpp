#include "token_factories.h"

BracketToken* createBracket(BracketType type)
{
    BracketToken* new_bracket = new BracketToken();
    new_bracket->type = TokenTypes::Bracket;
    new_bracket->bracket_type = type;
    return new_bracket;
}

OperatorToken* createOperator(OperatorType type)
{
    OperatorToken* op = new OperatorToken();
    op->type = TokenTypes::Operator;
    op->op_type = type;
    switch (type)
    {
    case OperatorType::Plus:
        op->operator_priority = 1;
        break;
    case OperatorType::Minus:
        op->operator_priority = 1;
        break;
    case OperatorType::Multiplication:
        op->operator_priority = 2;
        break;
    case OperatorType::Division:
        op->operator_priority = 2;
        break;
    }
    return op;
};