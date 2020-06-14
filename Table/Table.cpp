#include "Table.h"
#include "../SqlEngine/SqlEngine.h"
#include "CellFactory.h"
#include <algorithm>

int Table::addRow(vector<string> fields, vector<string> valuesI, vector<string> valuesTxt) {
    vector<string> values;
    int e = 0;
    int f = 0;

    for (int r = 0; r < cols.size(); r++){
        for (int g = 0; g < fields.size(); g++){
            if (!fields[g].compare(cols[r].getName())){
                if (cols[r].getMask() == MASK_TEXT){
                    values.push_back(valuesTxt[e]);
                    e++;
                } else{
                    values.push_back(valuesI[f]);
                    f++;
                }
            }
        }
    }

    if (!checkFieldsColsOrder(fields, cols)) {
        orderFieldsCols(fields, values, cols);
    }

    vector<int> masks = getMasks(fields);
    try {
        if (fields.size() < getnumberNotNull()) {
            throw invalid_argument("ERROR: Not null fields not respect"); //Else if?
        }
        if (fields.size() != values.size()) {
            throw invalid_argument("ERROR: Something is missing");
        }
        if (!checkNotNullinFields(fields)) {
            throw invalid_argument("ERROR: Missing not null parameters");
        }
        if (!checkMasks(getMasks(fields), values)) {
            throw invalid_argument("ERROR: Incorrect types");
        }
        if (getNotNullCols().size() != getnumberNotNull()) {
            throw invalid_argument("ERROR: Something gone wrong");
        }
        for (int i = 0; i < fields.size(); i++) {
            if (!isFieldPresent(fields[i])) {
                throw invalid_argument("ERROR: Invalid fields");
            }
        }
        vector <RowElement*> row;

        for (int r = 0; r < cols.size(); r++){
            int index = -1;
            for (int g = 0; g < fields.size(); g++){
                if (!fields[g].compare(cols[r].getName())){
                    index = g;
                }
            }
            if (index != -1){
                CellFactory cellFactory(masks[index], values[index]);
                row.push_back(cellFactory.getCell());
            } else{
                CellFactory cellFactory(cols[r].getMask(), "-999");
                row.push_back(cellFactory.getCell());
            }
        }

        Row rT;
        rT.setRow(row);
        checkRow(row);

        rows.push_back(rT);
                                                                // indirizzi di memoria allocati in base alla mask
        cout << "test" << endl;

    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }

    return 0;
}

bool Table::isFieldPresent(string field) {
    bool ret = false;
    for (int i = 0; i < cols.size(); i++) {
        if (!cols[i].getName().compare(field)) {
            ret = true;
            break;
        }
    }
    return ret;
}

int Table::getnumberNotNull() {
    int count = 0;
    for (int i = 0; i < cols.size(); i++) {
        if (cols[i].getMask() & MASK_NOTNULL) {
            count++;
        }
    }
    return count;
}

vector<Column> Table::getNotNullCols() {
    vector<Column> notNullCols;
    for (int i = 0; i < cols.size(); i++) {
        if (cols[i].getMask() & MASK_NOTNULL) {
            notNullCols.push_back(cols[i]);
        }
    }
    return notNullCols;
}

bool Table::checkNotNullinFields(vector<string> fields) {
    bool ret = true;
    vector<Column> notNullCols = getNotNullCols();
    for (int i = 0; i < notNullCols.size(); i++) {
        bool found = false;
        for (int j = 0; j < fields.size(); j++) {
            if (!notNullCols[i].getName().compare(fields[j])) {
                found = true;
                break;
            }
        }
        ret &= found;
    }
    return ret;
}

int Table::getTypeFromValue(string value) {

    int type = MASK_TEXT;
    static const regex doubleRegex{R"([+\-]?(?:0|[1-9]\d*)(?:\.\d*)?(?:[eE][+\-]?\d+)?)"};
    static const regex intRegex{R"(\d+)"};
    if (regex_match(value, intRegex)) {
        type = MASK_INT;
        cout << "IsInt" << endl;

    } else if (regex_match(value, doubleRegex)) {
        type = MASK_FLOAT;
        cout << "IsFloat" << endl;

    } else if (value.length() == 1) {
        type = MASK_CHAR;
        cout << "IsChar" << endl;

    }else if (value.length() != 0) {
        type = MASK_TEXT;
        cout << "IsString" << endl;
    } else {
        type = MASK_INVALID;
    }

    return type;
}

