#include <string>
#include <finite_state_machine.h>

enum TokenTypes { NumberToken, FloatToken, BracketToken, AssignToken, OperationToken, IdToken, End };

struct Token
{
public:
    TokenTypes type;
};