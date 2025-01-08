#include <iostream>  // For input-output operations
#include <fstream>   // For file handling (not used in this code but included for potential future use)
#include <string>    // For handling strings
#include <vector>    // For using dynamic arrays (vectors)
#include <regex>     // For regular expressions to parse SQL-like commands

using namespace std;

// Represents a column in a table
class Column {
public:
    string name;          // The name of the column
    string type;          // The data type of the column (e.g., integer, text)
    int size;             // The maximum size of the column
    string defaultValue;  // The default value for the column

    // Constructor to initialize a column with its properties
    Column(const string& name, const string& type, int size, const string& defaultValue)
        : name(name), type(type), size(size), defaultValue(defaultValue) {}

 //Diana- i have added this bc i need it for select
        string getName() const { return name; }
        //only this

};

class Row {
public:
    vector<string> values; // values of the columns in a row

    // Constructor to initialize the row with the given values
    Row(const vector<string>& values) : values(values) {}

    // Method to update a specific column value in the row
    void updateColumn(int columnIndex, const string& newValue) {
        if (columnIndex >= 0 && columnIndex < values.size()) {
            values[columnIndex] = newValue;
        }
    }

    // Method to get the value of a specific column
    string getColumnValue(int columnIndex) const {
        if (columnIndex >= 0 && columnIndex < values.size()) {
            return values[columnIndex];
        }
        return "";
    }
};

// Represents a table, which contains a name and a list of columns
class Table {
public:
    string name; // The name of the table
    vector<Column> columns; // A list of columns in the table
    vector<Row> rows; // A list of rows in the table

    // Constructor to initialize a table with a name
    Table(const string& tableName) : name(tableName) {}

    // Adds a new column to the table
    void addColumn(const string& columnName, const string& type, int size, const string& defaultValue) {
        columns.emplace_back(columnName, type, size, defaultValue); // Add a column to the vector
    }

    // Adds a new row to the table
    void addRow(const vector<string>& rowValues) {
        rows.emplace_back(rowValues);
    }

    // Returns the name of the table
    string getName() const { return name; }

    // Displays the structure of the table
    void display() const {
        cout << "Table: " << name << endl;
        cout << "Columns: " << columns.size() << endl;
        for (size_t i = 0; i < columns.size(); ++i) {
            cout << "  Column " << i + 1 << ":\n"
                 << "    Name: " << columns[i].name << endl
                 << "    Type: " << columns[i].type << endl
                 << "    Size: " << columns[i].size << endl
                 << "    Default: " << columns[i].defaultValue << endl;
        }
        cout << "Rows: " << rows.size() << endl;
        for (const auto& row : rows) {
            for (const auto& value : row.values) {
                cout << value << " ";
            }
            cout << endl;
        }
    }

//Diana - I added the list that we previously had and create file bc we will need it later
    vector<Column> getColumns() const { return columns; } 

  void createFile(const string& tableName) {
        ofstream table(tableName + ".txt");

        table.close();
    }
// here it stops what I needed to add

    // Finds column index by name
    int findColumnIndex(const string& columnName) const {
        for (size_t i = 0; i < columns.size(); i++) {
            if (columns[i].name == columnName) {
                return i;
            }
        }
        return -1; // Column not found
    }

    // Finds rows that match WHERE clause condition
    vector<int> findRowsWhere(const string& whereColumn, const string& whereValue) const {
        int columnIndex = findColumnIndex(whereColumn);
        vector<int> matchingRows;

        if (columnIndex != -1) {
            for (size_t i = 0; i < rows.size(); ++i) {
                if (rows[i].getColumnValue(columnIndex) == whereValue) {
                    matchingRows.push_back(i);
                }
            }
        }
        return matchingRows;
    }
};

// Processes SQL-like commands to manage tables
class Processor {
public:
    // Processes a single command input by the user
    void processCommand(const string& command) {
        if (command.find("CREATE TABLE") == 0) {
            createTable(command); // Handle CREATE TABLE command
        }
        else if (command.find("DROP TABLE") == 0) {
            dropTable(command);   // Handle DROP TABLE command
        }
        else if (command.find("UPDATE") == 0) {
            updateTable(command); // Handle UPDATE command
        }
        else if (command == "DISPLAY TABLES") {
            displayTables();      // Handle DISPLAY TABLES command
        }
        else if (command.find("DELETE FROM") == 0) {
            deleteRecord(command); // Handle DELETE FROM command
        }
        else if (command.find("SELECT *") == 0) {
            SelectAllTable(command);
        }

        else if (command.find("SELECT") == 0) {
            SelectTable(command);
        }

        else {
            cout << "Error: Unknown command.\n";
        }
    }

private:
    vector<Table> tables; // A collection of tables managed by the processor

