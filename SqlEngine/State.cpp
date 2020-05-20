#include "State.h"

using namespace std;

string State::getDbName() {
    return dbName;
}

void State::setDbName(const string &dbNameI) {
    dbName.assign(dbNameI);
}

void State::addTable(string nameTable, vector<int>paramsMasks, vector<string> paramsNames) {

    bool found = false;
    Table table;
    int index = 0;
    vector<Column> columns;

    try{
        for(int i = 0; i < tables.size(); i++){
            if(!tables[i].getName().compare(nameTable)){
                found = true;
                index = i;
            }
        }
        if(found){
            throw invalid_argument("ERROR: Table already exists!");
        } else{
            table.setName(nameTable);
            for(int j = 0; j < paramsMasks.size(); j++){
                Column column;
                column.setMask(paramsMasks[j]);
                column.setName(paramsNames[j]);
                columns.push_back(column);
            }
            table.setCols(columns);
            tables.push_back(table);
        }
    }
    catch(invalid_argument &exc){
        cerr << exc.what() << endl;
    }
}
