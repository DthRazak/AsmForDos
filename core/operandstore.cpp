#include "operandstore.h"

const std::vector<OperandCreateFunc> OperandStore::Store = {
    &Constant::TryToCreate,
    &Register::TryToCreate,
    &Memory::TryToCreate
};


std::shared_ptr<Operand> OperandStore::TryToCreate(const std::string& op) {
    for (auto func : Store) {
        auto result = func(op);
        if (result != nullptr)  return result;
    }

    return std::shared_ptr<Operand>(new Operand(op));
}
