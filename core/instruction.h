#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "types.h"

class Instruction
{
public:
    virtual ~Instruction() = default;

    virtual std::vector<u8> GetBinaryData(
        std::unordered_map<std::string, u32>& SYMTAB,
        u32 &LOCCTR,
        bool isFirstIteration
    ) = 0;
};

#endif // INSTRUCTION_H
