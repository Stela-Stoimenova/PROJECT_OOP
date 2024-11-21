#pragma once
#include <iostream>
#include <string>
#include "table.cpp"
#include "processor.h"

using namespace std;

int main() {
    Processor processor;

    //string command1 = "CREATE TABLE students";
    string command1 = "";
    getline(cin, command1);
    for(int i{};i<command1.size();++i)
    {
        if(command1[i] >= 'a' && command1[i] <= 'z') {
            command1[i]-=32;
        }
    }
    processor.processCommand(command1);

    return 0;
}
