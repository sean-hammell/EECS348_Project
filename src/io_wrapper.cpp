#include "src/io_wrapper.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "src/calculator.h"

std::string IOWrapper::Prompt() {
  // Prompt the user to enter an equation and store it.
  std::cout << "\nEnter an expression: ";
  std::string equation;
  std::getline(std::cin, equation);

  // Remove any whitespace from the equation string.
  int count = 0;
  for (int i = 0; equation[i]; ++i) {
    if (equation[i] != ' ' && equation[i] != '\t') {
      equation[count] = equation[i];
      ++count;
    }
  }
  equation.resize(count);

  return equation;
}

void IOWrapper::Validate(const std::string& equation) {
  int parens = 0;
	int operators_to_operands = 0;
	int token = Base;
	for (int i = 0; i < equation.length(); i++) {

		// Check if the equation contains invalid characters.
		if (!(Calculator::IsOperator(equation[i]) || Calculator::IsValidParenthesis(equation[i]) || Calculator::IsDigit(equation[i]))) {
			std::string message = "Invalid Character '";
			message += equation[i];
			message += "'";
			throw std::invalid_argument(message);
		}

		if (Calculator::IsDigit(equation[i]) && token != Digit) {
			// If the character is a digit and the previous character was not a digit, it is the start of
			// a new number.
			token = Digit;
			operators_to_operands++;
		} else if (Calculator::IsBinaryOperator(equation[i])) {
			// If the character is not a '+' or '-' or the character is a '+' or '-' preceded by a digit
			// right parenthesis, or unary operator, then it is a binary operator. Otherwise, it is an
			// unary operator.
			if ((equation[i] != '-' && equation[i] != '+') || token == Digit || token == RightParen || token == UnaryOperator) {
				token = BinaryOperator;
				operators_to_operands--;
			} else {
				token = UnaryOperator;
			}
		} else if (Calculator::IsValidParenthesis(equation[i])) {
			if (equation[i] == '(') {
				token = LeftParen;
				++parens;
			} else {
				token = RightParen;
				--parens;
			}
		}

		// Left parenthesis proofing.
		if (token == LeftParen) {
			// Check the left side of the parenthesis.
			if (i > 0) {
				if (Calculator::IsDigit(equation[i - 1]) || equation[i - 1] == ')') {
					// The parenthesis is invalid if it is preceded by a digit or right parenthesis.
					throw std::invalid_argument("Improper Parenthesis Usage");
				}
			}

			// Check the right side of the parenthesis.
			if (i < equation.length() - 1) {
				if (Calculator::IsOperator(equation[i + 1]) || equation[i + 1] == ')') {
					if ((equation[i + 1] == '-' || equation[i + 1] == '+') && i < equation.length() - 2) {
						if (!(Calculator::IsDigit(equation[i + 2]) || Calculator::IsValidParenthesis(equation[i + 2]))) {
							// The parenthesis is invalid if it is succeeded by an invalid unary operator.
							throw std::invalid_argument("Improper Parenthesis Usage");
						}
					} else {
						// The parenthesis is invalid if it is succeeded by a binary operator or right
						// parenthesis.
						throw std::invalid_argument("Improper Parenthesis Usage");
					}
				}
			}
		}

		// Right parenthesis proofing
		if (token == RightParen) {
			// Check the left side of the parenthesis.
			if (i > 0) {
				if (Calculator::IsOperator(equation[i - 1]) || equation[i - 1] == '(') {
					// The parenthesis is invalid if it is preceded by an operator or left parenthesis.
					throw std::invalid_argument("Improper Parenthesis Usage");
				}
			}

			// Check the right side of the parenthesis.
			if (i < equation.length() - 1) {
				if (Calculator::IsDigit(equation[i + 1]) || equation[i + 1] == '(') {
						// The parenthesis is invalid if it is succeeded by a digit or left parenthesis.
					throw std::invalid_argument("Improper Parenthesis Usage");
				}
			}
		}
	}

	if (parens != 0) {
		// The parentheses an unbalanced if there is an unequal number of left and right parentheses.
		throw std::invalid_argument("Unbalanced Parentheses");
	}

	if (operators_to_operands > 1) {
		// There are too many operands if the number of operands to operators is greater than one.
		throw std::invalid_argument("Too Many Operands");
	}
	
	if (operators_to_operands < 1) {
		// There are too many operators if the number of operands to operators is less than one.
		throw std::invalid_argument("Too Many Operators");
	}
}
