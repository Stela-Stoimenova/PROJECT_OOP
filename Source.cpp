#include <iostream>
#include <cstring>
#include <string.h>
#include<string>

int main()
{
	bool isRunning = true;
	std::string userCommand;

	while (isRunning) {
		std::cout << std::endl << "Enter a command: ";
		getline(std::cin, userCommand);

		// Exiting the application
		if (userCommand == std::string("0")) {
			isRunning = false;
		}
	}
}
