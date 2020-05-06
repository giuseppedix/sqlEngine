//
// Created by gdisi on 06/05/2020.
//

#ifndef SQLENGINE_COLUMN_H
#define SQLENGINE_COLUMN_H
#include <vector>
#include <iostream>
#include "../types.h"

using namespace std;

class Column {
    string name;
    Type type;
    bool isNotNull;
};


#endif //SQLENGINE_COLUMN_H
