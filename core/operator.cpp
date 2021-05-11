#include "operator.h"
#include "operandstore.h"
#include "operatorstore.h"
#include "exceptions.h"


Operator::Operator(
    const std::string& label,
    const std::string& name,
    std::shared_ptr<Operand>& op1,
    std::shared_ptr<Operand>& op2,
    const std::vector<Format>& formats
)
    : Label(label), Name(name), OP1(op1), OP2(op2), Formats(formats)
{
}

Operator::Info Operator::GetInfo(const std::vector<std::string>& tokens) {
    Operator::Info info;

    if (tokens.size() > 0) {
        if (tokens[0].back() == ':') {
            info.Label = tokens[0];
            info.Label.pop_back();
            if (tokens.size() > 1)  info.Name = tokens[1];
            if (tokens.size() > 2)  info.OP1 = tokens[2];
            if (tokens.size() > 3)  info.OP2 = tokens[3];
        }
        else {
            info.Name = tokens[0];
            if (tokens.size() > 1)  info.OP1 = tokens[1];
            if (tokens.size() > 2)  info.OP2 = tokens[2];
        }
    }

    return info;
}

std::shared_ptr<Instruction> Operator::Create(const std::vector<std::string>& tokens) {
    auto opInfo = Operator::GetInfo(tokens);
    auto formats = OperatorStore::GetFormats(opInfo.Name);

    if (!formats.empty()) {
        auto OP1 = OperandStore::TryToCreate(opInfo.OP1);
        auto OP2 = OperandStore::TryToCreate(opInfo.OP2);

        if ((OP1->GetType() & Operand::Memory) && ((OP2->GetType() & Operand::Memory)))
            throw InvalidOperandException(
                "Memory to memory addressation is not supported", opInfo.OP1, opInfo.OP2);

        if ((OP1->GetType() & Operand::Immediate) && ((OP2->GetType() & Operand::Immediate)))
            throw InvalidOperandException(
                "Immediate to immediate is not allowed", opInfo.OP1, opInfo.OP2);

        return std::shared_ptr<Operator>(
            new Operator(opInfo.Label, opInfo.Name, OP1, OP2, formats)
        );
    }

    return nullptr;
}

std::vector<u8> Operator::GetBinaryData(
    std::unordered_map<std::string, u32>& SYMTAB,
    u32& LOCCTR,
    bool isFirstIteration
)
{
    std::vector<u8> res;

    if (isFirstIteration){
        if (!Label.empty())
            SYMTAB.insert(std::make_pair(Label, LOCCTR));

        bool isFormatted = false;
        for (Format& format : Formats) {
            isFormatted = format.TryToFormat(OP1, OP2, SYMTAB, LOCCTR, res, isFirstIteration);
            if (isFormatted)                    break;
            if (!isFormatted && res.size() > 0) break;
        }

        LOCCTR += res.size();

        if (!isFormatted && res.size() == 0) {
            throw InstructionFormatException(Name, OP1->GetName(), OP2->GetName());
        }

        // Reaches when memory operand doesn't have address durring first iteration
        // Clean neads to detect empty data and re-execute this function
        if (!isFormatted && res.size() > 0) {
            res.clear();
        }
    }
    else {
        for (Format& format : Formats) {
            if (format.TryToFormat(OP1, OP2, SYMTAB, LOCCTR, res, isFirstIteration))
                break;
        }

        if (res.empty()) {
            if (OP1->GetType() & Operand::Memory) {
                throw AddressResolveException(OP1->GetName());
            }
            else {
                throw AddressResolveException(OP2->GetName());
            }
        }
    }

    return std::move(res);
}

std::ostream& operator<<(std::ostream& os, const Operator* op)
{
    os << "Operator - Label: " << op->Label
        << " Name: " << op->Name
        << "\nOP1: " << *op->OP1
        << "OP2: " << *op->OP2 << std::endl;
    return os;
}
