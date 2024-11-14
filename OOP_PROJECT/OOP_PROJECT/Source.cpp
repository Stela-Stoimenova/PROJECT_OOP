#pragma once
#include <iostream>
#include <string>
#include "table.cpp"
#include "processor.h"

using namespace std;

int main() {
    Processor processor;
    string command1 = "CREATE TABLE students";
    processor.processCommand(command1);

    return 0;
}
