#include "lexer.h"
#include "finite_state_machine.h"

int main()
{
    Lexer lexer("(5.2+a_12)\0");
    Token *token = lexer.getNextToken();
    token = lexer.getNextToken();
    token = lexer.getNextToken();
    return 0;
}