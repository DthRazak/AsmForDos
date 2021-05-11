#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <sstream>

class InstructionFormatException : public std::exception
{
public:
    InstructionFormatException(
        const std::string& instruction,
        const std::string& op1,
        const std::string& op2
    ) {
        std::ostringstream oss;
        oss << instruction << ' ' << op1 << ' ' << op2
            << " - Instruction format doesn't match to these operands";
        _msg = oss.str();
    };

    virtual const char* what() const throw()
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

class DirectiveFormatException : public std::exception
{
public:
    DirectiveFormatException(
        const std::string& directive
    ) {
        std::ostringstream oss;
        oss << directive << " - Directive format doesn't recognized";
        _msg = oss.str();
    };

    virtual const char* what() const throw()
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

class InvalidOperandException : public std::exception
{
public:
    InvalidOperandException(
        const std::string& error_message,
        const std::string& op1,
        const std::string& op2
    ) {
        std::ostringstream oss;
        oss << "Operand 1: " << op1 << ", Operand 2: " << op2
            << " - " << error_message;
        _msg = oss.str();
    };

    virtual const char* what() const throw()
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

class AddressResolveException : public std::exception
{
public:
    AddressResolveException(
        const std::string& op
    ) {
        std::ostringstream oss;
        oss << op << " - Can't resolve this address";
        _msg = oss.str();
    };

    virtual const char* what() const throw()
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

class ParseException : public std::exception
{
public:
    ParseException(
        const std::string& msg
    ) : _msg(msg) {};

    virtual const char* what() const throw()
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

#endif // EXCEPTIONS_H
