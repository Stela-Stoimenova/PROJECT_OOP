#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;


struct Column {
    string name;
    string type;
    //columnType type;
    int size;  
    string defaultValue;
      public:
    // Constructor
    Column(const string& name, const string& type, int size, const string& defaultValue)
        : name(name), type(type), size(size), defaultValue(defaultValue) {}

    // Getters
    string getName() const { return name; }
    string getType() const { return type; }
    int getSize() const { return size; }
    string getDefaultValue() const { return defaultValue; }

    // Setters
    void setName(const string& newName) { name = newName; }
    void setType(const string& newType) { type = newType; }
    void setSize(int newSize) { size = newSize; }
    void setDefaultValue(const string& newValue) { defaultValue = newValue; }

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
        for(auto a = 0;a < columns.size();a++)
        {
            table << columns[a].name << " ";
        }
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
        else if(command=="DISPLAY TABLES")
        {
            displayTables();
        }
        else if(command=="DELETE FROM") 
        {
            deleteColumn(command);
        }

        else {
            cout << "Error: Unknown command type.\n";
        }
    }

private:
vector<Table> tables;//stores all created tables


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

                // Check if the table already exists
                for (const auto& table : tables) {
                    if (table.getName() == tableName) {
                        cout << "Error: Table '" << tableName << "' already exists." << endl;
                        return;
                    }
                }
                Table newTable(tableName);
                tables.push_back(newTable);
                // Regex to extract column definitions
                regex columnRegex(
                    R"((\w+),\s*(\w+),\s*(\d+),\s*('?.*'?))"
                    
                );

                sregex_iterator begin(columnDefinitions.begin(), columnDefinitions.end(), columnRegex);
                sregex_iterator end;

                for (auto it = begin; it != end; ++it) {
                    smatch columnMatch = *it;
                    string colName = columnMatch[1];  
                    string colType = columnMatch[2];  
                    int colSize = stoi(columnMatch[3]); 
                    string defaultValue = columnMatch[4];
                    
                    //basically it goes to 3 outputs, if there 3 commands, it[1] finds column name
                    //it[2]finds type int,float which is a string
                    //it[3] finds size.
                    
                    newTable.addColumn(colName, colType, colSize, defaultValue);

                }

                //if (columns.empty()) {
                    //throw "No valid columns found.";
                    //cout << "No valid columns found.";
                //}

                //check if the table already exists
                // for(const auto& table : tables)
                // {
                //     if(table.getName()==tableName)
                //     {
                //         cout<< "Error:  Table '"<<tableName<<"' already exists.\n";
                //         return;
                //     }
                // }
                
                //Create and store the new table
                // Table newTable(tableName);
                newTable.createFile(tableName);
                for (const Column& col : columns) {
                    newTable.addColumn(col.name, col.type, col.size, col.defaultValue);
                }

                tables.push_back(newTable);
                newTable.display();
            } else {
                throw "Error: Invalid CREATE TABLE syntax.";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
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
                 for (auto it = tables.begin(); it != tables.end(); ++it) {
                    if (it->getName() == tableName) {
                        tables.erase(it);
                        cout << "Table '" << tableName << "' dropped successfully." << endl;
                        return;
                    }
                }
                cout << "Error: Table '" << tableName << "' does not exist." << endl;
            } else {
                cout << "Error: Invalid DROP TABLE syntax." << endl;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }

    }

    void InsertIntoTable(const string& command) {
        
    }

    void deleteColumn(const string& command) {
        
        try {
            string tableName;
            string columnName;
            regex deleteRegex( R"(DELETE\s*FROM\s(\w+)\s*(WHERE\s*(\w+)\s*=\s*'([^']+)'|\d+)?)" , regex::icase);
            smatch deleteMatches;

            if (regex_search(command, deleteMatches, deleteRegex)) {
                tableName = deleteMatches[0];
                cout << tableName;
            }
        }
        catch (const exception& e){
            cout << "Error: " << e.what() << endl;
        }
    }

    void displayTables()
    {
        if(tables.empty())
        {
            cout<<"No tables exist."<<endl;
        }
        else
        {
           cout<<"Existing tables:"<<endl;
           for(const auto& table:tables)
           {
            cout<<" - "<<table.getName()<<endl;
           }
        }
    }

};

int main() {
    Processor processor;

    string command;

    while (true) {
        cout << "\nEnter command (or type 'EXIT' to quit): ";
        getline(cin, command);

        if (command == "EXIT") {
            cout << "Exiting program." << endl;
            break;
        }

        processor.processCommand(command);
    }

    return 0;
}
