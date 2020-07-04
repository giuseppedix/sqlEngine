#include "Table.h"
#include "../SqlEngine/SqlEngine.h"
#include "CellFactory.h"
#include <algorithm>


int Table::addRow(vector<string> fields, vector<string> values) {

    for (int i = 0; i < fields.size(); i++) {
        if (!isFieldPresent(fields[i])) {
            throw invalid_argument("ERROR: Invalid fields");
        }
    }

    if (!checkFieldsColsOrder(fields, cols)) {
        orderFieldsCols(fields, values, cols);
    }

    //se scrivo not null auto_incr, posso lo stesso lasciare vuoto.
    vector<int> masks = getMasks(fields);

    try {
        if (fields.size() < getnumberNotNull()) {
            throw invalid_argument("ERROR: Not null fields not respect");
        }
        if (getAutoIncrementCols().size() != getNumberAutoIncrement()) {
            throw invalid_argument("ERROR: Auto Increment cols not respect");
        }

        /*if (fields.size() != values.size()) { // non è corretto perche io posso inserire un numero < di fields ma rispettare cmq i not null
            throw invalid_argument("ERROR: Something is missing");
        }*/
        if (!checkNotNullinFields(fields)) {
            throw invalid_argument("ERROR: Missing not null parameters");
        }
        if (!checkMasks(getMasks(fields), values, cols)) {
            throw invalid_argument("ERROR: Incorrect types");
        }
        if (getNotNullCols().size() != getnumberNotNull()) {
            throw invalid_argument("ERROR: Not Null cols not respect");
        }
        int count = 1;
        vector<RowElement *> row;

        for (int r = 0; r < cols.size(); r++) {
            int index = -1;
            for (int g = 0; g < fields.size(); g++) {
                if (!fields[g].compare(cols[r].getName())) {
                    index = g;
                }
            }
            if (index != -1) {
                if ((masks[index] & MASK_AUTOINCREMENT)  && (masks[index] & MASK_INT)) {
                    if (rows.size()){
                        vector<RowElement *> rowTmps = rows[rows.size() - 1].getRow();
                        count = ((Cell<int> *) rowTmps[r])->getValue();
                        count ++;
                    }
                    values[index] = to_string(count);
                }
                if (masks[index] & MASK_PRIMARYKEY){
                    if (checkDuplicate(r, masks[index], values[index])){
                        throw invalid_argument("ERROR: Duplicate found. Primary key cannot be duplicated");
                    }
                }
                CellFactory cellFactory(masks[index], values[index]);
                row.push_back(cellFactory.getCell());
            } else {
                string value = "-999";

                if ((cols[r].getMask() & MASK_PRIMARYKEY) && !(cols[r].getMask() & MASK_AUTOINCREMENT)){
                    throw invalid_argument("ERROR: Primary key cannot be null");
                }

                if ((cols[r].getMask() & MASK_AUTOINCREMENT) && (cols[r].getMask() & MASK_INT)) {
                    if (rows.size()){
                        vector<RowElement *> rowTmps = rows[rows.size() - 1].getRow();
                        count = ((Cell<int> *) rowTmps[r])->getValue();
                        count ++;
                    }
                    value = to_string(count);
                }
                CellFactory cellFactory(cols[r].getMask(), value);
                row.push_back(cellFactory.getCell());
            }
        }

        Row rT;
        rT.setRow(row);
        checkRow(row);
        rows.push_back(rT);
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
        if ((cols[i].getMask() & MASK_NOTNULL) && !(cols[i].getMask() & MASK_AUTOINCREMENT)) {
            count++;
        }
    }
    return count;
}

