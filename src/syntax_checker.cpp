#include "syntax_checker.h"
#include "token.h"
#include <exception>
#include <stack>

std::vector<std::string> checkSyntax(std::vector<Token *> tokens)
{
    std::vector<std::string> errors;
    FiniteStateMachine fsm;
    fsm.addState("Finish");
    fsm.addState("1");
    fsm.addState("2");
    fsm.addState("3");
    fsm.addState("4");
    fsm.addState("5");
    fsm.addState("6");

    fsm.addTransition("Initial state", "1", "(");
    fsm.addTransition("Initial state", "2", "id");
    fsm.addTransition("Initial state", "4", "func");
    fsm.addTransition("Initial state", "3", "number");

    fsm.addTransition("1", "2", "id");
    fsm.addTransition("1", "1", "(");
    fsm.addTransition("1", "3", "number");
    fsm.addTransition("1", "4", "func");

    fsm.addTransition("2", "5", "op");
    fsm.addTransition("2", "6", ")");
    fsm.addTransition("2", "Finish", "EOF");

    fsm.addTransition("3", "5", "op");
    fsm.addTransition("3", "6", ")");
    fsm.addTransition("3", "Finish", "EOF");

    fsm.addTransition("4", "1", "(");

    fsm.addTransition("5", "1", "(");
    fsm.addTransition("5", "2", "id");
    fsm.addTransition("5", "3", "number");
    fsm.addTransition("5", "4", "func");

    fsm.addTransition("6", "5", "op");
    fsm.addTransition("6", "6", ")");
    fsm.addTransition("6", "Finish", "EOF");
    fsm.addFinishState("Finish");

    for (Token *token : tokens)
    {
        auto cur_state = fsm.getState();
        switch (token->type)
        {
        case TokenTypes::Number:
            fsm.move("number");
            if (fsm.getState() == "Failed")
                {
                    errors.push_back("Wrong number usage");
                    fsm.setState(cur_state);
                }
            break;
        case TokenTypes::Id:
            fsm.move("id");
            if (fsm.getState() == "Failed")
                {
                    errors.push_back("Wrong identified usage");
                    fsm.setState(cur_state);
                }
            break;
        case TokenTypes::Function:
            fsm.move("func");
            if (fsm.getState() == "Failed")
                {
                    errors.push_back("Wrong function usage");
                    fsm.setState(cur_state);
                }
            break;
        case TokenTypes::Operator:
            fsm.move("op");
            if (fsm.getState() == "Failed")
                {
                    errors.push_back("Wrong operator usage");
                    fsm.setState(cur_state);
                }
            break;
        case TokenTypes::Bracket:
            if (((BracketToken *)token)->bracket_type == BracketType::LeftBracket)
                fsm.move("(");
            else if ((((BracketToken *)token)->bracket_type == BracketType::RightBraket))
                fsm.move(")");
            if (fsm.getState() == "Failed")
                {
                    errors.push_back("Wrong parentheses usage");
                    fsm.setState(cur_state);
                }
            break;
        }
    }
    fsm.move("EOF");
    return errors;
}

bool checkParenthesesBalance(std::vector<Token *> tokens)
{
    std::stack<BracketToken> parentheses;

    for (Token *token : tokens)
    {
        if (token->type == TokenTypes::Bracket)
        {
            BracketToken *braket_token = (BracketToken *)token;
            if (braket_token->bracket_type == BracketType::LeftBracket)
                parentheses.push(*braket_token);
            if (braket_token->bracket_type == BracketType::RightBraket)
            {
                if (parentheses.size() == 0)
                    return false;
                parentheses.pop();
            }
        }
    }

    if (parentheses.size() != 0)
        return false;
    return true;
}