#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <algorithm>
#include <string>
#include <cmath>
#include <cstring>

#include <vector>
#include "stack.h"

/*
	Operator Precedence. A lower return value means that the operator has a higher precedence, and if
	the operator isn't recognized, then return -1. This function is used when converting the tokens from infix
	order to postfix order.
*/
int operatorPrec(const char c) {
	switch(c) {
		case '*':
			return 2;
		case '/':
			return 2;
		case '+':
			return 3;
		case '-':
			return 3;
		case '^':
			return 1;
		case '%':
			return 2;
		case 'n':
			return 0;
		case 'p':
			return 0;
		default:
			return -1;
	}
}

/*
	Checks if a given character is a valid parenthesis, returning true if it is and false if not. Used when initially tokenizing
	the string, and signals that the value that is currently being stored in the "value" vector is to then be added
	to the infix vector (if the "value" vetor has a value). Then it adds the parenthesis to the infix vector.
*/
bool isValidParenthesis(const char c) {
	switch(c) {
		case '(':
			return true;
		case ')':
			return true;
		default:
			return false;
	}
}

/*
	Checks if a given character is a valid digit (or decimal), returning true if the character's numerical value is between that of '0' and '9'
	or is '.'. Used during tokenization, when compiling a value together from the input string.
*/
bool isDigit(const char c) {
	if ( (c >= '0' && c <= '9') || c == '.') return true;
	return false;
}

/*
	Checks if a given character is a valid operator, returning true if it is and false if it isn't. Used during tokenization, and signals the end
	of a value (if there is one) before adding the operator to the infix tokens vector.
*/

