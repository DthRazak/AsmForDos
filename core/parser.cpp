#include "parser.h"
#include "directive.h"
#include "operator.h"
#include "exceptions.h"

#include <algorithm>
#include <bitset>
#include <cstring>

Parser::Parser(const std::vector<std::string>& data)
    :data(data), LOCCTR(0)
{
}

Parser::BinaryData::BinaryData(u32 index, u32 address, const std::vector<u8>& data)
    : index(index), address(address), data(data)
{}


void Parser::Parse() {
    for (u32 i = 0; i < data.size(); ++i) {
        auto tokens = Tokenize(data[i]);
        if (tokens.empty()) continue;
        try {
            auto result = Directive::Create(tokens);

            if (result != nullptr) {
                instructions.push_back(std::make_pair(i, result));
            }
            else {
                result = Operator::Create(tokens);

                if (result != nullptr) {
                    instructions.push_back(std::make_pair(i, result));
                }
                else {
                    errors.push_back(std::make_pair(i, "Row is not recognized"));
                    //std::cout << "Not found" << std::endl;
                }
            }
        }
        catch (const std::exception& ex) {
            errors.push_back(std::make_pair(i, ex.what()));
            //std::cout << ex.what() << std::endl;
        }
    }
}

void Parser::Compile() {
    for (size_t i = 0; i < instructions.size(); ++i) {
        try {
            u32 address = LOCCTR;
            resultData.push_back(
                Parser::BinaryData(
                    instructions[i].first,
                    address,
                    std::dynamic_pointer_cast<Instruction>(instructions[i].second)->
                        GetBinaryData(SYMTAB, LOCCTR, true)
                )
            );
        }
        catch (const std::exception& ex) {
            errors.push_back(std::make_pair(instructions[i].first, ex.what()));
            std::cout << ex.what() << std::endl;
        }
    }
    for (size_t i = 0; i < resultData.size(); ++i) {
        if (resultData[i].data.empty()) {
            try {
                resultData[i] = Parser::BinaryData(
                    resultData[i].index,
                    resultData[i].address,
                    std::dynamic_pointer_cast<Instruction>(instructions[i].second)->
                        GetBinaryData(SYMTAB, resultData[i].address, false)
                );
            }
            catch (const std::exception& ex) {
                errors.push_back(std::make_pair(instructions[i].first, ex.what()));
                std::cout << ex.what() << std::endl;
            }
        }
    }
}

std::vector<Parser::BinaryData> Parser::GetBinaryData() const
{
    return resultData;
}

std::vector<std::pair<u32, std::string> > Parser::GetErrors() const
{
    return errors;
}

std::string& Parser::ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& Parser::rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& Parser::trim(std::string& str, const std::string& chars) {
    return ltrim(rtrim(str, chars), chars);
}

std::deque<std::string> Parser::Split(std::string str, const std::string& delim) {
    std::deque<std::string> tokens;
    char* token = std::strtok(str.data(), delim.c_str());
    while (token != NULL) {
        std::string tk = token;
        if (tk.size() > 0 && tk.front() == ' ' && std::equal(tk.begin() + 1, tk.end(), tk.begin())){
            tokens.push_back(tk);
        }else{
            tokens.push_back(trim(tk));
        }
        token = std::strtok(NULL, delim.c_str());
    }
    return tokens;
}

std::vector<std::string> Parser::Tokenize(std::string row) {
    std::vector<std::string> result;
    row = trim(row);
    if (!row.empty() && row[0] == ';')
        return std::move(result);

    std::deque<std::string> quoteTokens = Split(row, "\"");

    for (u32 i = 0; i < quoteTokens.size(); i++) {
        if (i % 2 == 0) {
            std::string currToken = quoteTokens[i];
            std::transform(currToken.begin(), currToken.end(), currToken.begin(), ::toupper);

            size_t semiColPos = currToken.find_first_of(';');
            if (semiColPos != std::string::npos) {
                currToken.resize(semiColPos);
            }
            std::deque<std::string> comaTokens = Split(currToken, ",");

            if (!comaTokens.empty()) {
                std::string first = comaTokens.front();
                comaTokens.pop_front();

                std::deque<std::string> spaceTokens = Split(first, " \t");
                std::copy(spaceTokens.begin(), spaceTokens.end(), std::back_inserter(result));
            }
            std::copy(comaTokens.begin(), comaTokens.end(), std::back_inserter(result));
        }
        else {
            result.push_back('"' + quoteTokens[i] + '"');
        }
    }

    return std::move(result);
}
