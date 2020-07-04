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

void State::setParamsTable(string nameTable, vector<string> fields, vector<string> values) {

    int index;
    try {
        if (tablePresent(nameTable, index)) {
            tables[index].addRow(fields, values);
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
    try {
        string _filepath = "";
        _filepath = path + getDbName();
        sfile.open(_filepath);
        for (int i = 0; i < tables.size(); i++) {
            sfile << BEGIN << endl;
            sfile << tables[i].getName() << endl;
            if (!sfile.is_open()) {
                throw invalid_argument("ERROR: Cannot open file");
            }
            //write cols name
            for (int j = 0; j < tables[i].getCols().size(); j++) {
                sfile << tables[i].getCols()[j].getName();
                sfile << "|";
            }
            sfile << endl;
            //write cols masks
            for (int j = 0; j < tables[i].getCols().size(); j++) {
                sfile << tables[i].getCols()[j].getMask();
                sfile << "|";
            }
            sfile << endl;
            //write rows
            for (int j = 0; j < tables[i].getRows().size(); j++) {
                vector<RowElement *> rowElements = tables[i].getRows()[j].getRow();
                for (int a = 0; a < tables[i].getCols().size(); a++) {
                    switch (tables[i].getCols()[a].getMask() & 0b111) {
                        case MASK_INT: {
                            int vali = ((Cell<int> *) rowElements[a])->getValue();
                            sfile << vali;
                            break;
                        }
                        case MASK_FLOAT: {
                            float valf = ((Cell<float> *) rowElements[a])->getValue();
                            sfile << valf;
                            break;
                        }
                        case MASK_TEXT: {
                            string vals = ((Cell<string> *) rowElements[a])->getValue();
                            sfile << vals;
                            break;
                        }
                        case MASK_TIME:
                        case MASK_CHAR: {
                            char valc = ((Cell<char> *) rowElements[a])->getValue();
                            sfile << valc;
                            break;
                        }
                        case MASK_DATE:
                        default:
                            break;
                    }
                    sfile << "|";
                }
                sfile << endl;
            }
            sfile << END << endl;
        }
        sfile.close();
    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
        sfile.close();
    }
}

void State::load(string path) {

    ifstream file;
    string filepath = path + "/" + getDbName();
    string rowFile = "";
    try {
        file.open(filepath);
        if (!file.is_open()) {
            throw invalid_argument("ERROR: Cannot open file");
        } else {
            while (getline(file, rowFile)) {
                vector<string> tmp;
                if (!rowFile.compare(BEGIN)) {
                    while (getline(file, rowFile)) {
                        if (!rowFile.compare(END)) {
                            break;
                        } else {
                            tmp.push_back(rowFile);
                        }
                    }
                    Table table;
                    table.setTableName(tmp[0]);
                    vector<Column> columns;
                    vector<string> fields = SqlEngine::splitValueByDelimiter(tmp[1], "|");
                    vector<string> masks = SqlEngine::splitValueByDelimiter(tmp[2], "|");
                    fields.pop_back();
                    masks.pop_back();
                    for (int i = 0; i < fields.size(); i++) {
                        Column column;
                        column.setName(fields[i]);
                        column.setMask(atoi(masks[i].c_str()));
                        columns.push_back(column);
                    }
                    table.setCols(columns);
                    for (int j = 3; j < tmp.size(); j++) {
                        vector<string> valuesI, valuesTxt;
                        vector<string> values = SqlEngine::splitValueByDelimiter(tmp[j], "|");
                        values.pop_back();
                        for (int g = 0; g < values.size(); g++) {
                            if (values[g].compare("") && (atoi(masks[g].c_str()) & MASK_TEXT))
                                valuesTxt.push_back(values[g]);
                            else
                                valuesI.push_back(values[g]);
                        }
                        table.addRow(fields, values);
                    }
                    tables.push_back(table);
                }
            }
        }
        file.close();
    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void State::eraseTable(int index) {
    tables.erase(tables.begin() + index);
}

void State::truncateTable(int index) {
    tables[index].removeAllRows();
}

void State::deleteFrom(int index, string fieldToDelete, string valueToDelete) {
    tables[index].deleteFrom(fieldToDelete, valueToDelete);
}

void State::update(int &index, string &fieldToSet, string &valueToSet, string &valueToWhere, string &fieldToWhere) {
    tables[index].updateRecords(valueToSet, fieldToSet, valueToWhere, fieldToWhere);
}


