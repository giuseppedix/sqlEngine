#ifndef SQLENGINE_SQLENGINE_H
#define SQLENGINE_SQLENGINE_H

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../Table/Table.h"
#include "State.h"
#include <bitset>

using namespace std;

#define CREATE_TABLE_D "createtable"
#define DROP_TABLE_D "droptable"
#define INSERT_INTO_D "insertinto"
#define DELETE_FROM_D "deletefrom"
#define TRUNCATE_TABLE_D "truncatetable"
#define UPDATE_D "createtable"
#define SELECT_D "select"
#define SELECTALLFROM_D "select*from"
#define ORDER_BY_D "orderby"


// Mask: 3 bit per il tipo, 1 bit per NOT NULL, 1 bit AUTO_INCREMENT, 1 bit PRIMARY KEY

#define MASK_INVALID 0b000000
#define MASK_INT 0b000001 //int
#define MASK_FLOAT 0b000010 //float
#define MASK_CHAR 0b000011 //char
#define MASK_TEXT 0b000100 //text
#define MASK_DATE 0b000101 //date
#define MASK_TIME 0b000110 //time

#define MASK_NOTNULL 0b001000 //not null
#define MASK_AUTOINCREMENT 0b010000 //autoincrement
#define MASK_PRIMARYKEY 0b100000 //primarikey

enum Command {CREATE_TABLE, DROP_TABLE, INSERT_INTO, DELETE_FROM, TRUNCATE_TABLE, UPDATE, SELECT, ORDER_BY, NOT_VALID};

class SqlEngine{

public:
    //DEFAULT CONSTRUCTOR
    SqlEngine() = default;
    //DESTRUCTOR
    ~SqlEngine();
    //METHODS
    int execute(const string &command);
    int loadState(const string &dirPath);
    State getState() const;

    static vector<string> splitValueByDelimiter(string basicString, const string string);

private:
    // METHODS
    string removeSpace(string input);
    Command getCommand(string command_input);
    void executeCreateTable(string basicString);
    void executeInsertInto(string command);
    void removeSubstrs(string &s, string p);
    string findNameTable(string basicString);
    vector<string> getValueInQuote(string s);
    vector<string> getParams(string basicString);

    void getParamInfo(const string &s, int &paramMask, string &name);

    //ATTRIBUTES
    State state;

    void executeSelect(string cmd);

    void executeDropTable(string cmd);

    void executeTruncateTable(string cmd);
};

#endif