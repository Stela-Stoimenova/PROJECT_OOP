#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

/*enum class columnType{
    INT, FLOAT, VARCHAR, BOOL,
    UNKNOWN //for unsupported or unrecognised types
};*/

struct Column {
    string name;
    string type;
    //columnType type;
    int size;  //should i make it a string bc it is too complicated ????
    string defaultValue;
};

class Table {
public:
    Table(const string& tableName) : name(tableName) {}

    string getName() const {return name;}

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

    void createFile(const string& tableName) {
        ofstream table(tableName + ".txt");
        
        table.close();
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
        } 
        else if (command.find("DROP TABLE") == 0) {
            DropTable(command);
        }
        else if (command.find("INSERT INTO") == 0) {
            // Insert into command
            cout << "Inserting into table";
            InsertIntoTable(command);
        }

        else {
            cout << "Error: Unknown command type.\n";
        }
    }

private:
vector<Table> tables;//stores all created tables

//helper function for enum
/*columnType parseColumnType(const string& typeStr)
{
if (typeStr == "int") return columnType::INT;
  if (typeStr == "float") return columnType::FLOAT;
  if (typeStr == "varchar") return columnType::VARCHAR;
  if (typeStr == "bool") return columnType::BOOL;
  return columnType::UNKNOWN;
}

string columnTypeToString(columnType type)
{
    switch (type)
    {
    case columnType::INT:
    return "int";
    case columnType::FLOAT:
    return "float";
    case columnType::VARCHAR:
    return "varchar";
    case columnType::BOOL:
    return "bool";
    
    default:
        return "uknown";
    }
}*/


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
                

                string columnDefinitions = matches[2];

                
                regex columnRegex(
                    R"((\w+),\s*(\w+),\s*(\d+),\s*('?.*'?))"
                    
                );

                sregex_iterator begin(columnDefinitions.begin(), columnDefinitions.end(), columnRegex);
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

                //check if the table already exists
                for(const auto& table : tables)
                {
                    if(table.getName()==tableName)
                    {
                        cout<< "Error:  Table '"<<tableName<<"' already exists.\n";
                        return;
                    }
                }
                
                //Create and store the new table
                Table newTable(tableName);
                newTable.createFile(tableName);
                for (const Column& col : columns) {
                    newTable.addColumn(col.name, col.type, col.size, col.defaultValue);
                }

                tables.push_back(newTable);
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

    void DropTable(const string& command) {
        string tableName;

        try {
            regex dropTableRegex(R"(DROP TABLE (\w+))", regex::icase);
            smatch matches;
            if (regex_search(command, matches, dropTableRegex)) {
                tableName = matches[1];

                // Find the table in the vector
                //auto-specifies that the type of the variable that is being declared
                //will be automatically deducted from its initializer by the compiler
                auto it=find_if(tables.begin(), tables.end(), [&](const Table& table)
                {
                    return table.getName()==tableName;
                });

                if(it!=tables.end())
                {
                    tables.erase(it);//remove the table
                    cout<<"Table '"<<tableName<<"' does not exist.\n";
                }
                else {
                    cout<<"Error: Table '"<<tableName<<"' does not exist.\n";
                }
    
            
            } else {
                throw "Failed to parse DROP TABLE command.";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        } catch (const char* msg) {
            cout << "Error: " << msg << endl;
        }
    }

    void InsertIntoTable(const string& command) {
        
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
