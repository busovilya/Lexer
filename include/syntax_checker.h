#include <vector>
#include <string>
#include "token.h"

std::vector<std::string> checkSyntax(std::vector<Token *> tokens);
bool checkParenthesesBalance(std::vector<Token *> tokens);