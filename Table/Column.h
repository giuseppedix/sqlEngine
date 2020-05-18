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

public:
    int getMask() const;

    const string &getName() const;

    void setName(const string &name);

    void setMask(int mask);

private:
    string name;
    int mask;
};


#endif //SQLENGINE_COLUMN_H