bool isBinaryOperator(const char c) {
	switch(c) {
		case'+':
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

bool isUnaryOperator(const char c) {
	switch(c) {
		case 'n':
			return true;
		case 'p':
			return true;
		default:
			return false;
	}
}

bool isOperator(const char c) {
	return (isBinaryOperator(c) || isUnaryOperator(c));
}

/*
	Compute an operation between two operands. LHS = left-hand-side, RHS = right-hand-side, OPT = operator.
	Used when calculating the final value from the postfix tokens. If a character is not a valid operator, then return
	0.0 instead of an actual calculation.
*/
double computeBinaryOperation (const double lhs, const double rhs, const std::string& opt) {
	switch(opt[0]) {
		case '+':
			return lhs + rhs;
		case '-':
			return lhs - rhs;
		case '*':
			return lhs * rhs;
		case '/':
			return lhs / rhs;
		case '^':
			return pow(lhs,rhs);
		case '%':
			return std::fmod(lhs,rhs);
		default:
			std::cout << "Error: unknown operator: " << opt << std::endl;
			return 0.0;
	}
}

double computeUnaryOperation (const double num, const std::string& opt) {
	switch (opt[0]) {
		case 'n':
			return num * -1;
		case 'p':
			return abs(num);
		default:
			std::cout << "Error: unknown operator: " << opt << std::endl;
			return 0.0;
	}
}

/*
	Tokenizes the input string. This means taking the string, and separating every value and operator into individual elements, or tokens,
	so that a value or operator can be processed separately from the rest of the string.
*/
void tokenize(const std::string& s, std::vector<std::string>& tokens) {
	// this vector is for storing a number. only digits (potentially a decimal or negative sign) will be pushed to the vector
	std::vector<char> value;
	
	// for loop to iterate through the string, from index 0 to the end character ('\0') of a string
	for (int index = 0; index < s.length(); index++) {
		// variable to store the current character
		char current = s[index];
		
		// if the character is a digit, then push it to the value vector
		if (isDigit(current)) {
			value.push_back(current);
		}
		
		// if the character is an operator, then add the value to the tokens vector then add the operator to the vector (or do some other stuff if it's a negative sign)
		else if (isOperator(current)) {
			/*
				This if statement is for when the there is currently a value being stored. This means that the operator
				must add the value to the tokens vector and clear the value vector, as the operator denotes the end of a
				number.
			*/
			if (!value.empty()) {
				char number[value.size() + 1];
				for (int i = 0; i < value.size(); i++) number[i] = value[i];
				number[value.size()] = '\0';
				tokens.push_back(number);
				
				value.clear();
			}
			
			/*
				To do: need a way to check if a number should be negative or not. The way I previously did this was by checking if the previous
				character was an operator or open parenthesis, and if it was, then it assumed the current character was a negative sign. I'd like to
				come up with a better way to do this, but don't want to commit to a solution at the moment, so I'm leaving this open for now.
			*/
			
			if (index > 0) {
				char prev = s[index - 1];
				if ((isOperator(prev) || prev == '(') && (current == '-' || current == '+')) {
					if (current == '-') tokens.push_back("n");
					else tokens.push_back("p");
				}
				else {
					char opt[1];
					opt[0] = current;
					tokens.push_back(opt);
				}
			}
			else if (current == '-' || current == '+') {
				if (current == '-') tokens.push_back("n");
				else tokens.push_back("p");
			}
		}
		
		// if the character is a parenthesis, then add the value vector to the tokens vector if necessary, then add the parenthesis to the vector
		else if (isValidParenthesis(current)) {
			
			/*
				This if statement is to empty the value vector and add the number in it to the tokens vector, given that there is actually
				a value being stored.
			*/
			if (!value.empty()) {
				char number[value.size() + 1];
				for (int i = 0; i < value.size(); i++) number[i] = value[i];
				number[value.size()] = '\0';
				tokens.push_back(number);
				
				value.clear();
			}
			
			// convert the current character to a string and push it to the tokens vector
			char opt[1];
			opt[0] = current;
			tokens.push_back(opt);
		}
	}
	
	// if there is a number currrently in the value vector, add it to the tokens vector
	if (!value.empty()) {
		char number[value.size() + 1];
		for (int i = 0; i < value.size(); i++) number[i] = value[i];
		number[value.size()] = '\0';
		tokens.push_back(number);
		
		value.clear();
	}
}

/*
	Convert the tokens from infix order (how a human normally reads an equation) to postfix order (the way that is easier for a computer to
	compute the equation).
*/
void infixToPostfix(std::vector<std::string>& infix_tokens, std::vector<std::string>& postfix_tokens) {
	
	// stack for the operators, where they are added/removed (ordered) based on their precedence (PEMDAS)
	stack<std::string> opt_stack;
	
	// iterate through the tokens, read each one and decide how they should be ordered in the postfix_tokens
	for (int i = 0; i < infix_tokens.size(); i++) {
		
		// store the current token
		std::string token = infix_tokens[i];
		
		/*
			If the current token is an open parenthesis, add it to the operator stack. when a close parenthesis is seen, every operator
			between the top and the closed parenthesis will be popped to the postfix_tokens vector.
		*/
		if (token[0] == '(') opt_stack.push(token);
		
		// if the token is an operator, push it to the operator stack. pop any operator in the stack of lower precedence
		else if (isOperator(token[0])) {
			
			// push the operator to the top of the stack if the stack is empty
			if (opt_stack.empty()) opt_stack.push(token);
			
			else {
				// pop every operator on the stack until we hit either a parenthesis or an operator of higher precedence
				while (opt_stack.top()[0] != '(' && operatorPrec(opt_stack.top()[0]) <= operatorPrec(token[0])) {
					postfix_tokens.push_back(opt_stack.top());
					opt_stack.pop();
					
					if (opt_stack.empty()) break;
				}
				opt_stack.push(token);
			}
		}
		
		// if the token is a close parenthesis, pop every operator between the top of the stack and the next open parenthesis
		else if (token[0] == ')') {
			while (opt_stack.top()[0] != '(') {
				postfix_tokens.push_back(opt_stack.top());
				opt_stack.pop();
			}
			opt_stack.pop();
		}
		
		// if the token is not an operator then it must be a number, so the token gets added to the postfix_tokens immediately
		else postfix_tokens.push_back(token);
	}
	
	// add what's left in the operator stack to the postfix_tokens
	while (!opt_stack.empty()) {
		postfix_tokens.push_back(opt_stack.top());
		opt_stack.pop();
	}
}

/*
	Evaluate the equation using the postfix tokens. This function uses a stack to hold the values calculated,
	pushing to the stack when a new value token is encountered, and using the top two values in the stack
	when an operator token is encountered to evaluate, then the result of that operation is pushed
	back to the stack.
*/
double calPostfix(const std::vector<std::string>& postfix_tokens) {
	// operand stack to hold the values of the equation
	stack<double> operand_stack;
	
	// iterate through all the postfix tokens for evaluation
	for (int i = 0; i < postfix_tokens.size(); i++) {
		// store the current token
		std::string token = postfix_tokens[i];
		
		
		/*
			If the current token is an operator, use the top two values on the stack as the operands and
			push the result back to the stack.
		*/
		if (isOperator(token[0])) {
			if(isBinaryOperator(token[0])) {
				double num2 = operand_stack.top();
				operand_stack.pop();
				double num1 = operand_stack.top();
				operand_stack.pop();
				
				operand_stack.push(computeBinaryOperation(num1, num2, token));
			}
			else if (isUnaryOperator(token[0])) {
				double num = operand_stack.top();
				operand_stack.pop();
				
				operand_stack.push(computeUnaryOperation(num,token));
			}
		}
		
		// if not doing an operation then just push the value to the stack
		else operand_stack.push(std::stod(token));
	}
	
	// store the final value, empty the stack, then return the value
	double value = operand_stack.top();
	while (!operand_stack.empty()) operand_stack.pop();
	return value;
}

/*
	Perform the calculation process, where we:
		take a string
		tokenize it to tokens of operators, operands, and parentheses (infix_tokens)
		reorder the tokens from infix to postfix order
		perform the calculation using the postfix tokens
*/
double calculate(const std::string& s) {
	std::vector<std::string> infix_tokens;
	std::vector<std::string> postfix_tokens;
	tokenize(s, infix_tokens);
	infixToPostfix(infix_tokens, postfix_tokens);
	return calPostfix(postfix_tokens);
}

#endif