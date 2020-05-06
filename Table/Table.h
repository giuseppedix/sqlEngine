//
// Created by gdisi on 06/05/2020.
//

#ifndef SQLENGINE_TABLE_H
#define SQLENGINE_TABLE_H
#include <vector>
#include <iostream>
#include "Row.h"
#include "Column.h"

using namespace std;

class Table {
    string tableName;
    vector<Row> rows;
    vector<Column> cols;
};


#endif //SQLENGINE_TABLE_H
