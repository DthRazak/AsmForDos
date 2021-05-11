#ifndef OPERATOR_H
#define OPERATOR_H

#include "instruction.h"
#include "operand.h"
#include "format.h"

class Operator : public Instruction
{
public:
    struct Info {
        std::string Label;
        std::string Name;
        std::string OP1;
        std::string OP2;
    };

    std::vector<u8> GetBinaryData(
        std::unordered_map<std::string, u32>& SYMTAB,
        u32& LOCCTR,
        bool isFirstIteration
    ) override;

    static Info GetInfo(const std::vector<std::string>& tokens);
    static std::shared_ptr<Instruction> Create(const std::vector<std::string>& tokens);

    friend std::ostream& operator<<(std::ostream& os, const Operator* op);

protected:
    Operator(
        const std::string& label,
        const std::string &name,
        std::shared_ptr<Operand>& op1,
        std::shared_ptr<Operand>& op2,
        const std::vector<Format> &formats
    );

    std::string Label;
    std::string Name;
    std::shared_ptr<Operand> OP1;
    std::shared_ptr<Operand> OP2;
    std::vector<Format> Formats;

private:
    Operator() = default;
};

#endif // OPERATOR_H
