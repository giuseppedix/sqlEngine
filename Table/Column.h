#ifndef SQLENGINE_COLUMN_H
#define SQLENGINE_COLUMN_H

#include <vector>
#include <iostream>
#include "../types.h"

using namespace std;

class Column {

public:
    //METHODS
    Column(){};
    int getMask() const;
    const string &getName() const;
    void setName(const string &name);
    void setMask(int mask);

private:
    //ATTRIBUTES
    string name;
    int mask;
};


#endif //SQLENGINE_COLUMN_H
