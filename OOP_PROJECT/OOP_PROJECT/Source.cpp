#include <iostream>
#include <cstring>
#include <string.h>
#include<string>

using namespace std;


//The code reads a command from the user, splits the command into individual words,
// and displays each word one by one. It also has an exit condition: if the user enters "0",
// the program terminates.
class Input
{
private:
	
	string userCommand = ""; //store user's input
	string delimiter = " "; //identify spaces between words
	string* commandList = nullptr;
	
	// int noCommands = 0;

public:
	bool isRunning = true; 
	//in order to keep the code running
	// allowing the program to keep running until the user decides to exit

	/*void addToCommandList(string word, int noCommands)
	{
		string* listCopy = new string[noCommands];
		for(int i{};i<noCommands;++i)
		{
			listCopy[i] = this->commandList[i];
		}
		delete[] this->commandList;
		for (int i{}; i < noCommands; ++i)
		{
			this->commandList[i] = listCopy[i];
		}
		this->commandList[noCommands] = word;
		this->noCommands++;
	}*/
	void readInput()
	{
		cout << endl << "Enter a command: ";
		getline(cin, userCommand);

		// Exiting the application if user command is '0'
		if (userCommand == string("0")) {
			this->isRunning = false;
			cout << endl << "Exited the program successfully.";
		}
		else
		{
			// Bogdan's code
			auto pos = userCommand.find(delimiter);
			cout << "First word is: " << userCommand.substr(0, pos) << endl;
			while (pos != string::npos) {
				cout << userCommand.substr(0, pos) << " ";
				userCommand.erase(0, pos + delimiter.length());
				pos = userCommand.find(delimiter);
				
			}
			pos = userCommand.find('\0');
			cout << userCommand.substr(0, pos) << " ";
			userCommand.erase(0, pos + delimiter.length());
		}
	}
};

int main()
{
	//bool isRunning = true;
	//std::string userCommand;

	//while (isRunning) {
	//	std::cout << std::endl << "Enter a command: ";
	//	getline(std::cin, userCommand);

	//	// Exiting the application
	//	if (userCommand == std::string("0")) {
	//		isRunning = false;
	//	}
	//}

	Input i1;
	while(i1.isRunning)
	{
		i1.readInput();
	}
	
	
}