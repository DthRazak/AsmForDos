#ifndef CONVERTER_H
#define CONVERTER_H

#include "types.h"
#include <bitset>
#include <unordered_map>

class Converter
{
public:
    static std::unordered_map<std::string, OperationReg> OperationRegisters;
    static std::unordered_map<std::string, OperationRegInfo> OpRegisters;
    static std::unordered_map<std::string, SegmentReg> SegmentRegisters;
    static std::unordered_map<std::string, SegmentRegInfo> SegRegisters;
    static std::unordered_map<std::string, std::string> RmInfo;

    static bool isNumber(const std::string& op);
    static bool isConstant(const std::string& op);
    static bool isString(const std::string& op);
    static bool isRegister(const std::string& op);
    static bool isSegmentRegister(const std::string& op);
    static bool isMemory(const std::string& op);

    static std::string ToBinary(u32 num, u8 bytes);
    static u8 ToByte(const std::string& str);

    template<typename T = u16>
    static bool TryGetNumber(const std::string& str, T& digit);

};

template<typename T>
bool Converter::TryGetNumber(const std::string& str, T& digit) {
    bool success = true;

    try {
        if (str.size() > 2) {
            if (str[1] == 'B' || str[1] == 'b') {
                digit = std::bitset<16>(str.c_str() + 2).to_ulong();
            }
            else if (str[1] == 'Q' || str[1] == 'q') {
                digit = std::stoi(str.c_str() + 2, nullptr, 8);
            }
            else if (str[1] == 'X' || str[1] == 'x') {
                digit = std::stoi(str.c_str() + 2, nullptr, 16);
            }
        }
        else if (std::isdigit(str[0])) {
            digit = std::stoi(str.c_str(), nullptr, 10);
        }
        else {
            success = false;
        }
    }
    catch (std::exception& ex) {
        success = false;
    }

    return success;
}


#endif // CONVERTER_H
