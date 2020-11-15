#include "lexer.h"
#include "parser.cpp"
#include "finite_state_machine.h"
#include "syntax_checker.h"
#include "notation_convertors.h"
#include "syntax_tree.h"

#include <iostream>
#include <vector>
#include <string>

void printTokens(const std::vector<Token*>& tokens)
{
     for(Token* token: tokens)
    {
        if(token->type == TokenTypes::Operator)
        {
            if(((OperatorToken*)token)->op_type == OperatorType::Plus)
                std::cout << "+ ";
            else if(((OperatorToken*)token)->op_type == OperatorType::Minus)
                std::cout << "- ";
        }
        else if(token->type == TokenTypes::Number)
        {
            std::cout << ((NumberToken*)token)->value << ' ';
        }
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    std::string input = std::string(argv[1]);
    std::cout << "Input: " << input << std::endl;
    Lexer lexer(input + '\0');

    std::vector<Token *> tokens;
    Token *token;
    while ((token = lexer.getNextToken()) != nullptr)
    {
        tokens.push_back(token);
    }

    if(!checkParenthesesBalance(tokens))
    {
        std::cout << "Unbalanced parentheses!" << std::endl;
    }
    std::vector<std::string> errors = checkSyntax(tokens);
    if(errors.size() > 0)
    {
        for(auto error: errors)
            std::cout << error << std::endl;
    }
    else
    {
        std::cout << "Correct!" << std::endl;
    }

    auto postfix = from_infix_to_postfix(tokens);
    printTokens(postfix);
    
    SyntaxTree tree;
    tree.buildFromPostfix(postfix);
    tree.balanceTree();

    std::vector<Token*> rebalanced_expression = tree.postfix();
    printTokens(rebalanced_expression);
    
       return 0;
}
