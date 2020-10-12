#include "token_recognizers.h"

void TokenRecognizer::reset()
{
    fsm.clear();
}

bool TokenRecognizer::accepted()
{
    return fsm.isFinished();
}

bool TokenRecognizer::failed()
{
    return fsm.getState().compare("Failed") == 0;
}

bool TokenRecognizer::putChar(char ch)
{
    if (ch == '\0')
        return fsm.move("EOF");
    return fsm.move(std::string(1, ch));
}

OperationRecognizer::OperationRecognizer()
{
    fsm.addState("1");
    fsm.addState("Finish");
    fsm.addTransition("Initial state", "1", "+");
    fsm.addTransition("Initial state", "1", "-");
    fsm.addTransition("Initial state", "1", "*");
    fsm.addTransition("Initial state", "1", "/");

    for (int i = 0; i <= 9; i++)
        fsm.addTransition("1", "Finish", std::to_string(i));
    for (char i = 'a'; i != 'z'; i++)
        fsm.addTransition("1", "Finish", std::string(1, i));
    for (char i = 'A'; i != 'Z'; i++)
        fsm.addTransition("1", "Finish", std::string(1, i));
    fsm.addTransition("1", "Finish", "_");
    fsm.addTransition("1", "Finish", "EOF");
    fsm.addFinishState("Finish");
}

NumberRecognizer::NumberRecognizer()
{
    fsm.addState("1");
    fsm.addState("2");
    fsm.addState("3");
    
    for (int i = 1; i <= 9; i++)
        fsm.addTransition("Initial state", "1", std::to_string(i));
    for (int i = 0; i <= 9; i++)
        fsm.addTransition("1", "1", std::to_string(i));
    fsm.addTransition("1", "2", ".");
    for(int i = 0; i <= 9; i++)
    {
        fsm.addTransition("2", "3", std::to_string(i));
        fsm.addTransition("3", "3", std::to_string(i));
    }
    fsm.addState("Finish");

    for (char i = 'a'; i != 'z'; i++)
        fsm.addTransition("3", "Finish", std::string(1, i));
    for (char i = 'A'; i != 'Z'; i++)
        fsm.addTransition("3", "Finish", std::string(1, i));
    fsm.addTransition("3", "Finish", "+");
    fsm.addTransition("3", "Finish", "-");
    fsm.addTransition("3", "Finish", "*");
    fsm.addTransition("3", "Finish", "/");
    fsm.addTransition("3", "Finish", ".");
    fsm.addTransition("3", "Finish", "_");
    fsm.addTransition("3", "Finish", "(");
    fsm.addTransition("3", "Finish", ")");
    fsm.addTransition("3", "Finish", "EOF");
    fsm.addFinishState("Finish");
}

IdRecognizer::IdRecognizer()
{
    fsm.addState("1");
    fsm.addState("2");
    fsm.addState("Finish");

    for (int i = 'a'; i <= 'z'; i++)
        fsm.addTransition("Initial state", "1", std::string(1, i));
    for (int i = 'A'; i <= 'Z'; i++)
        fsm.addTransition("Initial state", "1", std::string(1, i));
    for (char i = 'a'; i != 'z'; i++)
        fsm.addTransition("1", "1", std::string(1, i));
    for (char i = 'A'; i != 'Z'; i++)
        fsm.addTransition("1", "1", std::string(1, i));
    for(int i = 0; i <= 9; i++)
        fsm.addTransition("1", "1", std::to_string(i));
    fsm.addTransition("1", "1", "_");
    
    fsm.addTransition("1", "Finish", "+");
    fsm.addTransition("1", "Finish", "-");
    fsm.addTransition("1", "Finish", "*");
    fsm.addTransition("1", "Finish", "/");
    fsm.addTransition("1", "Finish", ".");
    fsm.addTransition("1", "Finish", ")");
    fsm.addTransition("1", "Finish", "(");
    fsm.addTransition("1", "Finish", "EOF");
    fsm.addFinishState("Finish");
}

BracketsRecognizer::BracketsRecognizer()
{
    fsm.addState("1");
    fsm.addState("Finish");

    fsm.addTransition("Initial state", "1", "(");
    fsm.addTransition("Initial state", "1", ")");

    for (char i = 'a'; i != 'z'; i++)
        fsm.addTransition("1", "Finish", std::string(1, i));
    for (char i = 'A'; i != 'Z'; i++)
        fsm.addTransition("1", "Finish", std::string(1, i));
    for (char i = 0; i <= 9; i++)
        fsm.addTransition("1", "Finish", std::to_string(i));

    fsm.addTransition("1", "Finish", "+");
    fsm.addTransition("1", "Finish", "-");
    fsm.addTransition("1", "Finish", "*");
    fsm.addTransition("1", "Finish", "/");
    fsm.addTransition("1", "Finish", ".");
    fsm.addTransition("1", "Finish", "_");
    fsm.addTransition("1", "Finish", ")");
    fsm.addTransition("1", "Finish", "(");
    fsm.addTransition("1", "Finish", "EOF");
    fsm.addFinishState("Finish");
}

FunctionRecognizer::FunctionRecognizer()
{
    fsm.addState("1");
    fsm.addState("2");
    fsm.addState("3");
    fsm.addState("4");
    fsm.addState("5");
    fsm.addState("6");
    fsm.addState("Finish");

    fsm.addTransition("Initial state", "1", "s");
    fsm.addTransition("1", "2", "i");
    fsm.addTransition("2", "3", "n");

    fsm.addTransition("Initial state", "4", "c");
    fsm.addTransition("4", "5", "o");
    fsm.addTransition("5", "6", "s");

    for(char i = 'a'; i != 'z'; i++)
    {
        fsm.addTransition("3", "Finish", std::string(1, i));
        fsm.addTransition("3", "Finish", std::string(1, i - 'a' + 'A'));
        fsm.addTransition("6", "Finish", std::string(1, i));
        fsm.addTransition("6", "Finish", std::string(1, i - 'a' + 'A'));
    }
    fsm.addTransition("3", "Finish", "+");
    fsm.addTransition("3", "Finish", "-");
    fsm.addTransition("3", "Finish", "*");
    fsm.addTransition("3", "Finish", "/");
    fsm.addTransition("3", "Finish", ".");
    fsm.addTransition("3", "Finish", ")");
    fsm.addTransition("3", "Finish", "(");
    fsm.addTransition("3", "Finish", "EOF");

    fsm.addTransition("6", "Finish", "+");
    fsm.addTransition("6", "Finish", "-");
    fsm.addTransition("6", "Finish", "*");
    fsm.addTransition("6", "Finish", "/");
    fsm.addTransition("6", "Finish", ".");
    fsm.addTransition("6", "Finish", ")");
    fsm.addTransition("6", "Finish", "(");
    fsm.addTransition("6", "Finish", "EOF");
    fsm.addFinishState("Finish");
}