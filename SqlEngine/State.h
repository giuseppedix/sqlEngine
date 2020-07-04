#ifndef SQLENGINE_STATE_H
#define SQLENGINE_STATE_H

#include "../Table/Table.h"
#include <iostream>
#include <fstream>
#include <string>

#define BEGIN "-----BEGIN-----"
#define END "-----END-----"

class State {

public:
    //DEFAULT CONSTRUCTOR
    State() = default;
    //METHODS
    vector<Table> getTables() const{return tables;};
    string getDbName();
    void setDbName(const string &dnName);
    void addTable(string basicString, vector<int> paramsMask, vector<string> paramsNames);
    bool tablePresent(string nametable, int &index);
    void setParamsTable(string nameTable, vector<string> fields, vector<string> values);
    void saveStateOnFile(string filepath = "../repository");
    void load(string filepath = "../repository");
    void eraseTable(int index);
    void truncateTable(int index);
    void deleteFrom(int index, string fieldToDelete, string valueToDelete);
    void update(int &index, string &fieldToSet, string &valueToSet, string &valueToWhere, string &fieldToWhere);

private:
    //ATTRIBUTES
    string dbName;
    vector <Table> tables;

};


#endif //SQLENGINE_STATE_H
