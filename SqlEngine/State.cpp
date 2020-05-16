//
// Created by gdisi on 16/05/2020.
//

#include "State.h"
using namespace std;
string State::getDbName() {
    return dbName;
}

void State::setDbName(const string &dbNameI) {
    dbName.assign(dbNameI);
}
