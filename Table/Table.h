#ifndef SQLENGINE_TABLE_H
#define SQLENGINE_TABLE_H

#include <vector>
#include <iostream>
#include "Row.h"
#include "Column.h"
#include <regex>
using namespace std;

class Table {

public:
    //DEFAULT CONSTRUCTOR
    Table() = default;
    //ATTRIBUTES
    string tableName;
    vector<Row> rows;
    vector<Column> cols;
    //METHODS
    int addRow (vector<string>fields, vector <string> values);
    string getName(){
        return tableName;
    }
    void setName(string name){
        this->tableName = name;
    }
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

private:
    //METHODS
    bool isFieldPresent(string field);
    int getnumberNotNull();
    vector<Column> getNotNullCols();
    bool checkNotNullinFields(vector<string>fields);
    vector<int> getMasks(vector<string> fields);
    bool checkMasks(vector<int> masks, vector<string> values);
    int getTypeFromValue(string value);

    void checkRow(vector<RowElement *> row);
    bool checkFieldsColsOrder (vector<string> fields, vector<Column> cols);
    void orderFieldsCols(vector<string> fields, vector<Column> cols);

};


#endif //SQLENGINE_TABLE_H
