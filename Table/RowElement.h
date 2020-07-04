#ifndef SQLENGINE_ROWELEMENT_H
#define SQLENGINE_ROWELEMENT_H
#include <iostream>
#include <vector>
#include "../types.h"

using namespace std;

class RowElement{

public:
    string columnName;
    Type type;
    bool isNotNull;
    bool isAutoIncrement;
    void addElements(vector<string> values);

};

template <class T>
class Cell: public RowElement {
public:
    T value;

    Cell(T value){
        this->value = value;
    }

    T getValue(){
        return value;
    }
    void setValue(T value){
        this->value = value;
    }
};




#endif //SQLENGINE_ROWELEMENT_H
