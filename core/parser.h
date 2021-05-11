#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <array>
#include <deque>
#include <unordered_map>

#include "types.h"
#include "instruction.h"

class Parser
{
public:
    Parser() = delete;
    Parser(const std::vector<std::string> &data);

    struct BinaryData
    {
        BinaryData(u32 index, u32 address, const std::vector<u8>& data);

        u32 index;
        u32 address;
        std::vector<u8> data;
    };

    void Parse();
    void Compile();

    std::vector<BinaryData> GetBinaryData() const;
    std::vector<std::pair<u32, std::string>> GetErrors() const;

private:
    std::vector<std::string> data;
    std::vector<BinaryData> resultData;
    std::vector<std::pair<u32, std::shared_ptr<Instruction>>> instructions;
    std::vector<std::pair<u32, std::string>> errors;
    std::unordered_map<std::string, u32> SYMTAB;
    u32 LOCCTR;

    std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
    std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
    std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
    std::deque<std::string> Split(std::string str, const std::string& delim);
    std::vector<std::string> Tokenize(std::string row);
};

#endif // PARSER_H
