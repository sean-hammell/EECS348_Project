#include "calculator.h"
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>


enum TokenTypes {
	Base,
	Digit,
	UnaryOperator,
	BinaryOperator,
	LeftParen,
	RightParen
};

void validate(const std::string& equation) {
	int parens = 0;
	int operators_to_operands = 0;
	int token = Base;
	for (int i = 0; i < equation.length(); i++) {

		// check if character is invalid
		if (!isOperator(equation[i]) && !isValidParenthesis(equation[i]) && !isDigit(equation[i])) {
			std::string message = "Invalid Character '";
			message += equation[i];
			message += "'";
			throw std::invalid_argument(message);
		}

		if (isDigit(equation[i]) && token != Digit) {
			// the character is the start of a new number
			token = Digit;
			operators_to_operands++;
		} else if (isBinaryOperator(equation[i])) {
			// if the character is not a '-' or '+' preceeded by a digit, right parenthesis, or unary operator
			if ((equation[i] != '-' && equation[i] != '+') || token == Digit || token == RightParen || token == UnaryOperator) {
				// the character is a binary operator
				token = BinaryOperator;
				operators_to_operands--;
			} else {
				// the character is a unary operator
				token = UnaryOperator;
			}
		} else if (isValidParenthesis(equation[i])) {
			// the character is a parenthesis
			if (equation[i] == '(') {
				token = LeftParen;
				++parens;
			} else {
				token = RightParen;
				--parens;
			}
		}

		// Left Parenthesis Proofing
		if (token == LeftParen) {
			// left side
			if (i > 0) {
				if (isDigit(equation[i - 1]) || equation[i - 1] == ')') {
					// invalid if the left parenthesis is preceded by a digit or right parenthesis
					throw std::invalid_argument("Improper Parenthesis Usage");
				}
			}

			// right side
			if (i < equation.length() - 1) {
				if (isOperator(equation[i + 1]) || equation[i + 1] == ')') {
					if ((equation[i + 1] == '-' || equation[i + 1] == '+') && i < equation.length() - 2) {
						if (!(isDigit(equation[i + 2]) || isValidParenthesis(equation[i + 2]))) {
							// invalid if the left parenthesis is succeeded by an invalid unary operator
							throw std::invalid_argument("Improper Parenthesis Usage");
						}
					} else {
						// invalid if the left parenthesis is succeeded by a binary operator or right parenthesis
						throw std::invalid_argument("Improper Parenthesis Usage");
					}
				}
			}
		}

		// Right Parenthesis Proofing
		if (token == RightParen) {
			// left side
			if (i > 0) {
				if (isOperator(equation[i - 1]) || equation[i - 1] == '(') {
					// invalid if the right parenthesis is preceeded by an operator or left parenthesis
					throw std::invalid_argument("Improper Parenthesis Usage");
				}
			}

			// right side
			if (i < equation.length() - 1) {
				if (isDigit(equation[i + 1]) || equation[i + 1] == '(') {
					// invalid if the right parenthesis is succeeded by a digit or left parenthesis
					throw std::invalid_argument("Improper Parenthesis Usage");
				}
			}
		}
	}

	if (parens != 0) {
		throw std::invalid_argument("Unbalanced Parentheses");
	}

	if (operators_to_operands > 1) {
		throw std::invalid_argument("Too Many Operands");
	}
	
	if (operators_to_operands < 1) {
		throw std::invalid_argument("Too Many Operators");
	}
}

int main() {
	std::cout << "Welcome to the calculator (type \"quit\" to exit)" << std::endl;

	// Continuously prompt the user for an equation
	while (1) {
		
		// prompt the user and store the equation they give
		std::cout << "Enter an equation: ";
		std::string equation;
		std::getline(std::cin, equation);
		
		if (equation.empty()) continue;
		
		// remove the whitespaces from the equation string
		int count = 0;
		for (int i = 0; equation[i]; i++) {
			if (equation[i] != ' ' && equation[i] != '\t') equation[count++] = equation[i];
		}
		equation.resize(count);
		
		// if the user enters "quit" then break out of the loop, and then the program exits
		if (strcmp(equation.c_str(), "quit") == 0) break;

		try {
			validate(equation);
		} catch (const std::invalid_argument& error) {
			std::cout << error.what() << std::endl;
			continue;
		}
		
		// print the calculation
		double result;
		try {
			result = calculate(equation);
			std::cout << result << std::endl;
		} catch (const std::domain_error& error) {
			std::cout << error.what() << std::endl;
		}
	}

	std::cout << "Exiting..." << std::endl;
	return 0;
}
