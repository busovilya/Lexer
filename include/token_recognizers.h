#include "finite_state_machine.h"

class TokenRecognizer
{
protected: 
    FiniteStateMachine fsm;
public:
    virtual bool putChar(char);
    virtual bool accepted();
    virtual bool failed();
    virtual void reset();
};

class NumberRecognizer : public TokenRecognizer
{
public:
    NumberRecognizer();
};

class OperationRecognizer : public TokenRecognizer
{
public:
    OperationRecognizer();
};

class IdRecognizer : public TokenRecognizer
{
public:
    IdRecognizer();
};

class BracketsRecognizer : public TokenRecognizer
{
public:
    BracketsRecognizer();
};

class FunctionRecognizer : public TokenRecognizer
{
public:
    FunctionRecognizer();
};