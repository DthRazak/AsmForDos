#include <iostream>

#include "operand.h"
#include "converter.h"

/* BASE class Operand */

Operand::Operand(const std::string& name)
    : type(Type::None), mod(255), im(0), bytes(0), name(name)
{
}

u32 Operand::GetIm() const {
    return im;
}

u16 Operand::GetType() const {
    return type;
}

std::string Operand::GetValue() const {
    return value;
}

std::string Operand::GetName() const
{
    return name;
}

u8 Operand::GetMod() const {
    return mod;
}

u8 Operand::GetBytesCount() const {
    return bytes;
}

std::ostream& operator<<(std::ostream& os, const Operand& op) {
    os << "Operand(Type: " << op.type
        << " Value: " << op.value
        << " Bytes: " << (int)op.bytes
        << " Mod: " << std::bitset<2>(op.mod)
        << ")" << std::endl;

    return os;
}

/* class Constant */

Constant::Constant(const std::string& name, u32 num, u8 bytes)
    : Operand(name)
{
    im = num;
    this->bytes = bytes;
    type = Operand::Type::Immediate;
    type |= Operand::Type::Immediate8;
}

std::shared_ptr<Operand> Constant::TryToCreate(const std::string& str) {
    if (Converter::isNumber(str)) {
        u32 num;
        if (str[0] == '-') {
            if (Converter::TryGetNumber(str.substr(1), num)) {
                // negative number
                if (num > std::numeric_limits<i8>::max()) {
                    return std::shared_ptr<Constant>(new Constant(str, -(i32)num, 2));
                }
                else {
                    return std::shared_ptr<Constant>(new Constant(str, -(i32)num, 1));
                }
            }
        }
        else {
            if (Converter::TryGetNumber(str, num)){
                // positive number
                if (num > std::numeric_limits<i8>::max()) {
                    return std::shared_ptr<Constant>(new Constant(str, num, 2));
                }
                else {
                    return std::shared_ptr<Constant>(new Constant(str, num, 1));
                }
            }
        }
    }
    else if (Converter::isConstant(str)) {
        return std::shared_ptr<Constant>(new Constant(str, str[1], 1));
    }

    return nullptr;
}

/* class Register */

Register::Register(const std::string& name, OperationRegInfo info)
    : Operand(name)
{
    mod = 0b11;
    bytes = info.Bytes;
    value = info.Code;

    if (info.Type == OperationReg::AX)
        type = Type::RegisterAX;
    else if (info.Type == OperationReg::CX)
        type = Type::RegisterCX;
    else if (info.Type == OperationReg::DX)
        type = Type::RegisterDX;
    else
        type = 0;

    if (name == "CL")
        type |= Type::RegisterCL;

    if (info.Type == OperationReg::AX || info.Type == OperationReg::BX
        || info.Type == OperationReg::CX || info.Type == OperationReg::DX
        || info.Type == OperationReg::SP || info.Type == OperationReg::BP
        || info.Type == OperationReg::SI || info.Type == OperationReg::DI
        ) {
        type |= Type::IndexRegister;
    }

    type |= Type::Register;
}

Register::Register(const std::string& name, SegmentRegInfo info)
    : Operand(name)
{
    mod = 0b11;
    bytes = info.Bytes;
    value = info.Code;
    type = Type::SegmentRegister;
    if (info.Type == SegmentReg::FS || info.Type == SegmentReg::GS) {
        type |= Type::ExtendedSegmentRegister;
    }
    else {
        type |= Type::MainSegmentRegister;
        value = value.substr(1);
    }
}

std::shared_ptr<Operand> Register::TryToCreate(const std::string& op) {
    if (Converter::isRegister(op))
        return std::shared_ptr<Register>(
                new Register(op, Converter::OpRegisters[op])
            );
    else if (Converter::isSegmentRegister(op))
        return std::shared_ptr<Register>(
                new Register(op, Converter::SegRegisters[op])
            );

    return nullptr;
}

/* class Memory */

Memory::Memory(const std::string& name, u8 mod, const std::string& rm, u16 disp)
    : Operand(name), hasValue(true)
{
    this->mod = mod;
    bytes = 2;
    type = Operand::Type::Memory;

    if (disp == 0 && rm != "110")
        value = rm;
    else
        value = rm + ' ' + Converter::ToBinary(disp, bytes);
}

Memory::Memory(const std::string& name, u8 mod, const std::string& rm, const std::string& var)
    : Operand(name), hasValue(false)
{
    this->mod = mod;
    this->var = var;
    bytes = 2;
    value = rm + " ?";
    type = Operand::Type::Memory;
    if (mod == 0)
        type |= Operand::Type::Relative8 | Operand::Type::Relative16;
}

std::shared_ptr<Operand> Memory::TryToCreate(const std::string& op) {
    if (Converter::isMemory(op)) {
        auto spos = op.find_first_of('['),
            epos = op.find_last_of(']');
        auto addressation = op.substr(spos + 1, epos - spos - 1);

        if (!op.empty() && spos == std::string::npos && epos == std::string::npos) { // Table
            // Direct - e.g. TABLE
            return std::shared_ptr<Memory>(
                new Memory(op, 0b00, Converter::RmInfo["DIRECT"], op)
            );
        }
        else if (!op.empty() && Converter::isNumber(addressation)) {
            u16 num;
            Converter::TryGetNumber(addressation, num);
            return std::shared_ptr<Memory>(
                new Memory(op, 0b00, Converter::RmInfo["DIRECT"], num)
            );
        }
        else if (op.front() != '[') {
            auto var = op.substr(0, spos);
            return std::shared_ptr<Memory>(
                new Memory(op, 0b10, Converter::RmInfo[addressation], var)
            );
        }
        else if (op.back() != ']') {
            auto num_str = op.substr(epos + 2, op.size() - epos - 1);
            i16 num;
            Converter::TryGetNumber(num_str, num);
            if (op[epos+1] == '+'){
                return std::shared_ptr<Memory>(
                    new Memory(op, 0b10, Converter::RmInfo[addressation], num)
                );
            }
            else {
                return std::shared_ptr<Memory>(
                    new Memory(op, 0b10, Converter::RmInfo[addressation], std::numeric_limits<u16>::max() - num + 1)
                );
            }
        }
        else {
            if (addressation != "BP") {
                return std::shared_ptr<Memory>(
                    new Memory(op, 0b00, Converter::RmInfo[addressation], 0)
                );
            }
            else {
                return std::shared_ptr<Memory>(
                    new Memory(op, 0b10, Converter::RmInfo[addressation], 0)
                );
            }
        }
    }

    return nullptr;
}

bool Memory::TrySetAddress(const std::unordered_map<std::string, u32>& SYMTAB) {
    if (!hasValue) {
        auto it = SYMTAB.find(var);
        if (it != SYMTAB.end()) {
            value = value.substr(0, 4) + Converter::ToBinary(it->second, bytes);

            hasValue = true;
        }
    }

    return hasValue;
}
