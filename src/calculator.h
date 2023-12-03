#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <string>
#include <vector>

class Calculator {
public:
    Calculator() = default;

    Calculator(const Calculator&) = delete;
    Calculator& operator=(const Calculator&) = delete;

    ~Calculator() = default;

    double Calculate(const std::string& equation);

    static bool IsDigit(const char c);
    static bool IsUnaryOperator(const char c);
    static bool IsBinaryOperator(const char c);
    static bool IsOperator(const char c);
    static bool IsValidParenthesis(const char c);

private:
    void Tokenize(const std::string& equation, std::vector<std::string> *infix_tokens);
    void InfixToPostfix(std::vector<std::string> *infix_tokens, std::vector<std::string> *postfix_tokens);
    double Evaluate(std::vector<std::string> *postfix_tokens);

    int OperatorPrecedence(const char op);
    double ComputeUnaryOperation(const double rhs, const char op);
    double ComputeBinaryOperation(const double lhs, const double rhs, const char op);
};

#endif  // __CALCULATOR_H__
