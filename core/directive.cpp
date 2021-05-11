#include <algorithm>

#include "directive.h"
#include "exceptions.h"
#include "converter.h"
#include "parser.h"


Directive::Directive(const std::string& name, const std::string& instruction)
    : name(name), instruction(instruction)
{}

std::shared_ptr<Instruction> Directive::Create(
    const std::vector<std::string>& tokens
)
{
    if (tokens[0] == "ORG") {
        if (tokens.size() == 2 && Converter::isNumber(tokens[1])) {
            u16 digit;
            if (Converter::TryGetNumber(tokens[1], digit)) {
                auto directive = std::shared_ptr<Directive>(new Directive("", "ORG"));
                directive->values.push_back(digit);

                return directive;
            }
            else {
                throw ParseException("Can't parse number");
            }
        }
        else {
            throw DirectiveFormatException(tokens[0]);
        }
    }

    if (tokens.size() > 1 && ((tokens[1] == "DB") || (tokens[1] == "DW"))){
        auto directive = std::shared_ptr<Directive>(new Directive(tokens[0], tokens[1]));
        u16 digit;
        for (i32 i = 2; i < tokens.size(); ++i) {
            if (tokens[i] == "?") {
                directive->values.push_back(0);
            }
            else if (Converter::isConstant(tokens[i])) {
                directive->values.push_back(tokens[i][1]);
            }
            else if (tokens[1] == "DB" && Converter::isString(tokens[i])) {
                for (i32 j = 1; j < tokens[i].size() - 1; ++j) {
                    directive->values.push_back(tokens[i][j]);
                }
            }
            else if (tokens[1] == "DW" && Converter::isString(tokens[i]) && tokens[i].size() == 4) {
                directive->values.push_back(
                    ((u16)tokens[i][1] << 8) | tokens[i][2]
                );
            }
            else if (Converter::TryGetNumber(tokens[i], digit)) {
                directive->values.push_back(digit);
            }
            else {
                throw ParseException("Can't parse data");
            }
        }
        if (directive->values.empty()) {
            throw DirectiveFormatException(tokens[1]);
        }

        return directive;
    }
    else if (tokens.size() > 1 && ((tokens[0] == "DB") || (tokens[0] == "DW"))) {
        auto directive = std::shared_ptr<Directive>(new Directive("", tokens[0]));
        u16 digit;
        for (i32 i = 1; i < tokens.size(); ++i) {
            if (tokens[i] == "?") {
                directive->values.push_back(0);
            }
            else if (Converter::isConstant(tokens[i])) {
                directive->values.push_back(tokens[i][1]);
            }
            else if (tokens[0] == "DB" && Converter::isString(tokens[i])) {
                for (i32 j = 1; j < tokens[i].size() - 1; ++j) {
                    directive->values.push_back(tokens[i][j]);
                }
            }
            else if (tokens[0] == "DW" && Converter::isString(tokens[i]) && tokens[i].size() == 4) {
                directive->values.push_back(
                    ((u16)tokens[i][1] << 8) | tokens[i][2]
                );
            }
            else if (Converter::TryGetNumber(tokens[i], digit)) {
                directive->values.push_back(digit);
            }
            else {
                throw ParseException("Can't parse data");
            }
        }
        if (directive->values.empty()) {
            throw DirectiveFormatException(tokens[1]);
        }

        return directive;
    }

    return nullptr;
}

std::vector<u8> Directive::GetBinaryData(
    std::unordered_map<std::string, u32>& SYMTAB,
    u32& LOCCTR,
    bool isFirstIteration
)
{
    if (isFirstIteration) {
        if (instruction == "ORG") {
            LOCCTR = values[0];
        }
        else if (instruction == "DB") {
            std::vector<u8> res;
            std::for_each(values.begin(), values.end(), [&res](u16 num) {
                res.push_back(num);
            });

            if (!name.empty())
                SYMTAB.insert(std::make_pair(name, LOCCTR));
            LOCCTR += res.size();

            return std::move(res);
        }
        else if (instruction == "DW") {
            std::vector<u8> res;
            std::for_each(values.begin(), values.end(), [&res](u16 num) {
                auto num_str = Converter::ToBinary(num, 2);
                res.push_back(Converter::ToByte(num_str.substr(0, 8)));
                res.push_back(Converter::ToByte(num_str.substr(8, 8)));
            });

            if (!name.empty())
                SYMTAB.insert(std::make_pair(name, LOCCTR));
            LOCCTR += res.size();

            return std::move(res);
        }
    }

    return std::vector<u8>();
}

std::ostream& operator<<(std::ostream& os, const Directive* directive){
    os << "Directive - Name: " << directive->name
        << " Instruction: " << directive->instruction << std::endl;
    return os;
}