vector<int> Table::getMasks(vector<string> fields) {
    vector<int> masks;
    for (int i = 0; i < fields.size(); i++) {
        for (int j = 0; j < cols.size(); j++) {
            if (!fields[i].compare(cols[j].getName())) {
                masks.push_back(cols[j].getMask());
            }
        }
    }
    return masks;
}

bool Table::checkMasks(vector<int> masks, vector<string> values) {
    bool check = false;
    int f = 0;
    for (int i = 0; i < masks.size(); i++) {
        int mskTmp = getTypeFromValue(values[i]) & masks[i];
        if (mskTmp == masks[i] || mskTmp != MASK_NOTNULL) {
            check = true;
        } else {
            check = false;
            f++;
        }
    }
    if (f > 0) {
        check = false;
    }
    return check;
}

void Table::checkRow(vector<RowElement *> row) {
    for (int i = 0; i < cols.size(); i++){
        switch (cols[i].getMask()) {
            case MASK_INT:{
                int vali = ((Cell<int>*) row[i])->getValue();
                cout << "INT VALUE: " << vali << endl;
                break;
            }
            case MASK_FLOAT:{
                float valf = ((Cell<float>*) row[i])->getValue();
                cout << "FLOAT VALUE: " << valf << endl;
                break;
            }
            case MASK_TEXT:{
                string vals = ((Cell<string>*) row[i])->getValue();
                cout << "TEXT VALUE: " << vals << endl;
                break;
            }
            case MASK_TIME:
            case MASK_CHAR:{
                char valc = ((Cell<char>*) row[i])->getValue();
                cout << "CHAR VALUE: " << valc << endl;
                break;
            }
            case MASK_DATE:
            default:
                break;
        }
    }
}
bool Table::checkFieldsColsOrder(vector<string> fields, vector<Column> cols) {
    bool check = true;
    for (int a = 0; a < cols.size() && a < fields.size(); a++) {
        if (fields[a] != cols[a].getName()) {
            check = false;
            break;
        }
    }
    return check;
}

void Table::orderFieldsCols(vector<string> &fields, vector<string> &values, vector<Column> cols) {

    int pos = 0;
    int j = 0;
    int i = 0;

    while (cols[i].getName() != fields[j] && i < cols.size() - 1) {
        while (cols[i].getName() != fields[j] && j < fields.size() - 1) {
            j++;
            if (cols[i].getName() == fields[j]) {
                pos = i;
                swap(fields[j], fields[pos]);
                swap(values[j], values[pos]);
            }
        }
        i++;
        j = 0;
    }
}

void Table::printAll() {
    cout << "-------------------------------------" << endl;
    for (int i=0; i < cols.size(); i++){
        cout << cols[i].getName() << "|";
    }
    cout << endl;
    cout << "-------------------------------------" << endl;

    for (int j = 0; j < getRows().size(); j++){
        vector<RowElement*> rowElements = getRows()[j].getRow();
        for (int a = 0; a < getCols().size(); a++) {
            switch (getCols()[a].getMask()) {
                case MASK_INT:{
                    int vali = ((Cell<int>*) rowElements[a])->getValue();
                    cout << vali;
                    break;
                }
                case MASK_FLOAT:{
                    float valf = ((Cell<float>*) rowElements[a])->getValue();
                    cout << valf;
                    break;
                }
                case MASK_TEXT:{
                    string vals = ((Cell<string>*) rowElements[a])->getValue();
                    cout << vals;
                    break;
                }
                case MASK_TIME:
                case MASK_CHAR:{
                    char valc = ((Cell<char>*) rowElements[a])->getValue();
                    cout << valc;
                    break;
                }

                case MASK_DATE:
                default:
                    break;
            }
            cout << "|";
        }
        cout << endl;
    }
}

void Table::removeAllRows() {
    rows.clear();
}
