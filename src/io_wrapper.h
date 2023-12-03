#ifndef __IO_WRAPPER_H__
#define __IO_WRAPPER_H__

#include <string>

class IOWrapper {
public:
    enum TokenTypes {
        Base,
        Digit,
        UnaryOperator,
        BinaryOperator,
        LeftParen,
        RightParen
    };

    IOWrapper() = default;

    IOWrapper(const IOWrapper&) = delete;
    IOWrapper& operator=(const IOWrapper&) = delete;

    ~IOWrapper() = default;

    std::string Prompt();
    void Validate(const std::string& equation);
};

#endif  // __IO_WRAPPER_H__
