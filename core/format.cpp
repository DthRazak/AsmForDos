#include "format.h"
#include "converter.h"

Format::Format(const std::string& format, u16 Op1Type, u16 Op2Type)
    : formatStr(format), Op1Type(Op1Type), Op2Type(Op2Type)
{
}

bool Format::TryToFormat(
    std::shared_ptr<Operand>& op1,
    std::shared_ptr<Operand>& op2,
    std::unordered_map<std::string, u32>& SYMTAB,
    u32 LOCCTR,
    std::vector<u8>& resBytes,
    bool isFirstIteration
)
{
    if (!(op1->GetType() & Op1Type) || !(op2->GetType() & Op2Type))
        return false;

    if (isFirstIteration) {
        if ((op1->GetType() & Operand::Memory) || (Op1Type & Operand::Relative8) || (Op1Type & Operand::Relative16)) {
            std::static_pointer_cast<Memory>(op1)->TrySetAddress(SYMTAB);
        }
        if ((op2->GetType() & Operand::Memory) || (Op2Type & Operand::Relative8) || (Op2Type & Operand::Relative16)) {
            std::static_pointer_cast<Memory>(op2)->TrySetAddress(SYMTAB);
        }
    }
    else {
        bool isAddressSetted = false;
        if ((op1->GetType() & Operand::Memory) || (Op1Type & Operand::Relative8) || (Op1Type & Operand::Relative16)) {
            isAddressSetted |= std::static_pointer_cast<Memory>(op1)->TrySetAddress(SYMTAB);
        }
        if ((op2->GetType() & Operand::Memory) || (Op2Type & Operand::Relative8) || (Op2Type & Operand::Relative16)) {
            isAddressSetted |= std::static_pointer_cast<Memory>(op2)->TrySetAddress(SYMTAB);
        }
        if (!isAddressSetted) {
            return false;
        }
    }

    std::istringstream formatStream(formatStr);
    std::ostringstream result;

    char c = formatStream.get();
    bool isImSetted = false;
    std::string field;
    while (!formatStream.eof()) {
        if (c == 'w') {
            SetW(op1, op2, result);
        }
        else if (c == 's') {
            SetS(op1, op2, result);
        }
        else if (c == 'd') {
            SetD(op1, op2, result);
        }
        else if (c == ' ') {
            formatStream >> field;

            if (field == "mod") {
                SetMod(op1, op2, result);
            }
            else if (field == "reg") {
                SetReg(op1, op2, result);
            }
            else if (field == "sreg") {
                SetSReg(op1, op2, result);
            }
            else if (field == "r/m") {
                SetRM(op1, op2, result);
            }
            else if (field == "im") {
                if ((op1->GetType() & Operand::Type::Memory) || (op2->GetType() & Operand::Type::Memory)) {
                    SetAddressation(op1, op2, LOCCTR, result);
                }
                SetIm(op1, op2, result);
                isImSetted = true;
            }
            else if (std::isdigit(field[0])) {
                formatStream.seekg(-(int)field.size(), std::ios_base::cur);
            }
        }
        else {
            result << c;
        }

        c = formatStream.get();
    }

    if (!isImSetted && ((op1->GetType() & Operand::Type::Memory)
                        || (op2->GetType() & Operand::Type::Memory))) {
        SetAddressation(op1, op2, LOCCTR, result);
    }

    std::string binaryData = result.str();

    // Allocating two bytes for address displacement
    // In this case size % 8 != because " ?" symbol
    if (isFirstIteration) {
        if (binaryData.size() % 8 != 0) {
            for (u8 i = 0; i < binaryData.size() / 8 + 2; ++i) {
                resBytes.push_back(0);
            }
            return false;
        }
    }

    for (u8 i = 0; i < binaryData.size() / 8; ++i) {
        resBytes.push_back(Converter::ToByte(binaryData.substr(i * 8, 8)));
    }

    return true;
}

void Format::SetD(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
) {
    result << '1';
}

void Format::SetS(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
) {
    if (op1->GetBytesCount() != op2->GetBytesCount())
        result << '1';
    else
        result << '0';
}

