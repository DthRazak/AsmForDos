#ifndef OPERANDSTORE_H
#define OPERANDSTORE_H

#include <vector>
#include <functional>

#include "operand.h"

using OperandCreateFunc = std::function<std::shared_ptr<Operand>(const std::string&)>;

class OperandStore
{
public:
    static std::shared_ptr<Operand> TryToCreate(const std::string& op);

private:
    static const std::vector<OperandCreateFunc> Store;
};
#endif // OPERANDSTORE_H
