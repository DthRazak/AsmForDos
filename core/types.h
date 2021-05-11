#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <string>

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;

enum class OperationReg : u8
{
    AL = 0b000, AX = 0b000, EAX = 0b000,
    CL = 0b001, CX = 0b001, ECX = 0b001,
    DL = 0b010, DX = 0b010, EDX = 0b010,
    BL = 0b011, BX = 0b011, EBX = 0b011,
    AH = 0b100, CH = 0b101, DH = 0b110, BH = 0b111,
    SP = 0b100, BP = 0b101, SI = 0b110, DI = 0b111,
};

enum class SegmentReg : u8
{
    ES = 0b000,
    CS = 0b001,
    SS = 0b010,
    DS = 0b011,
    FS = 0b100,
    GS = 0b101
};

struct OperationRegInfo
{
    OperationReg Type;
    u8 Bytes;
    std::string Code;
};

struct SegmentRegInfo
{
    SegmentReg Type;
    u8 Bytes;
    std::string Code;
};

#endif // TYPES_H
