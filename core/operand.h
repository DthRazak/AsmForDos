#ifndef OPERAND_H
#define OPERAND_H

#include <memory>
#include <unordered_map>

#include "Types.h"

class Operand
{
public:
    enum Type : u16
    {
        Register = 0x1,  // R
        Memory = 0x2, // M
        Immediate = 0x4,  // im
        SegmentRegister = 0x8,
        RegisterCX = 0x10,
        RegisterAX = 0x20, // A
        None = 0x40,
        MainSegmentRegister = 0x80,
        ExtendedSegmentRegister = 0x100,
        IndexRegister = 0x200,
        RegisterDX = 0x400,
        RegisterCL = 0x800,
        Immediate8 = 0x1000,
        Relative8 = 0x2000,
        Relative16 = 0x4000,
    };

    Operand(const std::string& name);

    u32 GetIm() const;
    u16 GetType() const;
    u8 GetMod() const;
    u8 GetBytesCount() const;
    std::string GetValue() const;
    std::string GetName() const;

    friend std::ostream& operator<<(std::ostream& os, const Operand& op);

protected:
    u16 type;
    u8  mod, bytes;
    u32 im;
    std::string name, value;
};

class Constant : public Operand
{
public:
    static std::shared_ptr<Operand> TryToCreate(const std::string& op);

private:
    Constant(const std::string& name, u32 num, u8 bytes);
};

class Register : public Operand
{
public:
    static std::shared_ptr<Operand> TryToCreate(const std::string& op);

private:
    Register(const std::string& name, OperationRegInfo info);
    Register(const std::string& name, SegmentRegInfo info);
};

class Memory : public Operand
{
public:
    static std::shared_ptr<Operand> TryToCreate(const std::string& op);

    bool TrySetAddress(const std::unordered_map<std::string, u32>& SYMTAB);

private:
    bool hasValue;
    std::string var;

    Memory(const std::string& name, u8 mod, const std::string& rm, u16 disp);
    Memory(const std::string& name, u8 mod, const std::string& rm, const std::string &var);
};
#endif // OPERAND_H
