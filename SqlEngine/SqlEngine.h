

#ifndef SQLENGINE_SQLENGINE_H
#define SQLENGINE_SQLENGINE_H

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../Table/Table.h"
#include "State.h"

using namespace std;

#define CREATE_TABLE_D "createtable"
#define DROP_TABLE_D "droptable"
#define INSERT_INTO_D "insertinto"
#define DELETE_FROM_D "deletefrom"
#define TRUNCATE_TABLE_D "truncatetable"
#define UPDATE_D "createtable"
#define SELECT_D "select"
#define ORDER_BY_D "orderby"

// Mask: 3 bit per il tipo, 1 bit per NOT NULL, 1 bit AUTO_INCREMENT, 1 bit PRIMARY KEY

#define MASK_GENERIC 0x000000
#define MASK_INT 0x000001 //int
#define MASK_FLOAT 0x000010 //float
#define MASK_CHAR 0x000011 //char
#define MASK_TEXT 0x000100 //text
#define MASK_DATE 0x000101 //date
#define MASK_TIME 0x000110 //time

#define MASK_NOTNULL 0x001000 //not null
#define MASK_AUTOINCREMENT 0x010000 //autoincrement
#define MASK_PRIMARYKEY 0x100000 //primarikey


enum Command {CREATE_TABLE, DROP_TABLE, INSERT_INTO, DELETE_FROM, TRUNCATE_TABLE, UPDATE, SELECT, ORDER_BY, NOT_VALID};

class SqlEngine{
public:

    //METHODS
    SqlEngine() = default;
    int execute(const string &command);
    int loadState(const string &dirPath);
    State getState() const;
private:
    // METHODS
    string removeSpace(string input);
    string getParamsInBrakets(string command);
    Command getCommand(string command_input);
    vector<string> getValuesType();
    //ATTRIBUTES
    State state;


    void executeCreateTable(string basicString);

    void removeSubstrs(string &s, string p);

    string findNameTable(string basicString);

    vector<string> getParams(string basicString);

    vector<string> splitValueByDelimiter(string basicString, const string string);

    void getParamInfo(const string &basicString, string &basicString1, string &basicString2);
};

#endif