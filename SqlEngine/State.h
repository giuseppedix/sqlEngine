#ifndef SQLENGINE_STATE_H
#define SQLENGINE_STATE_H

#include "../Table/Table.h"

class State {

public:
    //DEFAULT CONSTRUCTOR
    State() = default;
    //METHODS
    vector<Table> getTables() const;
    string getDbName();
    void setDbName(const string &dnName);
    void addTable(string basicString, vector<int> paramsMask, vector<string> paramsNames);
    bool tablePresent(string nametable, int &index);
    void setParamsTable(string nameTable, vector<string> fields, vector<string> values);

private:
    //ATTRIBUTES
    string dbName;
    vector <Table> tables;

};


#endif //SQLENGINE_STATE_H
