#include "operatorstore.h"

std::vector<Format> OperatorStore::GetFormats(const std::string& opName)
{
    auto arifmeticIt = ArifmeticOperators.find(opName);
    if (arifmeticIt != ArifmeticOperators.end())
        return arifmeticIt->second;

    auto movingIt = MovingOperators.find(opName);
    if (movingIt != MovingOperators.end())
        return movingIt->second;

    auto logicIt = LogicOperators.find(opName);
    if (logicIt != LogicOperators.end())
        return logicIt->second;

    auto bitIt = BitOperators.find(opName);
    if (bitIt != BitOperators.end())
        return bitIt->second;

    auto controlIt = ControlOperators.find(opName);
    if (controlIt != ControlOperators.end())
        return controlIt->second;

    auto conditionIt = ConditionOperators.find(opName);
    if (conditionIt != ConditionOperators.end())
        return conditionIt->second;

    auto interruptsIt = InterruptsOperators.find(opName);
    if (interruptsIt != InterruptsOperators.end())
        return interruptsIt->second;

    return std::vector<Format>();
}

std::unordered_map<std::string, std::vector<Format>> OperatorStore::ArifmeticOperators = {
    {
        "ADD",
        std::vector<Format>
        {
            Format("0000010w im", Operand::RegisterAX, Operand::Immediate),
            Format("000000dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0000000w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0000001w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 000 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "ADC",
        std::vector<Format>
        {
            Format("0001010w im", Operand::RegisterAX, Operand::Immediate),
            Format("000100dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0001000w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0001001w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 010 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "INC",
        std::vector<Format>
        {
            Format("01000 reg", Operand::Register, Operand::None),
            Format("1111111w mod 000 r/m", Operand::Register | Operand::Memory, Operand::None)
        }
    },
    {
        "AAA",
        std::vector<Format>
        {
            Format("00111111", Operand::None, Operand::None)
        }
    },
    {
        "DAA",
        std::vector<Format>
        {
            Format("00100111", Operand::None, Operand::None)
        }
    },
    {
        "SUB",
        std::vector<Format>
        {
            Format("0010110w im", Operand::RegisterAX, Operand::Immediate),
            Format("001010dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0010100w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0010101w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 101 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "SBB",
        std::vector<Format>
        {
            Format("0001110w im", Operand::RegisterAX, Operand::Immediate),
            Format("000110dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0001100w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0001101w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 011 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "DEC",
        std::vector<Format>
        {
            Format("01001 reg", Operand::Register, Operand::None),
            Format("1111111w mod 001 r/m", Operand::Register | Operand::Memory, Operand::None)
        }
    },
    {
        "CMP",
        std::vector<Format>
        {
            Format("0011110w im", Operand::RegisterAX, Operand::Immediate),
            Format("001110dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0011100w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0011101w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 111 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "NEG",
        std::vector<Format>
        {
            Format("1111011w mod 011 r/m", Operand::Register | Operand::Memory, Operand::None)
        }
    },
    {
        "AAS",
        std::vector<Format>
        {
            Format("00111111", Operand::None, Operand::None)
        }
    },
    {
        "DAS",
        std::vector<Format>
        {
            Format("00101111", Operand::None, Operand::None)
        }
    },
    {
        "MUL",
        std::vector<Format>
        {
            Format("1111011w mod 100 r/m", Operand::Register | Operand::Memory,  Operand::None)
        }
    },
    {
        "IMUL",
        std::vector<Format>
        {
            Format("1111011w mod 101 r/m", Operand::Register | Operand::Memory,  Operand::None),
            Format("0000111110101111 mod reg r/m", Operand::Register, Operand::Register | Operand::Memory),
        }
    },
    {
        "AAM",
        std::vector<Format>
        {
            Format("1101010000001010", Operand::None, Operand::None)
        }
    },
    {
        "DIV",
        std::vector<Format>
        {
            Format("1111011w mod 110 r/m", Operand::Register | Operand::Memory,  Operand::None)
        }
    },
    {
        "IDIV",
        std::vector<Format>
        {
            Format("1111011w mod 111 r/m", Operand::Register | Operand::Memory,  Operand::None)
        }
    },
    {
        "AAD",
        std::vector<Format>
        {
            Format("1101010100001010", Operand::None, Operand::None)
        }
    },
    {
        "CBW",
        std::vector<Format>
        {
            Format("10011000", Operand::None, Operand::None)
        }
    },
    {
        "CWDE",
        std::vector<Format>
        {
            Format("10011000", Operand::None, Operand::None)
        }
    },
    {
        "CWD",
        std::vector<Format>
        {
            Format("10011001", Operand::None, Operand::None)
        }
    }
};

std::unordered_map<std::string, std::vector<Format>> OperatorStore::MovingOperators = {
    {
        "MOV",
        std::vector<Format>
        {
            Format("1000100w mod reg r/m", Operand::Register | Operand::Memory, Operand::Register),
            Format("1000101w mod reg r/m", Operand::Register, Operand::Register | Operand::Memory),
            Format("1011w reg im", Operand::Register, Operand::Immediate),
            Format("1100011w mod 000 r/m im", Operand::Register | Operand::Memory, Operand::Immediate),
            Format("10001110 mod sreg r/m", Operand::SegmentRegister, Operand::Register | Operand::Memory),
            Format("10001100 mod sreg r/m", Operand::Register | Operand::Memory, Operand::SegmentRegister)
        }
    },
    {
        "PUSH",
        std::vector<Format>
        {
            Format("11111111 mod 110 r/m", Operand::Memory, Operand::None),
            Format("01010 reg", Operand::Register, Operand::None),
            Format("0000111110 sreg 000", Operand::ExtendedSegmentRegister, Operand::None),
            Format("000 sreg 110", Operand::MainSegmentRegister, Operand::None)
        }
    },
    {
        "PUSHA",
        std::vector<Format>
        {
            Format("01100000", Operand::None, Operand::None)
        }
    },
    {
        "POP",
        std::vector<Format>
        {
            Format("10001111 mod 000 r/m", Operand::Memory, Operand::None),
            Format("01011 reg", Operand::IndexRegister, Operand::None),
            Format("0000111110 sreg 001", Operand::ExtendedSegmentRegister, Operand::None),
            Format("000 sreg 111", Operand::MainSegmentRegister, Operand::None)
        }
    },
    {
        "POPA",
        std::vector<Format>
        {
            Format("01100001", Operand::None, Operand::None)
        }
    },
    {
        "XCHG",
        std::vector<Format>
        {
            Format("10010 reg", Operand::Register, Operand::RegisterAX),
            Format("1000011w mod reg r/m", Operand::Register | Operand::Memory, Operand::Register),
            Format("1000011w mod reg r/m", Operand::Register, Operand::Register | Operand::Memory),
        }
    },
    {
        "XLAT",
        std::vector<Format>
        {
            Format("11010111", Operand::None, Operand::None)
        }
    },
    {
        "MOVSX",
        std::vector<Format>
        {
            Format("000011111011111w mod reg r/m", Operand::Register, Operand::Register | Operand::Memory)
        }
    },
    {
        "MOVZX",
        std::vector<Format>
        {
            Format("000011111011011w mod reg r/m", Operand::Register, Operand::Register | Operand::Memory)
        }
    },
    {
        "IN",
        std::vector<Format>
        {
            Format("1110010w im", Operand::RegisterAX, Operand::Immediate),
            Format("1110110w", Operand::RegisterAX, Operand::RegisterDX)
        }
    },
    {
        "OUT",
        std::vector<Format>
        {
            Format("1110011w im", Operand::Immediate, Operand::RegisterAX),
            Format("1110111w", Operand::RegisterDX, Operand::RegisterAX)
        }
    },
    {
        "LEA",
        std::vector<Format>
        {
            Format("10001101 mod reg r/m", Operand::Register, Operand::Memory)
        }
    },
    {
        "LDS",
        std::vector<Format>
        {
            Format("11000101 mod reg r/m", Operand::Register, Operand::Memory)
        }
    },
    {
        "LES",
        std::vector<Format>
        {
            Format("11000100 mod reg r/m", Operand::Register, Operand::Memory)
        }
    },
    {
        "LFS",
        std::vector<Format>
        {
            Format("0000111110110100 mod reg r/m", Operand::Register, Operand::Memory)
        }
    },
    {
        "LGS",
        std::vector<Format>
        {
            Format("0000111110110101 mod reg r/m", Operand::Register, Operand::Memory)
        }
    },
    {
        "LSS",
        std::vector<Format>
        {
            Format("0000111110110010 mod reg r/m", Operand::Register, Operand::Memory)
        }
    }
};

std::unordered_map<std::string, std::vector<Format>> OperatorStore::LogicOperators = {
    {
        "NOT",
        std::vector<Format>
        {
            Format("1111011w mod 010 r/m", Operand::Register | Operand::Memory, Operand::None)
        }
    },
    {
        "AND",
        std::vector<Format>
        {
            Format("0010010w im", Operand::RegisterAX, Operand::Immediate),
            Format("001000dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0010000w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0010001w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 100 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "OR",
        std::vector<Format>
        {
            Format("0000110w im", Operand::RegisterAX, Operand::Immediate),
            Format("000010dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0000100w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0000101w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 001 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "XOR",
        std::vector<Format>
        {
            Format("0011010w im", Operand::RegisterAX, Operand::Immediate),
            Format("001100dw mod reg r/m", Operand::Register, Operand::Register),
            Format("0011000w mod reg r/m", Operand::Memory, Operand::Register),
            Format("0011001w mod reg r/m", Operand::Register, Operand::Memory),
            Format("100000sw mod 110 r/m im", Operand::Register | Operand::Memory, Operand::Immediate)
        }
    },
    {
        "TEST",
        std::vector<Format>
        {
            Format("1010100w im", Operand::RegisterAX, Operand::Immediate),
            Format("1000010w mod reg r/m", Operand::Register | Operand::Memory, Operand::Register),
            Format("1111011w mod 000 r/m im", Operand::Register | Operand::Memory, Operand::Immediate),
        }
    },
    {
        "SHL",
        std::vector<Format>
        {
            Format("1101000w mod 100 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 100 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 100 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    },
    {
        "SAL",
        std::vector<Format>
        {
            Format("1101000w mod 100 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 100 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 100 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    },
    {
        "SHR",
        std::vector<Format>
        {
            Format("1101000w mod 101 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 101 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 101 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    },
    {
        "SAR",
        std::vector<Format>
        {
            Format("1101000w mod 111 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 111 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 111 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    },
    {
        "ROL",
        std::vector<Format>
        {
            Format("1101000w mod 000 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 000 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 000 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    },
    {
        "ROR",
        std::vector<Format>
        {
            Format("1101000w mod 001 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 001 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 001 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    },
    {
        "RCL",
        std::vector<Format>
        {
            Format("1101000w mod 010 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 010 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 010 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    },
    {
        "RCR",
        std::vector<Format>
        {
            Format("1101000w mod 011 r/m", Operand::Register | Operand::Memory, Operand::None),
            Format("1101001w mod 011 r/m", Operand::Register | Operand::Memory, Operand::RegisterCL),
            Format("1100000w mod 011 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
        }
    }
};

std::unordered_map<std::string, std::vector<Format>> OperatorStore::BitOperators = {
    {
        "BT",
        std::vector<Format>
        {
            Format("0000111110111010 mod 100 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
            Format("0000111110100011 mod reg r/m", Operand::Register | Operand::Memory, Operand::Register)
        }
    },
    {
        "BTS",
        std::vector<Format>
        {
            Format("0000111110111010 mod 101 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
            Format("0000111110101011 mod reg r/m", Operand::Register | Operand::Memory, Operand::Register)
        }
    },
    {
        "BTR",
        std::vector<Format>
        {
            Format("0000111110111010 mod 110 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
            Format("0000111110110011 mod reg r/m", Operand::Register | Operand::Memory, Operand::Register)
        }
    },
    {
        "BTC",
        std::vector<Format>
        {
            Format("0000111110111010 mod 111 r/m im", Operand::Register | Operand::Memory, Operand::Immediate8),
            Format("0000111110111011 mod reg r/m", Operand::Register | Operand::Memory, Operand::Register)
        }
    },
    {
        "BTF",
        std::vector<Format>
        {
            Format("0000111110111100 mod reg r/m", Operand::Register, Operand::Register | Operand::Memory)
        }
    },
    {
        "BSR",
        std::vector<Format>
        {
            Format("0000111110111101 mod reg r/m", Operand::Register, Operand::Register | Operand::Memory)
        }
    },
};

std::unordered_map<std::string, std::vector<Format>> OperatorStore::ControlOperators = {
    {
        "JMP",
        std::vector<Format>
        {
            Format("11111111 mod 100 r/m", Operand::IndexRegister, Operand::None),
            Format("11101001", Operand::Relative16, Operand::None)
        }
    },
    {
        "CALL",
        std::vector<Format>
        {
            Format("11111111 mod 010 r/m", Operand::IndexRegister, Operand::None),
            Format("11101000", Operand::Relative16, Operand::None)
        }
    },
    {
        "RET",
        std::vector<Format>
        {
            Format("11000011", Operand::None, Operand::None),
            Format("11000010", Operand::Relative16, Operand::None)
        }
    },
};

std::unordered_map<std::string, std::vector<Format>> OperatorStore::ConditionOperators = {
    {
        "JO",
        std::vector<Format>
        {
            Format("0000111110000000", Operand::Relative16, Operand::None)
        }
    },
    {
        "JB",
        std::vector<Format>
        {
            Format("0000111110000010", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNAE",
        std::vector<Format>
        {
            Format("0000111110000010", Operand::Relative16, Operand::None)
        }
    },
    {
        "JE",
        std::vector<Format>
        {
            Format("0000111110000100", Operand::Relative16, Operand::None)
        }
    },
    {
        "JZ",
        std::vector<Format>
        {
            Format("0000111110000100", Operand::Relative16, Operand::None)
        }
    },
    {
        "JBE",
        std::vector<Format>
        {
            Format("0000111110000110", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNA",
        std::vector<Format>
        {
            Format("0000111110000110", Operand::Relative16, Operand::None)
        }
    },
    {
        "JS",
        std::vector<Format>
        {
            Format("0000111110001000", Operand::Relative16, Operand::None)
        }
    },
    {
        "JP",
        std::vector<Format>
        {
            Format("0000111110001010", Operand::Relative16, Operand::None)
        }
    },
    {
        "JPE",
        std::vector<Format>
        {
            Format("0000111110001010", Operand::Relative16, Operand::None)
        }
    },
    {
        "JL",
        std::vector<Format>
        {
            Format("0000111110001100", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNGE",
        std::vector<Format>
        {
            Format("0000111110001100", Operand::Relative16, Operand::None)
        }
    },
    {
        "JLE",
        std::vector<Format>
        {
            Format("0000111110001110", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNG",
        std::vector<Format>
        {
            Format("0000111110001110", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNO",
        std::vector<Format>
        {
            Format("0000111110000001", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNB",
        std::vector<Format>
        {
            Format("0000111110000011", Operand::Relative16, Operand::None)
        }
    },
    {
        "JAE",
        std::vector<Format>
        {
            Format("0000111110000011", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNE",
        std::vector<Format>
        {
            Format("0000111110000101", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNZ",
        std::vector<Format>
        {
            Format("0000111110000101", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNBE",
        std::vector<Format>
        {
            Format("0000111110000111", Operand::Relative16, Operand::None)
        }
    },
    {
        "JA",
        std::vector<Format>
        {
            Format("0000111110000111", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNS",
        std::vector<Format>
        {
            Format("0000111110001001", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNP",
        std::vector<Format>
        {
            Format("0000111110001011", Operand::Relative16, Operand::None)
        }
    },
    {
        "PO",
        std::vector<Format>
        {
            Format("0000111110001011", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNL",
        std::vector<Format>
        {
            Format("0000111110001101", Operand::Relative16, Operand::None)
        }
    },
    {
        "JGE",
        std::vector<Format>
        {
            Format("0000111110001101", Operand::Relative16, Operand::None)
        }
    },
    {
        "JNLE",
        std::vector<Format>
        {
            Format("0000111110001111", Operand::Relative16, Operand::None)
        }
    },
    {
        "JG",
        std::vector<Format>
        {
            Format("0000111110001111", Operand::Relative16, Operand::None)
        }
    },
    {
        "JCXZ",
        std::vector<Format>
        {
            Format("11100011", Operand::Relative8, Operand::None)
        }
    },
    {
        "JECXZ",
        std::vector<Format>
        {
            Format("11100011", Operand::Relative8, Operand::None)
        }
    },
    {
        "LOOP",
        std::vector<Format>
        {
            Format("11100010", Operand::Relative8, Operand::None)
        }
    },
    {
        "LOOPZ",
        std::vector<Format>
        {
            Format("11100001", Operand::Relative8, Operand::None)
        }
    },
    {
        "LOOPE",
        std::vector<Format>
        {
            Format("11100001", Operand::Relative8, Operand::None)
        }
    },
    {
        "LOOPNZ",
        std::vector<Format>
        {
            Format("11100000", Operand::Relative8, Operand::None)
        }
    },
    {
        "LOOPNE",
        std::vector<Format>
        {
            Format("11100000", Operand::Relative8, Operand::None)
        }
    }
};

std::unordered_map<std::string, std::vector<Format>> OperatorStore::InterruptsOperators = {
    {
        "INT",
        std::vector<Format>
        {
            Format("11001101 im", Operand::Immediate8, Operand::None)
        }
    },
    {
        "INT3",
        std::vector<Format>
        {
            Format("11001100", Operand::None, Operand::None)
        }
    },
    {
        "INTO",
        std::vector<Format>
        {
            Format("11001110", Operand::None, Operand::None)
        }
    },
    {
        "IRET",
        std::vector<Format>
        {
            Format("11001111", Operand::None, Operand::None)
        }
    },
    {
        "CLI",
        std::vector<Format>
        {
            Format("11111010", Operand::None, Operand::None)
        }
    },
    {
        "STI",
        std::vector<Format>
        {
            Format("11111011", Operand::None, Operand::None)
        }
    },
    {
        "LAHF",
        std::vector<Format>
        {
            Format("10011111", Operand::None, Operand::None)
        }
    },
    {
        "SAHF",
        std::vector<Format>
        {
            Format("10011110", Operand::None, Operand::None)
        }
    },
    {
        "PUSHF",
        std::vector<Format>
        {
            Format("10011100", Operand::None, Operand::None)
        }
    },
    {
        "POPF",
        std::vector<Format>
        {
            Format("10011101", Operand::None, Operand::None)
        }
    },
    {
        "CLC",
        std::vector<Format>
        {
            Format("11111000", Operand::None, Operand::None)
        }
    },
    {
        "CLD",
        std::vector<Format>
        {
            Format("11111100", Operand::None, Operand::None)
        }
    },
    {
        "CMC",
        std::vector<Format>
        {
            Format("11110101", Operand::None, Operand::None)
        }
    },
    {
        "STC",
        std::vector<Format>
        {
            Format("11111001", Operand::None, Operand::None)
        }
    },
    {
        "STD",
        std::vector<Format>
        {
            Format("11111101", Operand::None, Operand::None)
        }
    }
};
