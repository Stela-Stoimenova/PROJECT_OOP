#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;
class Table {
	private:
		string name="";
	public:
		Table(string name) : name(name) {
			
		}

		void createTable() {
			ofstream f(this->name + ".txt");
			f.close();
		}
};
//The code reads a command from the user, splits the command into individual words,
// and displays each word one by one. It also has an exit condition: if the user enters "0",
// the program terminates.
class Input
{
private:
	
	string userCommand = ""; //store user's input
	string delimiter = " "; //identify spaces between words
	bool isRunning = true; 
	string firstWord = "";
	string secondWord = "";
	string thirdWord = "";
public:
	
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
			cout << endl << "Exited the program successfully.";
			this->isRunning = false;
		}
		else
		{
			// Bogdan's code
			auto pos = userCommand.find(delimiter);
			this->firstWord = userCommand.substr(0, pos);

			
			// Getting second word
			userCommand.erase(0, pos + delimiter.length());
			pos = userCommand.find(delimiter);
			this->secondWord = userCommand.substr(0, pos);

			// Getting third word
			userCommand.erase(0, pos + delimiter.length());
			pos = userCommand.find(delimiter);
			this->thirdWord = userCommand.substr(0, pos);

			// cout << "First word is: " << this->firstWord << endl;

			// while (pos != string::npos) {
			// 	cout << userCommand.substr(0, pos) << " ";

			// 	userCommand.erase(0, pos + delimiter.length());
			// 	pos = userCommand.find(delimiter);
				
			// }
			// pos = userCommand.find('\0');
			// cout << userCommand.substr(0, pos) << " ";

			// cout << userCommand.substr(0, pos);
			// userCommand.erase(0, pos + delimiter.length());
		}
	}
	// friend istream& operator>>(istream& in, Input& input) {
	// 	cout << "Input a command (0 to exit): ";
	// 	in >> input.userCommand;
	// }
	inline bool getIsRunning() {return this->isRunning;}
	inline string getFirstWord() {return this->firstWord;}
	inline string getDelimiter() {return this->delimiter;}
	inline string getUserCommand() {return this->userCommand;}
	inline string getSecondWord() {return this->secondWord;}
	inline string getThirdWord() {return this->thirdWord;}
};

// function to turn strings to uppercase
string upper(string str){
	for(int i{};i<str.size();++i) {
		if(str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	}
	return str;
}

int main()
{
	Input i1;
	while(i1.getIsRunning())
	{
		i1.readInput();
		if(i1.getIsRunning()) {
			if(upper(i1.getFirstWord()) == "CREATE") {

				if(upper(i1.getSecondWord()) != "TABLE")
				{
					cout << "Invalid command";
				}
				else {
					Table t1(i1.getThirdWord());
					t1.createTable();
				}
			}
			else {
				cout << "Invalid command.";
			}
		}
	}
}
