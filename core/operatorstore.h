#ifndef OPERATORSTORE_H
#define OPERATORSTORE_H

#include <vector>
#include <unordered_map>
#include <functional>

#include "operator.h"

class OperatorStore
{
public:
    static std::vector<Format> GetFormats(const std::string& opName);

    static std::unordered_map<std::string, std::vector<Format>> MovingOperators;
    static std::unordered_map<std::string, std::vector<Format>> ArifmeticOperators;
    static std::unordered_map<std::string, std::vector<Format>> LogicOperators;
    static std::unordered_map<std::string, std::vector<Format>> BitOperators;
    static std::unordered_map<std::string, std::vector<Format>> ControlOperators;
    static std::unordered_map<std::string, std::vector<Format>> ConditionOperators;
    static std::unordered_map<std::string, std::vector<Format>> InterruptsOperators;
};
#endif // OPERATORSTORE_H