vector<Column> Table::getNotNullCols() {

    vector<Column> notNullCols;
    for (int i = 0; i < cols.size(); i++) {
        if ((cols[i].getMask() & MASK_NOTNULL) && !(cols[i].getMask() & MASK_AUTOINCREMENT)) {
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

    int type = MASK_INVALID;
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

    } else if (value.length() != 0) {
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

bool Table::checkMasks(vector<int> masks, vector<string> values, vector<Column> cols) {

    bool check = false;
    int f = 0;
    for (int i = 0; i < masks.size(); i++) {
        int mskTmp = getTypeFromValue(values[i]);
        if (mskTmp == cols[i].getMask() || mskTmp != MASK_NOTNULL) {
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

    for (int i = 0; i < cols.size(); i++) {
        switch (cols[i].getMask() & 0b111) {
            case MASK_INT: {
                int vali = ((Cell<int> *) row[i])->getValue();
                cout << "INT VALUE: " << vali << endl;
                break;
            }
            case MASK_FLOAT: {
                float valf = ((Cell<float> *) row[i])->getValue();
                cout << "FLOAT VALUE: " << valf << endl;
                break;
            }
            case MASK_TEXT: {
                string vals = ((Cell<string> *) row[i])->getValue();
                cout << "TEXT VALUE: " << vals << endl;
                break;
            }
            case MASK_TIME:
            case MASK_CHAR: {
                char valc = ((Cell<char> *) row[i])->getValue();
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

    if (cols[i].getName() == fields[j]) {
        j++;
    }
    while (cols[i].getName() != fields[j] && i < cols.size() - 1) {
        while (cols[i].getName() != fields[j] && j < fields.size() - 1) {
            j++;
            if (cols[i].getName() == fields[j]) {
                pos = i;
                swap(fields[j], fields[pos]);
                swap(values[j], values[i]);
            }
        }
        i++;
        j = 0;
    }
}

void Table::printAll() {

    cout << "-------------------------------------" << endl;
    for (int i = 0; i < cols.size(); i++) {
        cout << cols[i].getName() << "|";
    }
    cout << endl;
    cout << "-------------------------------------" << endl;

    for (int j = 0; j < getRows().size(); j++) {
        vector<RowElement *> rowElements = getRows()[j].getRow();
        for (int a = 0; a < getCols().size(); a++) {
            switch (getCols()[a].getMask() & 0b111) {
                case MASK_INT: {
                    int vali = ((Cell<int> *) rowElements[a])->getValue();
                    cout << vali;
                    break;
                }
                case MASK_FLOAT: {
                    float valf = ((Cell<float> *) rowElements[a])->getValue();
                    cout << valf;
                    break;
                }
                case MASK_TEXT: {
                    string vals = ((Cell<string> *) rowElements[a])->getValue();
                    cout << vals;
                    break;
                }
                case MASK_TIME:
                case MASK_CHAR: {
                    char valc = ((Cell<char> *) rowElements[a])->getValue();
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

void Table::printSelect(vector<string> toPrint) {

    cout << "-------------------------------------" << endl;
    for (int i = 0; i < toPrint.size(); i++) {
        cout << toPrint[i] << "|";
    }
    cout << endl;
    cout << "-------------------------------------" << endl;

    for (int j = 0; j < getRows().size(); j++) {
        for (int i = 0; i < cols.size(); i++) {
            vector<RowElement *> rowElements = getRows()[j].getRow();
            vector<int> masksToPrint = getMasksToPrint(toPrint);
            for (int a = 0; a < toPrint.size(); a++) {
                if (!cols[i].getName().compare(toPrint[a])) {
                    switch (masksToPrint[a] & 0b111) {
                        case MASK_INT: {
                            int vali = ((Cell<int> *) rowElements[i])->getValue();
                            cout << vali;
                            break;
                        }
                        case MASK_FLOAT: {
                            float valf = ((Cell<float> *) rowElements[i])->getValue();
                            cout << valf;
                            break;
                        }
                        case MASK_TEXT: {
                            string vals = ((Cell<string> *) rowElements[i])->getValue();
                            cout << vals;
                            break;
                        }
                        case MASK_TIME:
                        case MASK_CHAR: {
                            char valc = ((Cell<char> *) rowElements[i])->getValue();
                            cout << valc;
                            break;
                        }
                        case MASK_DATE:
                        default:
                            break;
                    }
                    cout << "|";
                }
            }
        }
        cout << endl;
    }
}

vector<int> Table::getMasksToPrint(vector<string> &toPrint) {

    vector<int> masksToPrint;
    for (int i = 0; i < toPrint.size(); i++) {
        for (int j = 0; j < getCols().size(); j++) {
            if (!getCols()[j].getName().compare(toPrint[i])) {
                masksToPrint.push_back(getCols()[j].getMask());
            }
        }
    }
    return masksToPrint;

}

void Table::printWhere(vector<string> &toPrintWhere, vector<string> split) {

    cout << "-------------------------------------" << endl;
    for (int i = 0; i < toPrintWhere.size(); i++) {
        cout << toPrintWhere[i] << "|";
    }
    cout << endl;
    cout << "-------------------------------------" << endl;

    for (int j = 0; j < rows.size(); j++) {
        for (int h = 0; h < cols.size(); h++) {
            vector<RowElement *> rowElements = getRows()[j].getRow();
            vector<int> masksToPrint = getMasksToPrint(toPrintWhere);
            if (!cols[h].getName().compare(split[0])) {
                switch (cols[h].getMask() & 0b111) {
                    case MASK_INT: {
                        int vali = ((Cell<int> *) rowElements[h])->getValue();
                        if (vali == stoi(split[1])) {
                            for (int r = 0; r < toPrintWhere.size(); r++) {
                                for (int t = 0; t < cols.size(); t++) {
                                    if (!toPrintWhere[r].compare(cols[t].getName())) {
                                        int pos = t;
                                        switch (masksToPrint[r] & 0b111) {
                                            case MASK_INT: {
                                                int valin = ((Cell<int> *) rowElements[pos])->getValue();
                                                cout << valin << "|";
                                                break;
                                            }
                                            case MASK_FLOAT: {
                                                float valf = ((Cell<float> *) rowElements[pos])->getValue();
                                                cout << valf << "|";
                                                break;
                                            }
                                            case MASK_CHAR: {
                                                char valc = ((Cell<char> *) rowElements[pos])->getValue();
                                                cout << valc << "|";
                                                break;
                                            }
                                            case MASK_TEXT: {
                                                string vals = ((Cell<string> *) rowElements[pos])->getValue();
                                                cout << vals << "|";
                                                break;
                                            }
                                            default:
                                                break;
                                        }
                                    }
                                }
                            }
                            cout << endl;
                        }
                        break;
                    }
                    case MASK_FLOAT: {
                        float valf = ((Cell<float> *) rowElements[h])->getValue();
                        if (valf == stof(split[1])) {
                            for (int r = 0; r < toPrintWhere.size(); r++) {
                                for (int t = 0; t < cols.size(); t++) {
                                    if (!toPrintWhere[r].compare(cols[t].getName())) {
                                        int pos = t;
                                        switch (masksToPrint[r] & 0b111) {
                                            case MASK_INT: {
                                                int vali = ((Cell<int> *) rowElements[pos])->getValue();
                                                cout << vali << "|";
                                                break;
                                            }
                                            case MASK_FLOAT: {
                                                float valfl = ((Cell<float> *) rowElements[pos])->getValue();
                                                cout << valfl << "|";
                                                break;
                                            }
                                            case MASK_CHAR: {
                                                char valc = ((Cell<char> *) rowElements[pos])->getValue();
                                                cout << valc << "|";
                                                break;
                                            }
                                            case MASK_TEXT: {
                                                string vals = ((Cell<string> *) rowElements[pos])->getValue();
                                                cout << vals << "|";
                                                break;
                                            }
                                            default:
                                                break;
                                        }
                                    }
                                }
                            }
                            cout << endl;
                        }
                        break;
                    }
                    case MASK_TEXT: {
                        string vals = ((Cell<string> *) rowElements[h])->getValue();
                        if (!removeSpace(vals).compare(split[1])) {
                            for (int r = 0; r < toPrintWhere.size(); r++) {
                                for (int t = 0; t < cols.size(); t++) {
                                    if (!toPrintWhere[r].compare(cols[t].getName())) {
                                        int pos = t;
                                        switch (masksToPrint[r] & 0b111) {
                                            case MASK_INT: {
                                                int vali = ((Cell<int> *) rowElements[pos])->getValue();
                                                cout << vali << "|";
                                                break;
                                            }
                                            case MASK_FLOAT: {
                                                float valf = ((Cell<float> *) rowElements[pos])->getValue();
                                                cout << valf << "|";
                                                break;
                                            }
                                            case MASK_CHAR: {
                                                char valc = ((Cell<char> *) rowElements[pos])->getValue();
                                                cout << valc << "|";
                                                break;
                                            }
                                            case MASK_TEXT: {
                                                string valst = ((Cell<string> *) rowElements[pos])->getValue();
                                                cout << valst << "|";
                                                break;
                                            }
                                            default:
                                                break;
                                        }
                                    }
                                }
                            }
                            cout << endl;
                        }
                        break;
                    }
                    case MASK_CHAR: {
                        char valc = ((Cell<char> *) rowElements[h])->getValue();
                        if ((char) split[1].at(0) == valc) {
                            for (int r = 0; r < toPrintWhere.size(); r++) {
                                for (int t = 0; t < cols.size(); t++) {
                                    if (!toPrintWhere[r].compare(cols[t].getName())) {
                                        int pos = t;
                                        switch (masksToPrint[r] & 0b111) {
                                            case MASK_INT: {
                                                int vali = ((Cell<int> *) rowElements[pos])->getValue();
                                                cout << vali << "|";
                                                break;
                                            }
                                            case MASK_FLOAT: {
                                                float valf = ((Cell<float> *) rowElements[pos])->getValue();
                                                cout << valf << "|";
                                                break;
                                            }
                                            case MASK_CHAR: {
                                                char valch = ((Cell<char> *) rowElements[pos])->getValue();
                                                cout << valch << "|";
                                                break;
                                            }
                                            case MASK_TEXT: {
                                                string vals = ((Cell<string> *) rowElements[pos])->getValue();
                                                cout << vals << "|";
                                                break;
                                            }
                                            default:
                                                break;
                                        }
                                    }
                                }
                            }
                            cout << endl;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}

string Table::removeSpace(string input) {

    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

void Table::deleteFrom(string &fieldToDelete, string &valueToDelete) {

    for (int j = 0; j < getRows().size(); j++) {
        vector<RowElement *> rowElements = getRows()[j].getRow();
        for (int a = 0; a < getCols().size(); a++) {
            if (!fieldToDelete.compare(getCols()[a].getName())) {
                switch (getCols()[a].getMask() & 0b111) {
                    case MASK_INT: {
                        int vali = ((Cell<int> *) rowElements[a])->getValue();
                        if (vali == stoi(valueToDelete)) {
                            rows.erase(rows.begin() + j);
                        }
                        break;
                    }
                    case MASK_FLOAT: {
                        float valf = ((Cell<float> *) rowElements[a])->getValue();
                        if (valf == stof(valueToDelete)) {
                            rows.erase(rows.begin() + j);
                        }
                        break;
                    }
                    case MASK_TEXT: {
                        string vals = ((Cell<string> *) rowElements[a])->getValue();
                        if (!removeSpace(vals).compare(valueToDelete)) {
                            rows.erase(rows.begin() + j);
                        }
                        break;
                    }
                    case MASK_TIME:
                    case MASK_CHAR: {
                        char valc = ((Cell<char> *) rowElements[a])->getValue();
                        if (valc == valueToDelete.at(0)) {
                            rows.erase(rows.begin() + j);
                        }
                        break;
                    }
                    case MASK_DATE:
                    default:
                        break;
                }
            }
        }
    }
}

vector<Column> Table::getAutoIncrementCols() {

    vector<Column> autoIncrementCols;
    for (int i = 0; i < cols.size(); i++) {
        if (cols[i].getMask() & MASK_AUTOINCREMENT) {
            autoIncrementCols.push_back(cols[i]);
        }
    }
    return autoIncrementCols;

}

int Table::getNumberAutoIncrement() {

    int count = 0;
    for (int i = 0; i < cols.size(); i++) {
        if (cols[i].getMask() & MASK_AUTOINCREMENT) {
            count++;
        }
    }
    return count;

}

void Table::updateRecords(string &valueToSet, string &fieldToSet, string &valueToWhere, string &fieldToWhere) {

    for (int j = 0; j < rows.size(); j++) {
        vector<RowElement *> rowElements = rows[j].getRow();
        for (int a = 0; a < cols.size(); a++) {
            if (!fieldToWhere.compare(getCols()[a].getName())) {
                switch (cols[a].getMask() & 0b111) {
                    case MASK_INT: {
                        int vali = ((Cell<int> *) rowElements[a])->getValue();
                        if (vali == stoi(valueToWhere)) {
                            int pos = 0;
                            pos = j;
                            setValue(valueToSet, fieldToSet, fieldToWhere, pos);
                        }
                        break;
                    }
                    case MASK_FLOAT: {
                        float valf = ((Cell<float> *) rowElements[a])->getValue();
                        if (valf == stof(valueToWhere)) {
                            int pos = 0;
                            pos = j;
                            setValue(valueToSet, fieldToSet, fieldToWhere, pos);
                        }
                        break;
                    }
                    case MASK_TEXT: {
                        string vals = ((Cell<string> *) rowElements[a])->getValue();
                        if (!removeSpace(vals).compare(valueToWhere)) {
                            int pos = 0;
                            pos = j;
                            setValue(valueToSet, fieldToSet, fieldToWhere, pos);
                        }
                        break;
                    }
                    case MASK_TIME:
                    case MASK_CHAR: {
                        char valc = ((Cell<char> *) rowElements[a])->getValue();
                        if (valueToWhere.at((0) == valc)) {
                            int pos = 0;
                            pos = j;
                            setValue(valueToSet, fieldToSet, fieldToWhere, pos);
                        }
                        break;
                    }
                    case MASK_DATE:
                    default:
                        break;
                }
            }
        }
    }
}

void Table::setValue(string &valueToSet, string &fieldToset, string &fieldToWhere, int pos) {

    try {
        vector<RowElement *> rowElements = rows[pos].getRow();
        for (int a = 0; a < cols.size(); a++) {
            if (!fieldToset.compare(cols[a].getName())) {
                switch (cols[a].getMask()) {
                    case MASK_INT: {
                        if (getTypeFromValue(valueToSet) != MASK_INT) {
                            throw invalid_argument("ERROR: Set Value doesn't respect Type");
                        }
                        ((Cell<int> *) rowElements[a])->setValue(stoi(valueToSet));
                        break;
                    }
                    case MASK_FLOAT: {
                        if (getTypeFromValue(valueToSet) != MASK_FLOAT) {
                            throw invalid_argument("ERROR: Set Value doesn't respect Type");
                        }
                        ((Cell<float> *) rowElements[a])->setValue(stof(valueToSet));
                        break;
                    }
                    case MASK_TEXT: {
                        if (getTypeFromValue(valueToSet) != MASK_TEXT) {
                            throw invalid_argument("ERROR: Set Value doesn't respect Type");
                        }
                        ((Cell<string> *) rowElements[a])->setValue(valueToSet);
                        break;
                    }
                    case MASK_TIME:
                    case MASK_CHAR: {
                        if (getTypeFromValue(valueToSet) != MASK_CHAR) {
                            throw invalid_argument("ERROR: Set Value doesn't respect Type");
                        }
                        ((Cell<char> *) rowElements[a])->setValue(valueToSet.at(0));
                        break;
                    }
                    case MASK_DATE:
                    default:
                        break;
                }
            }
        }
    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }

}

void Table::orderBy() {

}

bool Table::checkAutoIncrementParams(vector<int> paramsMask) {

    bool autoIncrementNotRespect = false;
    for (int j = 0; j < paramsMask.size(); j++) {
        if (paramsMask[j] & MASK_AUTOINCREMENT) {
            if (paramsMask[j] & MASK_DATE ^ paramsMask[j] & MASK_TIME ^ paramsMask[j] & MASK_CHAR ^
                paramsMask[j] & MASK_TEXT ^ paramsMask[j] & MASK_FLOAT) {
                autoIncrementNotRespect = true;
            }
        }
    }
    return autoIncrementNotRespect;
}

bool Table::checkDuplicate(int i, int mask, string value) {
    bool retvalue = false;

    for (int a = 0; a < rows.size(); a++){
        vector<RowElement *> rowTmps = rows[a].getRow();
        switch (mask & 0b111) {
            case MASK_INT: {
                int vali = ((Cell<int> *) rowTmps[i])->getValue();
                if (vali == stoi(value)){
                    retvalue = true;
                }
                break;
            }
            case MASK_FLOAT: {
                float valf = ((Cell<float> *) rowTmps[i])->getValue();
                if (valf == stof(value)){
                    retvalue = true;
                }
                break;
            }
            case MASK_TEXT: {
                string vals = ((Cell<string> *) rowTmps[i])->getValue();
                if (!vals.compare(value)){
                    retvalue = true;
                }
                break;
            }
            case MASK_TIME:
            case MASK_CHAR: {
                char valc = ((Cell<char> *) rowTmps[i])->getValue();
                if (valc == value.at(0)){
                    retvalue = true;
                }
                break;
            }

            case MASK_DATE:
            default:
                break;
        }
    }
    return retvalue;
}

void Table::printAllOrdered(string mode, string col) {

    vector<int> indexes = getOrderIndexes(mode, col);

    cout << "-------------------------------------" << endl;
    for (int i = 0; i < cols.size(); i++) {
        cout << cols[i].getName() << "|";
    }
    cout << endl;
    cout << "-------------------------------------" << endl;

    for (int j = 0; j < indexes.size(); j++) {
        vector<RowElement *> rowElements = rows[indexes[j]].getRow();
        for (int a = 0; a < getCols().size(); a++) {
            switch (getCols()[a].getMask() & 0b111) {
                case MASK_INT: {
                    int vali = ((Cell<int> *) rowElements[a])->getValue();
                    cout << vali;
                    break;
                }
                case MASK_FLOAT: {
                    float valf = ((Cell<float> *) rowElements[a])->getValue();
                    cout << valf;
                    break;
                }
                case MASK_TEXT: {
                    string vals = ((Cell<string> *) rowElements[a])->getValue();
                    cout << vals;
                    break;
                }
                case MASK_TIME:
                case MASK_CHAR: {
                    char valc = ((Cell<char> *) rowElements[a])->getValue();
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

vector<int> Table::getOrderIndexes(string mode, string col) {
    //Only for int
    int cI = 0;
    int mask = 0;
    vector<int> ordIdx;

    for (int c=0; c<cols.size(); c++){
        if (!cols[c].getName().compare(col)){
            cI = c;
            mask = cols[c].getMask();
            break;
        }
    }

    switch (mask & 0b111) {
        case MASK_INT:{
            vector<int> valuesToOrder;
            for (int i = 0; i < rows.size(); i++){
                vector<RowElement *> rowTmps = rows[i].getRow();
                valuesToOrder.push_back( ((Cell<int> *) rowTmps[cI])->getValue() );
            }

            if (!mode.compare("asc"))
                ordIdx = BubbleSortAsc(valuesToOrder);
            else if (!mode.compare("desc"))
                ordIdx = BubbleSortDesc(valuesToOrder);
            break;
        }

        case MASK_FLOAT:{
            vector<float> valuesToOrder;
            for (int i = 0; i < rows.size(); i++){
                vector<RowElement *> rowTmps = rows[i].getRow();
                valuesToOrder.push_back( ((Cell<float> *) rowTmps[cI])->getValue() );
            }

            if (!mode.compare("asc"))
                ordIdx = BubbleSortAsc(valuesToOrder);
            else if (!mode.compare("desc"))
                ordIdx = BubbleSortDesc(valuesToOrder);
            break;
        }

        case MASK_TEXT:{
            vector<string> valuesToOrder;
            for (int i = 0; i < rows.size(); i++){
                vector<RowElement *> rowTmps = rows[i].getRow();
                valuesToOrder.push_back( ((Cell<string> *) rowTmps[cI])->getValue() );
            }

            if (!mode.compare("asc"))
                ordIdx = BubbleSortAsc(valuesToOrder);
            else if (!mode.compare("desc"))
                ordIdx = BubbleSortDesc(valuesToOrder);
            break;
        }
        case MASK_CHAR:{
            vector<char> valuesToOrder;
            for (int i = 0; i < rows.size(); i++){
                vector<RowElement *> rowTmps = rows[i].getRow();
                valuesToOrder.push_back( ((Cell<char> *) rowTmps[cI])->getValue() );
            }

            if (!mode.compare("asc"))
                ordIdx = BubbleSortAsc(valuesToOrder);
            else if (!mode.compare("desc"))
                ordIdx = BubbleSortDesc(valuesToOrder);
            break;
        }
    }
    return ordIdx;
}
vector<int> Table::BubbleSortAsc(vector<string> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }

    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] > elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}

vector<int> Table::BubbleSortDesc(vector<string> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }
    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] < elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}

vector<int> Table::BubbleSortDesc(vector<char> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }
    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] < elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}
vector<int> Table::BubbleSortAsc(vector<char> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }

    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] > elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}



vector<int> Table::BubbleSortAsc(vector<float> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }

    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] > elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}

vector<int> Table::BubbleSortDesc(vector<float> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }
    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] < elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}

vector<int> Table::BubbleSortAsc(vector<int> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }

    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] > elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}

vector<int> Table::BubbleSortDesc(vector<int> elements) {
    vector<int> indexes;
    for (int i = 0; i < elements.size(); i++){
        indexes.push_back(i);
    }
    for (int j = elements.size() - 1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (elements[i] < elements[i + 1]) {
                swap(elements[i], elements[i + 1]);
                swap(indexes[i], indexes[i + 1]);
            }
        }
    }
    return indexes;
}

