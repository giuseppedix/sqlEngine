//
// Created by gdisi on 06/05/2020.
//

#ifndef SQLENGINE_ROWELEMENT_H
#define SQLENGINE_ROWELEMENT_H
#include <iostream>
#include <vector>
#include "../types.h"

using namespace std;

class RowElement{
    string columnName;
    Type type;
    bool isNotNull;
    bool isAutoIncrement;
};

template <class T>
class Cell: public RowElement {
    T value;
};


#endif //SQLENGINE_ROWELEMENT_H
