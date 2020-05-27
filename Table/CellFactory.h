#ifndef SQLENGINE_CELLFACTORY_H
#define SQLENGINE_CELLFACTORY_H

#include <vector>
#include <string>
#include "RowElement.h"
#include "Column.h"
#include "Table.h"


using namespace std;

class CellFactory {

public:
    // DISTRUTTORE
    ~CellFactory();
    // CONSTRUCTOR
    CellFactory(int mask, string val);
    RowElement* getCell();
    vector<string> addElements(vector<string> values);

private:
    RowElement *cell;
};


#endif //SQLENGINE_CELLFACTORY_H
