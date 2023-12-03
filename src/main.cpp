#include <iostream>
#include <string>
#include <stdexcept>

#include "src/io_wrapper.h"
#include "src/calculator.h"

int main() {
	IOWrapper io_wrapper;
	Calculator calculator;

	std::cout << "\nWelcome to the calculator (type \"quit\" to exit)" << std::endl;

	while (1) {
		// Prompt the user for an equation.
		std::string equation = io_wrapper.Prompt();
		if (equation.empty()) {
			continue;
		}

		// If the user enters 'quit', break out of the loop and exit the program.
		if (equation.compare("quit") == 0) {
			break;
		}

		// Validate the equation before sending it to the Calculator.
		try {
			io_wrapper.Validate(equation);
		} catch (const std::invalid_argument& error) {
			std::cout << error.what() << std::endl;
			continue;
		}

		// Calculate the result of the equation.
		double result;
		try {
			result = calculator.Calculate(equation);
			std::cout << result << std::endl;
		} catch (const std::domain_error& error) {
			std::cout << error.what() << std::endl;
		}
	}

	std::cout << "Exiting...\n" << std::endl;
	return 0;
}
