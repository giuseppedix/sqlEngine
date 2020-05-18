//
// Created by gdisi on 16/05/2020.
//

#ifndef SQLENGINE_STATE_H
#define SQLENGINE_STATE_H


#include "../Table/Table.h"

class State {
public:
    State() = default;

    vector<Table> getTables() const;
    string getDbName();
    void setDbName(const string &dnName);

    void addTable(string basicString, vector<int> paramsMask, vector<string> paramsNames);

private:
    string dbName;
    vector <Table> tables;
};


#endif //SQLENGINE_STATE_H
