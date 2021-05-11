#ifndef FORMAT_H
#define FORMAT_H

#include <sstream>
#include <bitset>
#include <vector>

#include "operand.h"

class Format
{
public:
    Format(const std::string& format, u16 Op1Type, u16 Op2Type);

    bool TryToFormat(
        std::shared_ptr<Operand>& op1,
        std::shared_ptr<Operand>& op2,
        std::unordered_map<std::string, u32>& SYMTAB,
        u32 LOCCTR,
        std::vector<u8>& resBytes,
        bool isFirstIteration
    );

private:
    std::string formatStr;
    u16 Op1Type, Op2Type;

    void SetD(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream& result);
    void SetS(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream& result);
    void SetW(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream& result);
    void SetMod(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream &result);
    void SetReg(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream &result);
    void SetSReg(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream &result);
    void SetRM(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream &result);
    void SetIm(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, std::ostringstream &result);
    void SetAddressation(const std::shared_ptr<Operand>& op1, const std::shared_ptr<Operand>& op2, u32 LOCCTR, std::ostringstream &result);
};
#endif // FORMAT_H
