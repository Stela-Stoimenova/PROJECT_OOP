#pragma once
#include <string>
#include "table.cpp"

using namespace std;

class Processor {
public:
    void processCommand(const string& command);
    
private:
    void createTableCommand(const string& command);
  
    void displayTableCommand(const string& command);
    std::string getName(const string& command);
};
