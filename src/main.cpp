#include "calculator.h"
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

/*
	TO DO:
		OPERATOR PROOFING
			fix so cant do double operators
			check if there is an operator at the beginning of the string
			if the string starts with a negative, check if the next character is an operator
*/

int main() {
	std::cout << "Welcome to the calculator (type \"quit\" to exit)" << std::endl;
	
	/*
		Continuously prompt the user for an equation
	*/
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
		
		/*
			Check if the string:
				has an invalid character in it.
				has imbalanced parentheses
			
			cont = continue, denoting that the loop should continue to the next iteration
			
			when cont =
				1: invalid character
				2: imbalanced parentheses
				3: improper use of parentheses (e.g. 5(2), (2)5, 5(*2))
		*/
		int cont = 0;
		int i;
		int parens = 0;
		int counter = 0;
		enum Tokens {
			whitespace,
			digit,
			opt,
			lparen,
			rparen
		};
		int current = whitespace;
		for (i = 0; i < equation.length(); i++) {
			
			// check if character is invalid
			if (!isOperator(equation[i]) && !isValidParenthesis(equation[i]) && !isDigit(equation[i])) {
				cont = 1;
				break;
			}
			
			if (isDigit(equation[i]) && current != digit) {
				current = digit;
				counter++;
			}
			else if (isBinaryOperator(equation[i])) {
				if ((equation[i] != '-' && equation[i] != '+') || current == digit || current == rparen) {
					current = opt;
					counter--;
				}
			}
			else if (isValidParenthesis(equation[i])) {
				current = equation[i] == '(' ? lparen : rparen;
			}

			// count a parenthesis
			switch (equation[i]) {
				case '(':
					parens++;
					break;
				case ')':
					parens--;
					break;
				default:
					break;
			}
			
			/*
				If we've reached the end of the string and parens is not 0, then the parentheses are imbalanced
				OR if the number of parentheses is ever negative
			*/
			if (parens < 0) {
				cont = 2;
				break;
			}
			
			/*
				Parentheses proofing. Errors occur when:
					If an L parenthesis has a number directly outside it
					If an L parenthesis has an operator directly inside it
						Have to check if a '-' is a negative or a minus
					If an R parenthesis has a number directly outside it
					If an R parenthesis has an operator directly inside it
			*/
			if (isValidParenthesis(equation[i])) {
				// check the left of the parenthesis
				if (i - 1 >= 0) {
					
					// check outside the L paren (could be operator or another L paren)
					if (equation[i] == '(') {
						if (isDigit(equation[i-1]) || equation[i-1] == ')') {
							cont = 3;
							break;
						}
					}
					
					// check inside the R paren (could be digit or another R paren)
					else {
						if (isOperator(equation[i-1]) || equation[i-1] == '(') {
							cont = 3;
							break;
						}
					}
				}
				
				// check the right of the parenthesis
				if (i + 1 < equation.length()) {
					
					// check insisde the L paren (could be digit, another L paren, or a negative sign)
					if (equation[i] == '(') {
						
						// if the first character inside is an operator or R paren
						if (isOperator(equation[i+1]) || equation[i+1] == ')') {
							
							// if the operator is a minus sign and we can access the index to the right of the minus
							if ((equation[i+1] == '-' || equation[i+1] == '+') && i + 2 < equation.length()) {
								
								// if the character to the right of the operator is not a digit, than it is an invalid use of the parenthesis
								if (!isDigit(equation[i+2])) {
									cont = 3;
									break;
								}
							}
							
							// return error 3 as there can't be an operator or R paren directly inside an L paren
							else {
								cont = 3;
								break;
							}
						}
					}
					
					// check outside the R paren (could be operator or another R paren)
					else {
						if (isDigit(equation[i+1]) || equation[i+1] == '(') {
							cont = 3;
							break;
						}
					}
				}
			}
			// end parenthesis proofing
			
			// operator proofing
			
		}
		
		if (!cont) {
			if (counter > 1) cont = 4;
			else if (counter < 1) cont = 5;
			else if (parens != 0) cont = 2;
		}
		
		// print an error message depending on the type of error encountered
		switch (cont) {
			case 1:
				std::cout << "Error: Invalid character recognized: " << equation[i] << std::endl;
				break;
			case 2:
				std::cout << "Error: Parentheses not properly balanced" << std::endl;
				break;
			case 3:
				std::cout << "Error: Improper parenthesis usage" << std::endl;
				break;
			case 4:
				std::cout << "Error: Too many operands" << std::endl;
				break;
			case 5:
				std::cout << "Error: Not enough operands" << std::endl;
				break;
		}
		
		// if error encountered, continue to the next iteration of the loop
		if (cont) continue;
		
		// print the calculation
		std::cout << calculate(equation) << std::endl;
	}

	std::cout << "Exiting..." << std::endl;
	return 0;
}