#include <string>
#include <finite_state_machine.h>

enum TokenTypes { Number, Bracket, Operation, Id };
enum BracketType { LeftBracket, RightBraket };
enum OperationType { Plus, Minus, Multiplication, Division, Exponent };

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

struct OperationToken : Token
{
    OperationType op_type;
};

