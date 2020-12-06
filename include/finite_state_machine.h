#pragma once

#include <unordered_set>
#include <map>
#include <string>

class FiniteStateMachine
{
public:
    FiniteStateMachine();
    void addState(std::string);
    void addTransition(std::string, std::string, std::string);
    void addFinishState(std::string);
    bool isFinished();
    bool move(std::string);
    std::string getState();
    void setState(std::string);
    void clear();
private:
    std::unordered_set<std::string> states;
    std::map<std::string, std::map<std::string, std::string>> transitions;
    std::string current_state;
    std::unordered_set<std::string> final_states;
};