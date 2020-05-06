//
// Created by gdisi on 06/05/2020.
//

#ifndef SQLENGINE_ROW_H
#define SQLENGINE_ROW_H
#include <iostream>
#include <vector>
#include "RowElement.h"

using namespace std;

class Row {
    vector<RowElement*> row;
};


#endif //SQLENGINE_ROW_H
