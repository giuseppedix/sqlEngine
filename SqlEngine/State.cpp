#include "State.h"
#include "SqlEngine.h"

using namespace std;

string State::getDbName() {
    return dbName;
}

void State::setDbName(const string &dbNameI) {
    dbName.assign(dbNameI);
}

void State::addTable(string nameTable, vector<int> paramsMasks, vector<string> paramsNames) {

    bool found = false;
    Table table;
    int index = 0;
    vector<Column> columns;

    try {
        if (tablePresent(nameTable, index)) {
            throw invalid_argument("ERROR: Table already exists!");
        } else {
            table.setName(nameTable);
            for (int j = 0; j < paramsMasks.size(); j++) {
                Column column;
                column.setMask(paramsMasks[j]);
                column.setName(paramsNames[j]);
                columns.push_back(column);
            }
            table.setCols(columns);
            tables.push_back(table);
        }
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

bool State::tablePresent(string nametable, int &index) {
    bool found = false;
    for (int i = 0; i < tables.size(); i++) {
        if (!tables[i].getName().compare(nametable)) {
            found = true;
            index = i;
        }
    }
    return found;
}

void State::setParamsTable(string nameTable, vector<string> fields, vector<string> values, vector<string> valuesTxt) {
    int index;
    try {

        if (tablePresent(nameTable, index)) {
            tables[index].addRow(fields, values, valuesTxt);
        } else {
            throw invalid_argument("ERROR: Table not present");
        }
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void State::saveStateOnFile(string filepath) {
    string path = filepath + "/";
    ofstream sfile;
    try{
        string _filepath = "";
        for (int i = 0; i < tables.size(); i++){
            _filepath = path + tables[i].getName();
            sfile.open(_filepath);
            if (!sfile.is_open()){
                invalid_argument("ERROR");
            }
            //write cols name
            for (int j = 0; j < tables[i].getCols().size(); j++){
                sfile << tables[i].getCols()[j].getName();
                sfile << ";";
            }
            sfile << "\n";
            //write cols masks
            for (int j = 0; j < tables[i].getCols().size(); j++){
                sfile << tables[i].getCols()[j].getMask();
                sfile << ";";
            }
            sfile << "\n";
            //write rows
            for (int j = 0; j < tables[i].getRows().size(); j++){
                vector<RowElement*> rowElements = tables[i].getRows()[j].getRow();
                for (int a = 0; a < tables[i].getCols().size(); a++) {
                    switch (tables[i].getCols()[a].getMask()) {
                        case MASK_INT:{
                            int vali = ((Cell<int>*) rowElements[a])->getValue();
                            sfile << vali;
                            break;
                        }
                        case MASK_FLOAT:{
                            float valf = ((Cell<float>*) rowElements[a])->getValue();
                            sfile << valf;
                            break;
                        }
                        case MASK_TEXT:{
                            string vals = ((Cell<string>*) rowElements[a])->getValue();
                            sfile << vals;
                            break;
                        }
                        case MASK_TIME:
                        case MASK_CHAR:
                        case MASK_DATE:
                        default:
                            break;
                    }
                    sfile << ";";
                }
                sfile << endl;
            }

            sfile.close();
        }
    } catch (invalid_argument &exc) {
        cerr << exc.what();
        sfile.close();
    }

}