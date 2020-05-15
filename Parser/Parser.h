

#ifndef SQLENGINE_PARSER_H
#define SQLENGINE_PARSER_H

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../Table/Table.h"

using namespace std;

#define CREATE_TABLE_D "createtable"
#define DROP_TABLE_D "droptable"
#define INSERT_INTO_D "insertinto"
#define DELETE_FROM_D "deletefrom"
#define TRUNCATE_TABLE_D "truncatetable"
#define UPDATE_D "createtable"
#define SELECT_D "select"
#define ORDER_BY_D "orderby"


enum Command {CREATE_TABLE, DROP_TABLE, INSERT_INTO, DELETE_FROM, TRUNCATE_TABLE, UPDATE, SELECT, ORDER_BY, NOT_VALID};

class Parser{
public:

    Parser() = default;
    int execute(const string &command);



private:

    // METHODS
    string removeSpace(string input);
    string getParamsInBrakets(string command);
    Command getCommand(string command_input);
    vector<string> getValuesType();
    //ATTRIBUTES
    vector <Table> tables;


};

#endif