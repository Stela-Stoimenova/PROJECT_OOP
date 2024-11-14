#include "processor.h"
#include <iostream>
#include <sstream>

using namespace std;

void Processor::processCommand(const string& command) {
    if (command.find("CREATE TABLE") == 0) {
        createTableCommand(command);
    } else if (command.find("DROP TABLE") == 0) {
        dropTableCommand(command);
    } else if (command.find("DISPLAY TABLE") == 0) {
        displayTableCommand(command);
    } else {
        cout << "Error: Unknown command.\n";
    }
}

void Processor::createTableCommand(const string& command) {
    string tableName = getName(command);
    if (tableName.empty()) {
        cout << "Error: Invalid table name.\n";
        return;
    }
    
    void Processor::displayTableCommand(const string& command) {
}

    Table newTable(tableName);
    
    // let s play with TESTS !!
    newTable.addColumns("id", "int", 4, "0");
    newTable.addColumns("name", "string", 255, "''");
    
    // now stop this

    newTable.displayColumns();
    cout << "Table '" << tableName << "' created successfully.\n";
}

void Processor::dropTableCommand(const string& command) {
    string tableName = getName(command);
    if (tableName.empty()) {
        cout << "Error: Invalid table name.\n";
        return;
    }
    cout << "Table '" << tableName << "' dropped successfully.\n";
}


string Processor::getName(const string& command) {
    stringstream ss(command);
    string word;
    ss >> word;
    ss >> word;
    ss >> word;
    return word;
}
