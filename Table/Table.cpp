#include "Table.h"
#include "../SqlEngine/SqlEngine.h"
#include "CellFactory.h"

int Table::addRow(vector<string> fields, vector<string> values) {

    bool found = true;
    vector<int> masks = getMasks(fields);
    try {
        if (fields.size() < getnumberNotNull()) {
            throw invalid_argument("ERROR: Not null fields not respect");
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
        if(getNotNullCols().size() != getnumberNotNull()){
            throw invalid_argument("ERROR: Something gone wrong");
        }
        for (int i = 0; i < fields.size(); i++) {
            if (!isFieldPresent(fields[i])) {
                throw invalid_argument("ERROR: Invalid fields");
            }
        }

        vector <RowElement*> row;
        for (int i = 0; i < cols.size(); i++){
            CellFactory cellFactory(masks[i], values[i]);
            row.push_back(cellFactory.getCell());
        }
        Row rT;
        rT.setRow(row);
        checkRow(row);

        rows.push_back(rT);
                                                                // indirizzi di memoria allocati in base alla mask
        cout << "test" << endl;




        //cella per ogni field
        //quindi popolare vector row
        // ordinare i campi in base alle columns
        //row.addElements(values)
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

    } else {
        type = MASK_TEXT;
        cout << "IsString" << endl;
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
        if (masks[i] == getTypeFromValue(values[i])) {
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
                cout << "FLOAT VALUE: " << vals << endl;
                break;
            }
            case MASK_TIME:
            case MASK_CHAR:
            case MASK_DATE:
            default:
                break;
        }
    }
}