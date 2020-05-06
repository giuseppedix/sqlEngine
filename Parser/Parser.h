//
// Created by gdisi on 06/05/2020.
//

#ifndef SQLENGINE_TABLE_H
#define SQLENGINE_TABLE_H

#include <iostream>
#include <vector>
using namespace std;

class Parser{
public:
    int parserStr(string str);

private:
    string getCommand();
    string getParamsInBrakets();
    vector<string> getValuesType();
};

#endif