void Format::SetW(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
) {
    u8  bytes1 = op1->GetBytesCount(),
        bytes2 = op2->GetBytesCount();
    if (Op2Type == Operand::Type::RegisterCL)
        result << '1';
    else if ((bytes2 != 0 && bytes2 < bytes1) || (bytes1 == 1))
        result << '0';
    else
        result << '1';
    //if (op1->GetBytesCount() == 1)
    //    result << '0';
    //else
    //    result << '1';
}

void Format::SetMod(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
) {
    u8  mod1 = op1->GetMod(),
        mod2 = op2->GetMod();

    switch (std::min(mod1, mod2))
    {
        case 0:     result << "00"; break;
        case 0b01:  result << "01"; break;
        case 0b10:  result << "10"; break;
        case 0b11:  result << "11"; break;
        default:    break;
    }
}

void Format::SetReg(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
) {
    if ((op1->GetType() & Operand::Register) && (op1->GetType() & Operand::Memory) && (op2->GetType() & Operand::Register))
        result << op2->GetValue();
    else if ((op2->GetType() & Operand::Register) && (op2->GetType() & Operand::Memory) && (op1->GetType() & Operand::Register))
        result << op1->GetValue();
    else if (op1->GetType() & Operand::Type::Register)
        result << op1->GetValue();
    else
        result << op2->GetValue();
}

void Format::SetSReg(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
)
{
    if (op1->GetType() & Operand::Type::SegmentRegister)
        result << op1->GetValue();
    else
        result << op2->GetValue();
}

void Format::SetRM(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
) {
    if (Op2Type == Operand::RegisterCL)
        result << op1->GetValue();
    else if ((op1->GetType() & Operand::Register) && (op1->GetType() & Operand::Memory) && (op2->GetType() & Operand::Register))
        result << op1->GetValue().substr(0, 3);
    else if ((op2->GetType() & Operand::Register) && (op2->GetType() & Operand::Memory) && (op1->GetType() & Operand::Register))
        result << op2->GetValue().substr(0, 3);
    else if (op1->GetType() & Operand::Memory)
        result << op1->GetValue().substr(0, 3);
    else if (op2->GetType() & Operand::Memory)
        result << op2->GetValue().substr(0, 3);
    else if (op2->GetType() & Operand::Immediate)
        result << op1->GetValue();
    else if (!(op2->GetType() & Operand::None))
        result << op2->GetValue();
    else
        result << op1->GetValue();
}

void Format::SetIm(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    std::ostringstream& result
) {
    if (op2->GetType() & Operand::Immediate) {
        if (Op2Type == Operand::Immediate8)
            result << Converter::ToBinary(op2->GetIm(), 1);
        else
            result << Converter::ToBinary(op2->GetIm(), op1->GetBytesCount());
    }
    else if (op1->GetType() & Operand::Immediate) {
        if (Op1Type == Operand::Immediate8)
            result << Converter::ToBinary(op1->GetIm(), 1);
        else
            result << Converter::ToBinary(op1->GetIm(), op2->GetBytesCount());
    }
    else{
        result << Converter::ToBinary(op1->GetIm(), op1->GetBytesCount());
    }
}

void Format::SetAddressation(
    const std::shared_ptr<Operand>& op1,
    const std::shared_ptr<Operand>& op2,
    u32 LOCCTR,
    std::ostringstream& result
) {
    std::string addressation;
    if (op1->GetType() & Operand::Type::Memory)
        addressation = op1->GetValue();
    else
        addressation = op2->GetValue();
    u32 instructionSize = result.str().size() / 8;
    if (addressation.size() != 3) {
        if (Op1Type & Operand::Type::Relative8 && addressation.back() != '?') {
            u32 digit;
            Converter::TryGetNumber("0B" + addressation.substr(4, 8), digit);
            result << Converter::ToBinary(digit - (LOCCTR + instructionSize + 1), 1);
        } else if (Op1Type & Operand::Type::Relative16 && addressation.back() != '?') {
            u32 digit;
            Converter::TryGetNumber("0B" + addressation.substr(12, 8) + addressation.substr(4, 8), digit);
            result << Converter::ToBinary(digit - (LOCCTR + instructionSize + 2), 2);
        }
        else {
            result << addressation.substr(4);
        }
    }
}
