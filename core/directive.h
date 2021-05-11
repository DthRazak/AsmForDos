#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include "Instruction.h"
#include "types.h"

class Directive : public Instruction
{
public:
    static std::shared_ptr<Instruction> Create(const std::vector<std::string>& tokens);

    virtual std::vector<u8> GetBinaryData(
        std::unordered_map<std::string, u32>& SYMTAB,
        u32& LOCCTR,
        bool isFirstIteration
    ) override;

    friend std::ostream& operator<<(std::ostream& os, const Directive* directive);

private:
    Directive(const std::string& name, const std::string& instruction);

    std::string name;
    std::string instruction;
    std::vector<u16> values;
};

#endif // DIRECTIVE_H
