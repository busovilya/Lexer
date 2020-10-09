#include "finite_state_machine.h"

FiniteStateMachine::FiniteStateMachine()
{
    states.insert("Initial state");
    states.insert("Failed");
    current_state = "Initial state";
}

void FiniteStateMachine::addState(std::string state)
{
    states.insert(state);
}

void FiniteStateMachine::addTransition(std::string from, std::string to, std::string cond)
{
    if(transitions.find(from) == transitions.end())
        transitions.insert(std::make_pair(from, std::map<std::string, std::string>()));
    transitions.find(from)->second.insert(std::make_pair(cond, to));
}

bool FiniteStateMachine::move(std::string input)
{
    auto values = transitions.find(current_state)->second;
    if(values.find(input) != values.end())
    {
        current_state = values.find(input)->second;
        return true;
    }    
    current_state = "Failed";
    return false;
}

std::string FiniteStateMachine::getState()
{
    return current_state;
}

void FiniteStateMachine::addFinishState(std::string state)
{
    final_states.insert(state);
}

bool FiniteStateMachine::isFinished()
{
    return final_states.find(current_state) != final_states.end();
}

void FiniteStateMachine::clear()
{
    current_state = "Initial state";
}