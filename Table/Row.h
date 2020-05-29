#ifndef SQLENGINE_ROW_H
#define SQLENGINE_ROW_H
#include <iostream>
#include <vector>
#include "RowElement.h"

using namespace std;

class Row {
public:

    void setRow(vector<RowElement*> row){
        this->row = row;
    }

private:
    vector<RowElement*> row;

};


#endif //SQLENGINE_ROW_H
