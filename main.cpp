#include "lexer.h"
#include "finite_state_machine.h"

#include <vector>

int main()
{
    Lexer lexer("(5.2+a_12)\0");
    std::vector<Token*> tokens;
    Token *t;

    while((t = lexer.getNextToken()) != nullptr)
    {
        tokens.push_back(t);
    }
    return 0;
}