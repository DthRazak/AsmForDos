#include <bitset>

#include "converter.h"

using OpR = OperationReg;
using SR = SegmentReg;

bool Converter::isNumber(const std::string& str) {
    if (std::isdigit(str[0]))
        return true;

    if (str.size() >= 2 && str[0] == '-' && std::isdigit(str[1]))
        return true;

    return false;
}

bool Converter::isConstant(const std::string& op) {
    return op.size() == 3 && op.front() == '\'' && op.back() == '\'';
}

bool Converter::isString(const std::string& op)
{
    return op.size() >= 3 && op.front() == '\"' && op.back() == '\"';
}

bool Converter::isRegister(const std::string& op) {
    return OperationRegisters.find(op) != OperationRegisters.end();
}

bool Converter::isSegmentRegister(const std::string& op) {
    return SegmentRegisters.find(op) != SegmentRegisters.end();
}

bool Converter::isMemory(const std::string& op) {
    auto spos = op.find_first_of('['),
         epos = op.find_last_of(']');
    auto addressation = op.substr(spos + 1, epos - spos - 1);

    bool isMem = false;
    if (RmInfo.find(addressation) != RmInfo.end())                              isMem = true;
    if (isNumber(addressation) && spos == 0 && epos == op.size() - 1)           isMem = true;
    if (!op.empty() && spos == std::string::npos && epos == std::string::npos)  isMem = true;

    return isMem;
}

std::string Converter::ToBinary(u32 num, u8 bytes){
    if (bytes == 1) {
        return std::bitset<8>(num).to_string();
    }
    else if (bytes == 2) {
        std::string bits = std::bitset<16>(num).to_string();
        return bits.substr(8, 8) + bits.substr(0, 8);
    }

    std::string bits = std::bitset<32>(num).to_string();

    return bits.substr(24, 8) + bits.substr(16, 8) + bits.substr(8, 8) + bits.substr(0, 8);
}

u8 Converter::ToByte(const std::string& str) {
    return std::stoi(str.c_str(), nullptr, 2);
}

std::unordered_map<std::string, OperationReg> Converter::OperationRegisters = {
    {"AL", OpR::AL}, {"BL", OpR::BL}, {"CL", OpR::CL}, {"DL", OpR::DL},
    {"AH", OpR::AH}, {"BH", OpR::BH}, {"CH", OpR::CH}, {"DH", OpR::DH},
    {"AX", OpR::AX}, {"BX", OpR::BX}, {"CX", OpR::CX}, {"DX", OpR::DX},
    {"SP", OpR::SP}, {"BP", OpR::DI}, {"BP", OpR::BP}, {"SI", OpR::SI},
    {"EAX", OpR::EAX}, {"EBX", OpR::EBX}, {"ECX", OpR::ECX}, {"EDX", OpR::EDX}
};

std::unordered_map<std::string, OperationRegInfo> Converter::OpRegisters = {
    {"AL", {OpR::AL, 1, "000"}}, {"BL", {OpR::BL, 1, "011"}},
    {"CL", {OpR::CL, 1, "001"}}, {"DL", {OpR::DL, 1, "010"}},
    {"AH", {OpR::AH, 1, "100"}}, {"BH", {OpR::BH, 1, "111"}},
    {"CH", {OpR::CH, 1, "101"}}, {"DH", {OpR::DH, 1, "110"}},
    {"SP", {OpR::SP, 2, "100"}}, {"DI", {OpR::DI, 2, "111"}},
    {"BP", {OpR::BP, 2, "101"}}, {"SI", {OpR::SI, 2, "110"}},
    {"AX", {OpR::AX, 2, "000"}}, {"BX", {OpR::BX, 2, "011"}},
    {"CX", {OpR::CX, 2, "001"}}, {"DX", {OpR::DX, 2, "010"}},
    {"EAX", {OpR::EAX, 4, "000"}}, {"EBX", {OpR::EBX, 4, "011"}},
    {"ECX", {OpR::ECX, 4, "001"}}, {"EDX", {OpR::EDX, 4, "010"}}
};

std::unordered_map<std::string, SegmentReg> Converter::SegmentRegisters = {
    {"ES", SR::ES}, {"SS", SR::SS}, {"DS", SR::DS}, {"CS", SR::CS},
    {"FS", SR::FS}, {"GS", SR::GS}
};

std::unordered_map<std::string, SegmentRegInfo> Converter::SegRegisters = {
    {"ES", {SR::ES, 2, "000"}}, {"SS", {SR::SS, 2, "010"}},
    {"DS", {SR::DS, 2, "011"}}, {"CS", {SR::CS, 2, "001"}},
    {"FS", {SR::FS, 2, "100"}}, {"GS", {SR::GS, 2, "101"}}
};

std::unordered_map<std::string, std::string> Converter::RmInfo = {
    {"BX+SI", "000"}, {"BX+DI", "001"}, {"BP+SI", "010"}, {"BP+DI", "011"},
    {"SI", "100"}, {"DI", "101"},  {"BP", "110"}, {"DIRECT", "110"}, {"BX", "111"}
};
