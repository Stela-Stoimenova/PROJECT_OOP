#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
using namespace std;

struct Column {
    string name;
    string type;
    int size;  //should i make it a string bc it is too complicated ????
    string defaultValue;
};

class Table {
public:
    Table(const string& tableName) : name(tableName) {}

    void addColumn(const string& columnName, const string& type, int size, const string& defaultValue) {
        columns.push_back({columnName, type, size, defaultValue});
    }

    void display() const {
        cout << "Table: " << name << endl;
        cout << "Columns: " << columns.size() << endl;
        for (size_t i = 0; i < columns.size(); ++i) {
            cout << "Column " << i + 1 << ":\n";
            cout << "  Name: " << columns[i].name << endl;
            cout << "  Type: " << columns[i].type << endl;
            cout << "  Dimension: " << columns[i].size << endl;
            cout << "  Default: " << columns[i].defaultValue << endl;
        }
    }

private:
    string name;
    vector<Column> columns;
};

class Processor {
public:
    void processCommand(const string& command) {
        if (command.find("CREATE TABLE") == 0) {
            CreateTable(command);
        } else {
            cout << "Error: Unknown command type.\n";
        }
    }

private:
    void CreateTable(const string& command) {
        string tableName;   
        vector<Column> columns;

        try {
            regex createTableRegex(
                R"(CREATE TABLE (\w+)\s*\((.*)\))",
                regex::icase
            );
            smatch matches;
            if (regex_search(command, matches, createTableRegex)) {
                tableName = matches[1];
                string b = matches[2];

                
                regex a(
                    R"((\w+),\s*(\w+),\s*(\d+),\s*('?.*'?))"
                    
                );

                sregex_iterator begin(b.begin(), b.end(), a);
                sregex_iterator end;

                for (sregex_iterator it = begin; it != end; ++it) {
                    smatch match = *it;
                    string name = match[1];  
    string type = match[2];  
    int size = stoi(match[3]); // ??? should i make it a string ?? pls answer 
    string defaultValue = match[4];
                    
                    //basically it goes to 3 outputs, if there 3 commands, it[1] finds column name
                    //it[2]finds type int,float which is a string
                    //it[3] finds size.

                    columns.push_back({name, type, size, defaultValue});
                }

                if (columns.empty()) {
                    throw "No valid columns found.";
                }

                Table newTable(tableName);
                for (const Column& col : columns) {
                    newTable.addColumn(col.name, col.type, col.size, col.defaultValue);
                }
                newTable.display();
            } else {
                throw "Failed to parse command.";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        } catch (const char* msg) {
            cout << "Error: " << msg << endl;
        }
    }
};

int main() {
    Processor processor;

    string command;
    cout << "Enter command: ";
    getline(cin, command);

    processor.processCommand(command);

    return 0;
}
