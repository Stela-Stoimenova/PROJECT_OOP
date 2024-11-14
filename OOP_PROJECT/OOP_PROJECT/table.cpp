#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Table {
public:
    Table(const string& tableName) : name(tableName) {}

    void addColumns(const string& columnName, const string& type, int size, const string& defaultValue) {
        Column column = {columnName, type, size, defaultValue};
        columns.push_back(column);
    }

    void displayColumns() {
        cout << "Table: " << name << endl;
        for (const auto& column : columns) {
            cout << "  Column: " << column.name << ", Type: " << column.type
                 << ", Size: " << column.size << ", Default: " << column.defaultValue << "\n";
        }
    }

private:
    struct Column {
        string name;
        string type;
        int size;
        string defaultValue;
    };

    string name;
    vector<Column> columns;
};