    // Update table based on the UPDATE command
    void updateTable(const string& command) {
        try {
            // Regex to validate and parse the UPDATE command
            regex updateRegex(
                R"(UPDATE\s+(\w+)\s+SET\s+(\w+)\s*=\s*\"?(.+?)\"?\s+WHERE\s+(\w+)\s*=\s*\"?(.+?)\"?)",
                regex::icase
            );
            smatch matches;

            if (!regex_search(command, matches, updateRegex)) {
                cout << "Error: Invalid UPDATE syntax." << endl;
                return;
            }

            string tableName = matches[1]; // Extract table name
            string columnToSet = matches[2]; // Extract column to update
            string newValue = matches[3]; // Extract new value
            string whereColumn = matches[4]; // Extract WHERE column
            string whereValue = matches[5]; // Extract WHERE value

            // Find the table
            auto tableIt = find_if(tables.begin(), tables.end(), [&tableName](const Table& t) {
                return t.getName() == tableName;
            });

            if (tableIt == tables.end()) {
                cout << "Error: Table '" << tableName << "' does not exist." << endl;
                return;
            }

            // Validate the column to be updated
            int columnIndex = tableIt->findColumnIndex(columnToSet);
            if (columnIndex == -1) {
                cout << "Error: Column '" << columnToSet << "' does not exist in table '" << tableName << "'." << endl;
                return;
            }

            // Validate the WHERE column
            vector<int> matchingRows = tableIt->findRowsWhere(whereColumn, whereValue);
            if (matchingRows.empty()) {
                cout << "Error: No rows found matching the filter condition." << endl;
                return;
            }

            // Apply the update to each matching row
            for (int rowIndex : matchingRows) {
                auto& row = tableIt->rows[rowIndex];
                row.updateColumn(columnIndex, newValue);  // Update the column value
            }

            cout << "Rows updated successfully." << endl;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    // Creates a new table based on the CREATE TABLE command
    void createTable(const string& command) {
        string tableName;
        vector<Column> columns;

        try {
            // Regular expression to parse the CREATE TABLE command
            regex createTableRegex(
                R"(CREATE TABLE (\w+)\s*\((.+)\))",
                regex::icase // Case-insensitive matching
            );
            smatch matches;

            if (regex_search(command, matches, createTableRegex)) {
                tableName = matches[1]; // Extract the table name
                string columnDefinitions = matches[2]; // Extract the column definitions

                // Check if the table already exists
                for (const auto& table : tables) {
                    if (table.getName() == tableName) {
                        cout << "Error: Table '" << tableName << "' already exists." << endl;
                        return;
                    }
                }

                Table newTable(tableName); // Create a new table object

                // Regular expression to extract individual column definitions
                regex columnRegex(
                    R"(\s*(\w+)\s*,\s*(\w+)\s*,\s*(\d+)\s*,\s*'?(.+?)'?\s*)"
                );
                sregex_iterator begin(columnDefinitions.begin(), columnDefinitions.end(), columnRegex);
                sregex_iterator end;

                if (begin == end) {
                    cout << "Error: No valid columns found." << endl;
                    return;
                }

                // Parse each column and add it to the table
                for (sregex_iterator it = begin; it != end; ++it) {
                    smatch columnMatch = *it;
                    string colName = columnMatch[1]; // Column name
                    string colType = columnMatch[2]; // Column type
                    int colSize = stoi(columnMatch[3]); // Column size
                    string defaultValue = columnMatch[4]; // Default value

                    newTable.addColumn(colName, colType, colSize, defaultValue);
                }

                tables.push_back(newTable); // Add the new table to the collection
                cout << "Table '" << tableName << "' created successfully." << endl;


                
                //I needed to make a better display table for SELECT
                  newTable.createFile(tableName);
                for (const Column& col : columns) {
                    newTable.addColumn(col.name, col.type, col.size, col.defaultValue);
                }

                tables.push_back(newTable);
                newTable.display();
                
                //the new display is here :)
                
            }
            else {
                cout << "Error: Invalid CREATE TABLE syntax." << endl;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    // Drops (deletes) a table based on the DROP TABLE command
    void dropTable(const string& command) {
        string tableName;

        try {
            // Regular expression to parse the DROP TABLE command
            regex dropTableRegex(R"(DROP TABLE (\w+))", regex::icase);
            smatch matches;

            if (regex_search(command, matches, dropTableRegex)) {
                tableName = matches[1]; // Extract the table name

                // Find and erase the table from the collection
                auto it = find_if(tables.begin(), tables.end(), [&tableName](const Table& t) {
                    return t.getName() == tableName;
                });
                if (it != tables.end()) {
                    tables.erase(it);
                    cout << "Table '" << tableName << "' dropped successfully." << endl;
                } else {
                    cout << "Error: Table '" << tableName << "' does not exist." << endl;
                }
            }
            else {
                cout << "Error: Invalid DROP TABLE syntax." << endl;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    // Displays the names of all existing tables
    void displayTables() {
        if (tables.empty()) {
            cout << "No tables exist." << endl;
        } else {
            cout << "Existing tables:" << endl;
            for (const auto& table : tables) {
                cout << " - " << table.getName() << endl;
            }
        }
    }

    // Deletes records from the table based on the DELETE command
    void deleteRecord(const string& command) {
        try {
            // Regex to parse DELETE FROM command with optional WHERE clause
            regex deleteRegex(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(\w+)\s*=\s*'?([^']+)'?)", regex::icase);
            smatch deleteMatches;

            if (regex_search(command, deleteMatches, deleteRegex)) {
                string tableName = deleteMatches[1];
                string conditionColumn = deleteMatches[2];
                string conditionValue = deleteMatches[3];

                // Find the table in the vector
                auto tableIt = find_if(tables.begin(), tables.end(), [&tableName](const Table& t) {
                    return t.getName() == tableName;
                });
                if (tableIt == tables.end()) {
                    cout << "Error: Table '" << tableName << "' does not exist." << endl;
                    return;
                }

                // Find rows that match the condition
                vector<int> matchingRows = tableIt->findRowsWhere(conditionColumn, conditionValue);
                if (matchingRows.empty()) {
                    cout << "Error: No rows found matching the condition." << endl;
                    return;
                }

                // Delete matching rows
                for (int rowIndex : matchingRows) {
                    tableIt->rows.erase(tableIt->rows.begin() + rowIndex);
                }

                cout << "Rows deleted successfully." << endl;
            }
            else {
                cout << "Error: Invalid DELETE syntax." << endl;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }


 void SelectAllTable(const string& command) {
        try {
            regex selectAllRegex(R"(SELECT\s\*\sFROM\s(\w+))", regex::icase);
            smatch matches;

            if (regex_search(command, matches, selectAllRegex)) {
                string tableName = matches[1];
                auto it = find_if(tables.begin(), tables.end(), [&](const Table& table) {
                    return table.getName() == tableName;
                    });

                if (it != tables.end()) {
                    Table& table = *it;
                    cout << "Table: " << tableName << endl;

                    for (const auto& col : table.getColumns()) {
                        cout << "Column: " << col.getName() << endl;
                    }
                    table.display();
                }
                else {
                    cout << "Error: Table '" << tableName << "' does not exist.\n";
                }
            }
            else {
                cout << "Error: Failed to parse SELECT * command." << endl;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

   

  
        
  
void SelectTable(const string& command) {
    try {
        // Regex for SELECT command with optional WHERE clause
        regex selectRegex(R"(SELECT\s+([^\s]+|\)\s+FROM\s+(\w+)(\s+WHERE\s+(\w+)\s=\s*'?([^']*)'?)?)", regex::icase);
        smatch matches;

        if (regex_search(command, matches, selectRegex)) {
            string columnsPart = matches[1]; // Columns to select
            string tableName = matches[2];  // Table name
            string whereClause = matches[3]; // Optional WHERE clause
            string whereColumn = matches[4]; // Column in WHERE
            string whereValue = matches[5]; // Value in WHERE

            // Find the table
            auto it = find_if(tables.begin(), tables.end(), [&](const Table& table) {
                return table.getName() == tableName;
            });

            if (it == tables.end()) {
                cout << "Error: Table '" << tableName << "' does not exist.\n";
                return;
            }

            Table& table = *it;

            // Parse selected attributes
            vector<string> selectedAttributes;
            if (columnsPart != "*") {
                stringstream ss(columnsPart);
                string attribute;
                while (getline(ss, attribute, ',')) {
                    selectedAttributes.push_back(attribute);
                }
            }

            // Match attributes to column properties
            vector<string> outputResults;
            for (const string& attr : selectedAttributes) {
                bool found = false;
                for (const Column& col : table.columns) {
                    if (attr == "name" && !col.name.empty()) {
                        outputResults.push_back(attr + ": " + col.name);
                        found = true;
                    } else if (attr == "type" && !col.type.empty()) {
                        outputResults.push_back(attr + ": " + col.type);
                        found = true;
                    } else if (attr == "size") {
                        outputResults.push_back(attr + ": " + to_string(col.size));
                        found = true;
                    } else if (attr == "default" && !col.defaultValue.empty()) {
                        outputResults.push_back(attr + ": " + col.defaultValue);
                        found = true;
                    }
                }
                if (!found) {
                    cout << "Error: Attribute '" << attr << "' does not exist in table '" << tableName << "'.\n";
                    return;
                }
            }

            // Display results
            cout << "Table: " << tableName << endl;
            for (const string& result : outputResults) {
                cout << result << endl;
            }
        } else {
            cout << "Error: Invalid SELECT syntax.\n";
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    } catch (const char* msg) {
        cout << "Error: " << msg << endl;
    }
}



};

// Entry point of the program
int main() {
    Processor processor; // Create an instance of the Processor class
    string command;

    while (true) {
        cout << "\nEnter command (or type 'EXIT' to quit): ";
        getline(cin, command); // Get the user's command

        if (command == "EXIT") { // Exit the program if the user types 'EXIT'
            cout << "Exiting program." << endl;
            break;
        }

        processor.processCommand(command); // Process the user's command
    }

    return 0; // Indicate successful program termination
}
