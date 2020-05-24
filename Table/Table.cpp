#include "Table.h"
#include "../SqlEngine/SqlEngine.h"

int Table::addRow(vector<string> fields, vector<string> values) {

    bool found = true;
    vector<int> masks;
    try {
        if (fields.size() < getnumberNotNull()) {
            throw invalid_argument("ERROR: Criteria not met");
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
        cout << "IsInt" << endl;

    } else if (regex_match(value, doubleRegex)) {
        cout << "IsFloat" << endl;

    } else {
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
    bool check = true;
    for (int i = 0; i < masks.size(); i++) {
        check &= masks[i] && getTypeFromValue(values[i]);

    }

    return check;

}