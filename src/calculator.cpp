#include "src/calculator.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/stack.h"

double Calculator::Calculate(const std::string& equation) {
  std::vector<std::string> infix_tokens;
  std::vector<std::string> postfix_tokens;

  Tokenize(equation, &infix_tokens);
  InfixToPostfix(&infix_tokens, &postfix_tokens);
  return Evaluate(&postfix_tokens);
}

bool Calculator::IsDigit(const char c) {
  return ((c >= '0' && c <= '9') || c == '.');
}

bool Calculator::IsUnaryOperator(const char c) {
  switch(c) {
		case 'p':
			return true;
		case 'n':
			return true;
		default:
			return false;
	}
}

bool Calculator::IsBinaryOperator(const char c) {
  switch(c) {
		case '+':
			return true;
		case '-':
			return true;
		case '*':
			return true;
		case '/':
			return true;
		case '^':
			return true;
		case '%':
			return true;
		default:
			return false;
	}
}

bool Calculator::IsOperator(const char c) {
  return (IsBinaryOperator(c) || IsUnaryOperator(c));
}

bool Calculator::IsValidParenthesis(const char c) {
  switch(c) {
		case '(':
			return true;
		case ')':
			return true;
		default:
			return false;
	}
}

void Calculator::Tokenize(const std::string& equation, std::vector<std::string> *infix_tokens) {
	// Used to store the digits of any numbers in the equation.
	std::vector<char> value;

	for (int i = 0; i < equation.length(); i++) {
		char c = equation[i];

		if (IsDigit(c)) {
			// If the variable is a digit, push it to the value vector.
			value.push_back(c);
		} else {
			// If the value vector isn't empty and the current character isn't a digit, add the value to
			// the infix tokens and clear the value.
			if (!value.empty()) {
				char n[value.size() + 1];
				for (int i = 0; i < value.size(); ++i) {
 					n[i] = value[i];
				}
				n[value.size()] = '\0';
				infix_tokens->push_back(n);
				value.clear();
			}

			if (IsOperator(c)) {
				// If the character is an operator, determine if it is a unary operator or binary operator,
				// then add it to the infix tokens.
				if (i > 0) {
					char prev = equation[i - 1];
					if ((IsOperator(prev) || prev == '(') && (c == '-' || c == '+')) {
						infix_tokens->push_back(c == '+' ? "p" : "n");
					} else {
						std::string op;
						op = c;
						infix_tokens->push_back(op);
					}
				} else if (c == '-' || c == '+') {
					infix_tokens->push_back(c == '+' ? "p" : "n");
				}
			} else if (IsValidParenthesis(c)) {
				// If the character is a parenthesis, add it to the infix tokens.
				std::string paren;
				paren = c;
				infix_tokens->push_back(paren);
			}
		}
	}

	// Add any remaining value to the infix tokens.
	if (!value.empty()) {
		char n[value.size() + 1];
		for (int i = 0; i < value.size(); ++i) {
			n[i] = value[i];
		}
		n[value.size()] = '\0';
		infix_tokens->push_back(n);
		value.clear();
	}
}

void Calculator::InfixToPostfix(std::vector<std::string> *infix_tokens, std::vector<std::string> *postfix_tokens) {
	Stack<std::string> operator_stack;

	for (int i = 0; i < infix_tokens->size(); i++) {
		std::string token = (*infix_tokens)[i];

		if (token[0] == '(') {
			// If the token is an left parenthesis, add it to the operator stack.
			operator_stack.Push(token);
		} else if (token[0] == ')') {
			// If the token is a right parenthesis, pop every operator until we hit a left parenthesis.
			while (operator_stack.Peek()[0] != '(') {
				postfix_tokens->push_back(operator_stack.Peek());
				operator_stack.Pop();
			}

			operator_stack.Pop();
		} else if (IsOperator(token[0])) {
			// If the token is an operator.
			if (operator_stack.Empty()) {
				// If the operator stack is empty, push the operator.
				operator_stack.Push(token);
			} else {
				// Pop every operator on the stack until we hit a parenthesis or an operator of higher precedence.
				while (operator_stack.Peek()[0] != '(' && OperatorPrecedence(operator_stack.Peek()[0]) <= OperatorPrecedence(token[0])) {
					postfix_tokens->push_back(operator_stack.Peek());
					operator_stack.Pop();
					
					if (operator_stack.Empty()) {
						break;
					}
				}

				operator_stack.Push(token);
			}
		} else {
			// If the token is not a parenthesis or an operator, it must be a number. Add it to the
			// postfix tokens immediately.
			postfix_tokens->push_back(token);
		}
	}

	// Add anything left in the operator stack to the postfix tokens.
	while (!operator_stack.Empty()) {
		postfix_tokens->push_back(operator_stack.Peek());
		operator_stack.Pop();
	}
}

double Calculator::Evaluate(std::vector<std::string> *postfix_tokens) {
	Stack<double> operand_stack;
	
	for (int i = 0; i < postfix_tokens->size(); i++) {
		std::string token = (*postfix_tokens)[i];

		if (IsOperator(token[0])) {
			if(IsBinaryOperator(token[0])) {
				// If the token is a binary operator, pop the top two values off the operand stack and push
				// the result back to the stack.
				double rhs = operand_stack.Peek();
				operand_stack.Pop();
				double lhs = operand_stack.Peek();
				operand_stack.Pop();
				
				operand_stack.Push(ComputeBinaryOperation(lhs, rhs, token[0]));
			} else if (IsUnaryOperator(token[0])) {
				// If the token is an unary operator, pop the top value off the operand stack and push the
				// result back to the stack.
				double rhs = operand_stack.Peek();
				operand_stack.Pop();
				
				operand_stack.Push(ComputeUnaryOperation(rhs, token[0]));
			}
		} else {
			// If the token isn't an operator, push the value to the operand stack.
 			operand_stack.Push(std::stod(token));
		}
	}

	// Store the final value, empty the operand stack, and return the value
	double value = operand_stack.Peek();
	while (!operand_stack.Empty()) {
		operand_stack.Pop();
	}

	return value;
}

int Calculator::OperatorPrecedence(const char op) {
	switch(op) {
		case '+':
			return 3;
		case '-':
			return 3;
		case '*':
			return 2;
		case '/':
			return 2;
		case '%':
			return 2;
		case '^':
			return 1;
		case 'n':
			return 1;
		case 'p':
			return 1;
		default:
			return -1;
	}
}

double Calculator::ComputeUnaryOperation(const double rhs, const char op) {
	switch (op) {
		case 'n':
			return rhs * -1;
		case 'p':
			return rhs;
		default:
			std::cout << "Error: unknown operator: " << op << std::endl;
			return 0.0;
	}
}

double Calculator::ComputeBinaryOperation(const double lhs, const double rhs, const char op) {
  switch(op) {
		case '+':
			return lhs + rhs;
		case '-':
			return lhs - rhs;
		case '*':
			return lhs * rhs;
		case '/':
			if (rhs == 0) {
				throw std::domain_error("Error: Division by Zero");
			}
			return lhs / rhs;
		case '%':
			if (rhs == 0) {
				throw std::domain_error("Error: Division by Zero");
			}
			return std::fmod(lhs, rhs);
		case '^':
			return pow(lhs, rhs);
		default:
			std::cout << "Error: unknown operator: " << op << std::endl;
			return 0.0;
  }
}
