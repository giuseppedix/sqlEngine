#ifndef SQLENGINE_TABLE_H
#define SQLENGINE_TABLE_H

#include <vector>
#include <iostream>
#include "Row.h"
#include "Column.h"
#include <regex>
#include <algorithm>
#include "../DateTime/Time.h"
#include "../DateTime/Date.h"

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

    void printAll();
    void printAllOrdered(string mode, string col);
    void printSelect(vector<string> toPrint);
    void printWhere(vector<string> &toPrintWhere, vector<string>split);
    void removeAllRows();
    void deleteFrom(string &fieldToDelete, string &valueToDelete);
    void orderBy();
    bool isFieldPresent(string field);
    void updateRecords(string &valueToSet, string &fieldToSet, string &valueToWhere, string &fieldToWhere);
    void setValue(string &valueToSet, string &fieldToset, string &fieldToWhere, int pos);
    bool checkAutoIncrementParams(vector<int> paramsMask);
    int addRow (vector<string>fields, vector <string> values);

private:
    //METHODS
    int getnumberNotNull();
    vector<int> BubbleSortAsc(vector<Date> elements);
    vector<int> BubbleSortDesc(vector<Date> elements);
    vector<int> BubbleSortAsc(vector<Time> elements);
    vector<int> BubbleSortDesc(vector<Time> elements);
    vector<int> BubbleSortAsc(vector<int> elements);
    vector<int> BubbleSortDesc(vector<int> elements);
    vector<int> BubbleSortAsc(vector<float> elements);
    vector<int> BubbleSortDesc(vector<float> elements);
    vector<int> BubbleSortAsc(vector<string> elements);
    vector<int> BubbleSortDesc(vector<string> elements);
    vector<int> BubbleSortAsc(vector<char> elements);
    vector<int> BubbleSortDesc(vector<char> elements);
    vector<Column> getNotNullCols();
    vector<Column> getAutoIncrementCols();
    int getNumberAutoIncrement();
    bool checkNotNullinFields(vector<string>fields);
    vector<int> getMasks(vector<string> fields);
    bool checkMasks(vector<int> masks, vector<string> values, vector<Column> cols);
    int getTypeFromValue(string value);
    void checkRow(vector<RowElement *> row);
    bool checkFieldsColsOrder (vector<string> fields, vector<Column> cols);
    void orderFieldsCols(vector<string> &fields, vector<string> &values, vector<Column> cols);
    vector<int> getMasksToPrint (vector<string> &toPrint);
    string removeSpace(string input);

    bool checkDuplicate(int i, int mask, string value);

    vector<int> getOrderIndexes(string mode, string col);
};


#endif //SQLENGINE_TABLE_H
