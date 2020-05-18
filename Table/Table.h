

#ifndef SQLENGINE_TABLE_H
#define SQLENGINE_TABLE_H
#include <vector>
#include <iostream>
#include "Row.h"
#include "Column.h"

using namespace std;

class Table {
public:
    Table() = default;
    string tableName;
    vector<Row> rows;
    vector<Column> cols;
    string getName(){return tableName;};
    void setName(string name){ this->tableName = name;};

    const string &getTableName() const {
        return tableName;
    }

    void setTableName(const string &tableName) {
        Table::tableName = tableName;
    }

    const vector<Row> &getRows() const {
        return rows;
    }

    void setRows(const vector<Row> &rows) {
        Table::rows = rows;
    }

    const vector<Column> &getCols() const {
        return cols;
    }

    void setCols(const vector<Column> &cols) {
        Table::cols = cols;
    }


};


#endif //SQLENGINE_TABLE_H
