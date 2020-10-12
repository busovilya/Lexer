#pragma once

#include <string>
#include <finite_state_machine.h>

enum TokenTypes { Number, Bracket, Operator, Id, Function };
enum BracketType { LeftBracket, RightBraket };
enum OperatorType { Plus, Minus, Multiplication, Division, Exponent };
enum FunctionType { Sin, Cos };

struct Token
{
    TokenTypes type;
};

struct NumberToken : Token
{
    double value;
};

struct BracketToken : Token
{
    BracketType bracket_type;
};

struct OperatorToken : Token
{
    OperatorType op_type;
    int operator_priority;
};

struct FunctionToken : Token
{
    FunctionType func_type;
};


