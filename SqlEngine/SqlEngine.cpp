#include "SqlEngine.h"
#include "../Table/CellFactory.h"
#include <string>

SqlEngine::~SqlEngine() {
    state.saveStateOnFile();
}

int SqlEngine::execute(const string &command) {

    string command_down;
    int ret = 0;

    try {
        command_down.assign(command);
        transform(command_down.begin(), command_down.end(), command_down.begin(), ::tolower);
        switch (getCommand((command_down))) {
            case CREATE_TABLE:
                try {
                    executeCreateTable(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case DROP_TABLE:
                try {
                    executeDropTable(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case INSERT_INTO:
                try {
                    executeInsertInto(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case DELETE_FROM:
                try {
                    executeDeleteFrom(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case TRUNCATE_TABLE:
                try {
                    executeTruncateTable(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case UPDATE:
                try {
                    executeUpdate(command_down);
                }
                catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case ORDER_BY:
                try {
                    //executeOrderBy(command_down);
                }
                catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case SELECT:
                try {
                    executeSelect(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            default:
                throw invalid_argument("ERROR: command not valid");
        }

    }
    catch (invalid_argument &exec) {
        cerr << exec.what() << endl;
    }
    return ret;
}

Command SqlEngine::getCommand(string command) {

    string out_str;
    Command command_enum;
    out_str = removeSpace(command);

    if (out_str.find(CREATE_TABLE_D) != string::npos) {
        command_enum = CREATE_TABLE;

    } else if (out_str.find(DROP_TABLE_D) != string::npos) {
        command_enum = DROP_TABLE;

    } else if (out_str.find(INSERT_INTO_D) != string::npos) {
        command_enum = INSERT_INTO;

    } else if (out_str.find(DELETE_FROM_D) != string::npos) {
        command_enum = DELETE_FROM;

    } else if (out_str.find(TRUNCATE_TABLE_D) != string::npos) {
        command_enum = TRUNCATE_TABLE;

    } else if (out_str.find(UPDATE_D) != string::npos) {
        command_enum = UPDATE;

    } else if (out_str.find(SELECT_D) != string::npos) {
        command_enum = SELECT;

    } else if (out_str.find(ORDER_BY_D) != string::npos) {
        command_enum = ORDER_BY;

    } else {
        command_enum = NOT_VALID;
    }
    return command_enum;
}

State SqlEngine::getState() const {
    return state;
}

int SqlEngine::loadState(const string &dbName) {

    state.setDbName(dbName);
    int result = 0;
    try {
        state.load();
    } catch (invalid_argument &exec) {
        cerr << exec.what() << endl;
    }
    return result;
}

string SqlEngine::removeSpace(string input) {

    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

void SqlEngine::executeCreateTable(string command) {

    try {
        string out_str;
        out_str = removeSpace(command);
        size_t i = out_str.find(CREATE_TABLE_D);

        if (i != 0) {
            throw invalid_argument("ERROR: command not valid");
        }

        removeSubstrs(out_str, CREATE_TABLE_D);
        string nameTable = findNameTable(out_str);

        if (nameTable == "")
            throw invalid_argument("ERROR: Table name not defined");

        size_t in = out_str.find('(');
        out_str.erase(0, in);
        cout << "Create Table: " << nameTable << endl;
        cout << "Parameters: ";

        vector<string> params = getParams(out_str);
        removeSubstrs(params.back(), ")");
        vector<int> paramsMasks;
        vector<string> paramsNames;
        string pKey = getPkey(params);
        if (pKey.compare("")) {
            bool valid = checkPkeyPos(params);
            if (!valid){
                throw invalid_argument("ERROR: Wrong Primary Key position");
            }
        }
        for (int i = 0; i < params.size(); i++) {
            cout << params[i] << ", ";
            int paramMask = MASK_INVALID;
            string paramName = "";
            getParamInfo(params[i], paramMask, paramName);
            if (!paramName.compare(pKey)){
                paramMask = paramMask | MASK_PRIMARYKEY;
            }
            if (params[i].find("primarykey") == string::npos){
                paramsMasks.push_back(paramMask);
                paramsNames.push_back(paramName);
            }
        }

        if (checkColisNotKeyWord(paramsNames)) {
            throw invalid_argument("ERROR: Name Col cannot be KeyWord");
        }

        if (paramsNames[i].empty()) {
            throw invalid_argument("ERROR: Cannot create table, cols number must be at least one");
        }

        for (int a = 0; a < paramsMasks.size(); a++) {
            if (paramsMasks[a] == MASK_INVALID) {
                throw invalid_argument("ERROR: Invalid Mask found");
            }
        }

        Table table;
        if (table.checkAutoIncrementParams(paramsMasks)) {
            throw invalid_argument("ERROR: Auto_Increment Value must be INT");
        }

        state.addTable(nameTable, paramsMasks, paramsNames);
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void SqlEngine::removeSubstrs(string &s, string p) {

    size_t n = p.length();
    for (size_t i = s.find(p); i != string::npos; i = s.find(p))
        s.erase(i, n);
}

string SqlEngine::findNameTable(string c) {

    string nameTable = "";
    string ch = "(";
    size_t st = c.find(ch);
    if (st != string::npos) {
        int len = c.length() - st;
        c.erase(st, len);
        nameTable = c;
    }
    return nameTable;
}

vector<string> SqlEngine::getParams(string c) {

    removeSubstrs(c, "(");
    removeSubstrs(c, ");");
    return vector<string>(splitValueByDelimiter(c, ","));
}

vector<string> SqlEngine::getValueInQuote(string s) {

    string tmp;
    vector<string> ret;
    int count = 0;
    string delimiter = "\"";
    size_t pos = 0;
    while (((pos = s.find(delimiter)) != string::npos)) {
        count++;
        if (count % 2) {
            s.erase(0, pos + delimiter.length());
        } else {
            ret.push_back(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }
    }
    return ret;
}

vector<string> SqlEngine::splitValueByDelimiter(string s, string delimiter) {

    vector<string> ret;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        ret.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    ret.push_back(s);
    return ret;
}

void SqlEngine::getParamInfo(const string &s, int &paramMask, string &name) {

    int i = 0;
    int j = 0;
    int q = 0;

    try {
        if (s.find("int") != string::npos) {
            i = s.find("int");
            paramMask |= MASK_INT;
        } else if (s.find("float") != string::npos) {
            i = s.find("float");
            paramMask |= MASK_FLOAT;
        } else if (s.find("text") != string::npos) {
            i = s.find("text");
            paramMask |= MASK_TEXT;
        } else if (s.find("date") != string::npos) {
            i = s.find("date");
            paramMask |= MASK_DATE;
        } else if (s.find("time") != string::npos) {
            i = s.find("time");
            paramMask |= MASK_TIME;
        } else if (s.find("char") != string::npos) {
            i = s.find("char");
            paramMask |= MASK_CHAR;
        }

        string str;
        str.assign(s);
        str.erase(i, str.length() - i);
        name = str;
        cout << name << endl;
        if (s.find("notnull") != string::npos) {
            j = s.find("notnull");
            paramMask |= MASK_NOTNULL;
        }
        if (s.find("auto_increment") != string::npos) {
            q = s.find("auto_increment");
            paramMask |= MASK_AUTOINCREMENT;
        }

        if ((j != 0) && (q != 0)) {
            if (((j > q) || (i > j) || (q < i))) {
                throw invalid_argument("ERROR: Wrong order parameters");
            }
        } else if (q == 0 && j != 0) {
            if (i > j) {
                throw invalid_argument("ERROR: Wrong order parameters");
            }
        } else if (j == 0 && q != 0) {
            if (q < i) {
                throw invalid_argument("ERROR: Wrong order parameters");
            }
        }

        if (((j > q) || (i > j) || (q < i)) && ((j != 0) && (q != 0))) {
            throw invalid_argument("ERROR: Wrong order parameters");
        }
        bitset<8> x(paramMask);
        cout << x << endl;
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void SqlEngine::executeInsertInto(string command) {

    try {
        string field, value, nameTable, out_str;
        int a = 0;
        int b = 0;

        out_str = removeSpace(command);
        a = out_str.find(INSERT_INTO_D);
        b = out_str.find("values");
        if (b < a) {
            throw invalid_argument("ERROR: Wrong command order");
        }
        removeSubstrs(out_str, INSERT_INTO_D);
        removeSubstrs(out_str, "values");
        nameTable = findNameTable(out_str);
        if (nameTable == "") {
            throw invalid_argument("ERROR: Name Table not defined");
        }
        size_t in = out_str.find('(');
        out_str.erase(0, in);

        vector<string> args = splitValueByDelimiter(out_str, ")(");
        vector<string> fields = getParams(args[0]);
        vector<string> values = splitPreservingArgsInQuote(args[1]);

        state.setParamsTable(nameTable, fields, values);
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void SqlEngine::executeSelect(string &cmd) {

    try {
        string out_str = removeSpace(cmd);
        size_t f = out_str.find(SELECTALLFROM_D);
        if (out_str.find(SELECTALLFROM_D) != string::npos) {
            if (out_str.find(ORDER_BY_D) != string::npos) {
                string tableName;
                tableName.assign(out_str);
                removeSubstrs(tableName, SELECTALLFROM_D);
                string fieldOrder = out_str.substr(out_str.find(ORDER_BY_D), out_str.find(';'));
                removeSubstrs(tableName, fieldOrder);
                if (tableName == "") {
                    throw invalid_argument("ERROR: Table name not defined");
                }
                int index;
                if (!state.tablePresent(tableName, index)) {
                    throw invalid_argument("ERROR: Name Table doesn't exist");
                }
                vector<string> ret = getOrderByInfo(cmd, tableName);
                string mode = ret[0];
                string col = ret[1];
                for (int i = 0; i < state.getTables().size(); i++) {
                    if (!state.getTables()[i].getName().compare(tableName)) {
                        state.getTables()[i].printAllOrdered(mode, col);
                        break;
                    }
                }
            } else {
                string tableName;
                tableName.assign(out_str);
                removeSubstrs(tableName, SELECTALLFROM_D);
                removeSubstrs(tableName, ";");
                size_t s = out_str.find(tableName);
                if (tableName == "") {
                    throw invalid_argument("ERROR: Table name not defined");
                }
                int index;
                if (!state.tablePresent(tableName, index)) {
                    throw invalid_argument("ERROR: Name Table doesn't exist");
                }
                if (s < f) {
                    throw invalid_argument("ERROR: Wrong command order");
                }
                for (int i = 0; i < state.getTables().size(); i++) {
                    if (!state.getTables()[i].getName().compare(tableName)) {
                        state.getTables()[i].printAll();
                        break;
                    }
                }
            }
        } else {
            if (out_str.find(SELECT_D) != string::npos && out_str.find("from") != string::npos &&
                out_str.find("where") != string::npos == 0) {
                int l = out_str.find("from");
                int s = out_str.find(SELECT_D);
                if (l < s) {
                    throw invalid_argument("ERROR: Wrong command order");
                }
                string tableName;
                out_str.erase(0, l);
                tableName.assign(out_str);
                removeSubstrs(tableName, "from");
                removeSubstrs(tableName, ";");
                if (tableName == "") {
                    throw invalid_argument("ERROR: Table Name not defined");
                }
                int m = removeSpace(cmd).find(tableName);
                if (m < s || m < l) {
                    throw invalid_argument("ERROR: Wrong command order");
                }
                int index;
                if (!state.tablePresent(tableName, index)) {
                    throw invalid_argument("ERROR: Name Table doesn't exist");
                }
                string str = removeSpace(cmd);
                removeSubstrs(str, "from");
                removeSubstrs(str, SELECT_D);
                removeSubstrs(str, tableName);
                removeSubstrs(str, ";");
                vector<string> fieldsToPrint = splitValueByDelimiter(str, ",");
                for (int b = 0; b < state.getTables().size(); b++) {
                    if (!state.getTables()[b].getName().compare(tableName)) {
                        vector<string> toPrint = checkFieldsToPrint(fieldsToPrint, tableName, index);
                        state.getTables()[b].printSelect(toPrint);
                        break;
                    }
                }
            } else {
                if (out_str.find(SELECT_D) != string::npos && out_str.find("from") != string::npos &&
                    out_str.find("where") != string::npos) {
                    int g = out_str.find("from");
                    int s = out_str.find(SELECT_D);
                    if (g < s) { // mettere tutte le condizioni
                        throw invalid_argument("ERROR: Wrong command order");
                    }
                    string tableName;
                    out_str.erase(0, g);
                    removeSubstrs(out_str, "from");
                    tableName = out_str.erase(out_str.find("where"), out_str.find(';') + 1);
                    if (tableName == "") {
                        throw invalid_argument("ERROR: Table Name not defined");
                    }
                    int index;
                    if (!state.tablePresent(tableName, index)) {
                        throw invalid_argument("ERROR: Name Table doesn't exist");
                    }
                    vector<string> quoteargs = getValueInQuote(cmd);
                    string str = removeSpace(cmd);
                    string fieldWhere;
                    fieldWhere = str.erase(0, str.find("where"));
                    removeSubstrs(fieldWhere, "where");
                    string st = removeSpace(cmd);
                    removeSubstrs(st, "from");
                    removeSubstrs(st, SELECT_D);
                    removeSubstrs(st, tableName);
                    removeSubstrs(st, fieldWhere);
                    removeSubstrs(st, "where");
                    vector<string> fieldsToPrintWhere = splitValueByDelimiter(st, ",");
                    vector<string> split = splitWhereField(fieldWhere);
                    split[1] = removeQuotes(split[1]);
                    if (!checkWhereField(split, index, tableName)) {
                        throw invalid_argument("ERROR: Invalid Where Field");
                    }
                    for (int l = 0; l < state.getTables().size(); l++) {
                        if (!state.getTables()[l].getName().compare(tableName)) {
                            vector<string> toPrintWhere = checkFieldsToPrint(fieldsToPrintWhere, tableName, index);
                            if (!checkWhereValue(split, index, tableName)) {
                                throw invalid_argument("ERROR: Where Value doesn't exist");
                            }
                            state.getTables()[l].printWhere(toPrintWhere, split);
                            break;
                        }
                    }
                }
            }
        }
    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void SqlEngine::executeDropTable(string cmd) {

    string out_str = removeSpace(cmd);
    string tableName;
    int h = 0;
    int k = 0;

    tableName.assign(out_str);
    removeSubstrs(tableName, DROP_TABLE_D);
    removeSubstrs(tableName, ";");

    try {
        if (tableName == "") {
            throw invalid_argument("ERROR: Name Table not defined");
        }

        int index;
        if (!state.tablePresent(tableName, index)) {
            throw invalid_argument("ERROR: Name Table doesn't exist");
        }

        h = out_str.find(DROP_TABLE_D);
        k = out_str.find(tableName);
        if (h > k) {
            throw invalid_argument("ERROR: Wrong order parameters");
        }
        for (int i = 0; i < state.getTables().size(); i++) {
            if (!state.getTables()[i].getName().compare(tableName)) {
                state.eraseTable(i);
                break;
            }
        }
    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void SqlEngine::executeTruncateTable(string cmd) {

    string out_str = removeSpace(cmd);
    string tableName;
    int h = 0;
    int k = 0;
    int a = 0;

    tableName.assign(out_str);
    removeSubstrs(tableName, TRUNCATE_TABLE_D);
    removeSubstrs(tableName, ";");

    try {
        if (tableName == "") {
            throw invalid_argument("ERROR: Name Table not defined");
        }

        int index;
        if (!state.tablePresent(tableName, index)) {
            throw invalid_argument("ERROR: Name Table doesn't exist");
        }

        h = out_str.find(TRUNCATE_TABLE_D);
        k = out_str.find(tableName);
        if (h > k) {
            throw invalid_argument("ERROR: Wrong order parameters");
        }

        for (int i = 0; i < state.getTables().size(); i++) {
            if (!state.getTables()[i].getName().compare(tableName)) {
                state.truncateTable(i);
                break;
            }
        }
    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

vector<string> SqlEngine::checkFieldsToPrint(vector<string> &fieldsToPrint, string tableName, int index) {

    vector<string> toPrint;
    if (!state.getTables()[index].getName().compare(tableName)) {
        vector<Column> cols = state.getTables()[index].getCols();
        for (int a = 0; a < cols.size(); a++) {
            for (int b = 0; b < fieldsToPrint.size(); b++) {
                if (!cols[a].getName().compare(fieldsToPrint[b])) {
                    toPrint.push_back(fieldsToPrint[b]);
                }
            }
        }
    }
    return toPrint;
}

bool SqlEngine::checkWhereField(vector<string> split, int index, string tableName) {

    bool check = false;
    if (!state.getTables()[index].getName().compare(tableName)) {
        vector<Column> cols = state.getTables()[index].getCols();
        for (int a = 0; a < cols.size(); a++) {
            if (!cols[a].getName().compare(split[0])) {
                check = true;
            }
        }
    }
    return check;
}

vector<string> SqlEngine::splitWhereField(string fieldWhere) {

    vector<string> split = splitValueByDelimiter(fieldWhere, "=");
    removeSubstrs(split[1], ";");
    return split;
}

bool SqlEngine::checkWhereValue(vector<string> split, int index, string tableName) {

    bool check = false;
    if (!state.getTables()[index].getName().compare(tableName)) {
        vector<Column> cols = state.getTables()[index].getCols();
        for (int a = 0; a < cols.size(); a++) {
            if (!cols[a].getName().compare(split[0])) {
                for (int j = 0; j < state.getTables()[index].getRows().size(); j++) {
                    vector<RowElement *> rowElements = state.getTables()[index].getRows()[j].getRow();
                    switch (state.getTables()[index].getCols()[a].getMask() & 0b111) {
                        case MASK_INT: {
                            int vali = ((Cell<int> *) rowElements[a])->getValue();
                            if (vali == stoi(split[1])) {
                                check = true;
                            }
                            break;
                        }
                        case MASK_FLOAT: {
                            float valf = ((Cell<float> *) rowElements[a])->getValue();
                            if (valf == stof(split[1])) {
                                check = true;
                            }
                            break;
                        }
                        case MASK_TEXT: {
                            string vals = ((Cell<string> *) rowElements[a])->getValue();
                            if (!removeSpace(vals).compare(removeQuotes(split[1]))) {
                                check = true;
                            }
                            break;
                        }
                        case MASK_TIME:
                        case MASK_CHAR: {
                            char valc = ((Cell<char> *) rowElements[a])->getValue();
                            if ((char) split[1].at(0) == valc) {
                                check = true;
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
    return check;
}

void SqlEngine::executeDeleteFrom(string &cmd) {

    try {
        string out_str = removeSpace(cmd);
        if (out_str.find(DELETE_FROM_D) != string::npos && out_str.find("where") != string::npos) {
            int w = out_str.find("where");
            int s = out_str.find(DELETE_FROM_D);
            if (w < s) {
                throw invalid_argument("ERROR: Wrong command order");
            }
            string tableName;
            string str = out_str.erase(out_str.find("where"), out_str.find(';') + 1);
            removeSubstrs(str, DELETE_FROM_D);
            tableName.assign(str);
            int t = str.find(tableName);
            if (t < s || t > w) {
                throw invalid_argument("ERROR: Wrong command order");
            }
            if (tableName == "") {
                throw invalid_argument("ERROR: Table Name not defined");
            }
            int index;
            if (!state.tablePresent(tableName, index)) {
                throw invalid_argument("ERROR: Name Table doesn't exist");
            }
            vector<string> quoteargs = getValueInQuote(cmd);
            string st = removeSpace(cmd);
            string fieldWhere;

            fieldWhere = st.erase(0, st.find("where"));
            removeSubstrs(fieldWhere, "where");
            vector<string> split = splitWhereField(fieldWhere);
            string fieldToDelete = split[0];
            string valueToDelete = split[1];
            if (!quoteargs.empty()) {
                split[1] = removeQuotes(split[1]);
                valueToDelete = removeQuotes(valueToDelete);
            }

            if (!checkWhereField(split, index, tableName)) {
                throw invalid_argument("ERROR: Invalid Where Field");
            }
            for (int l = 0; l < state.getTables().size(); l++) {
                if (!state.getTables()[l].getName().compare(tableName)) {
                    if (!checkWhereValue(split, index, tableName)) {
                        throw invalid_argument("ERROR: Invalid Where Value");
                    }
                    state.deleteFrom(index, fieldToDelete, valueToDelete);
                    break;
                }
            }
        } else if (out_str.find("where") != string::npos == 0) {
            throw invalid_argument("ERROR: Missing where field, cannot delete record");
        }
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void SqlEngine::executeUpdate(string cmd) {

    try {
        string out_string = removeSpace(cmd);
        if (out_string.find(UPDATE_D) != string::npos && out_string.find("set") != string::npos &&
            out_string.find("where") != string::npos) {
            int i = out_string.find(UPDATE_D);
            int j = out_string.find("set");
            int k = out_string.find("where");
            if (j > k || j < i || k < i) { //mettere tutte le condizioni
                throw invalid_argument("ERROR: Wrong command order");
            }
            string str = out_string.erase(out_string.find("set"), out_string.find(';') + 1);
            removeSubstrs(str, UPDATE_D);
            string tableName;
            tableName.assign(str);
            int t = str.find(tableName);
            if (tableName == "") {
                throw invalid_argument("ERROR: Table Name not defined");
            }
            if (t < i) {
                throw invalid_argument("ERROR: Wrong Command order");
            }
            int index;
            if (!state.tablePresent(tableName, index)) {
                throw invalid_argument("ERROR: Name Table doesn't exist");
            }
            vector<string> quoteargs = getValueInQuote(cmd);
            string s = removeSpace(cmd);
            removeSubstrs(s, tableName);
            removeSubstrs(s, UPDATE_D);
            string fieldSet = s.erase(s.find("where"), s.find(';') + 1);
            removeSubstrs(fieldSet, "set");
            vector<string> splitSet = splitSetField(fieldSet);
            string fieldToSet = splitSet[0];
            string valueToSet = splitSet[1];
            if (!quoteargs.empty()) {
                valueToSet = removeQuotes(valueToSet);
            }
            if (!checkSetField(splitSet, index, tableName)) {
                throw invalid_argument("ERROR: Invalid Set Field");
            }
            string ss = removeSpace(cmd);
            removeSubstrs(ss, tableName);
            removeSubstrs(ss, UPDATE_D);
            removeSubstrs(ss, fieldSet);
            removeSubstrs(ss, "set");
            string whereField;
            whereField.assign(ss);
            removeSubstrs(whereField, "where");
            vector<string> splitWhere = splitWhereField(whereField);
            string fieldToWhere = splitWhere[0];

            if (!checkWhereField(splitWhere, index, tableName)) {
                throw invalid_argument("ERROR: Invalid Where Field");
            }
            string valueToWhere = splitWhere[1];
            if (!quoteargs.empty()) {
                valueToWhere = removeQuotes(valueToWhere);
            }
            if (!checkWhereValue(splitWhere, index, tableName)) {
                throw invalid_argument("ERROR: Invalid Where Value");
            }

            for (int b = 0; b < state.getTables().size(); b++) {
                if (!state.getTables()[b].getName().compare(tableName)) {
                    for (int c = 0; c < state.getTables()[b].getCols()[c].getName().size(); c++) {
                        if (!state.getTables()[b].getCols()[c].getName().compare(fieldToSet) &&
                            state.getTables()[b].getCols()[c].getMask() & MASK_AUTOINCREMENT) {
                            throw invalid_argument("ERROR: Set Field cannot be Auto_Increment");
                        }
                        state.update(index, fieldToSet, valueToSet, valueToWhere, fieldToWhere);
                        break;
                    }
                }
            }

        } else if (out_string.find("where") != string::npos == 0) {
            throw invalid_argument("ERROR: Missing Where field, cannot update");
        } else if (out_string.find("set") != string::npos == 0) {
            throw invalid_argument("ERROR: Missing Set field, cannot update");
        }
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

bool SqlEngine::checkSetField(vector<string> splitSet, int index, string tableName) {

    bool check = false;
    if (!state.getTables()[index].getName().compare(tableName)) {
        vector<Column> cols = state.getTables()[index].getCols();
        for (int a = 0; a < cols.size(); a++) {
            if (!cols[a].getName().compare(splitSet[0])) {
                check = true;
            }
        }
    }
    return check;
}

vector<string> SqlEngine::splitSetField(string fieldSet) {

    vector<string> split = splitValueByDelimiter(fieldSet, "=");
    removeSubstrs(split[1], ";");
    removeQuotes(split[1]);
    return split;

}

string SqlEngine::removeQuotes(string str) {

    str.erase(remove(str.begin(), str.end(), '\"'), str.end());
    return str;
}

vector<string> SqlEngine::getOrderByInfo(string command, string tableName) {

    vector<string> ret;
    try {
        string str = removeSpace(command);
        if (str.find(ORDER_BY_D) != string::npos) {
            string order = str.erase(0, str.find(ORDER_BY_D));
            if (order.find("asc") != string::npos) {
                ret.emplace_back("asc");
                removeSubstrs(order, ORDER_BY_D);
                removeSubstrs(order, "asc");
                removeSubstrs(order, ";");
                ret.push_back(order);
                if (order == "") {
                    throw invalid_argument("ERROR: Missing order field");
                }
                for (int j = 0; j < state.getTables().size(); j++) {
                    if (!state.getTables()[j].tableName.compare(tableName)) {
                        if (!state.getTables()[j].isFieldPresent(order)) {
                            throw invalid_argument("ERROR: Cannot order, invalid field");
                        }
                    }
                }
            } else if (str.find("desc") != string::npos) {
                ret.emplace_back("desc");
                removeSubstrs(order, ORDER_BY_D);
                removeSubstrs(order, "desc");
                removeSubstrs(order, ";");
                ret.push_back(order);
                if (order == "") {
                    throw invalid_argument("ERROR: Missing order field");
                }
                for (int i = 0; i < state.getTables().size(); i++) {
                    if (!state.getTables()[i].tableName.compare(tableName)) {
                        if (!state.getTables()[i].isFieldPresent(order)) {
                            throw invalid_argument("ERROR: Cannot order, invalid field");
                        }
                    }
                }
            }
        }

    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
    return ret;
}



bool SqlEngine::checkColisNotKeyWord(vector<string> paramsNames) {

    bool check = false;
    for (int i = 0; i < paramsNames.size(); i++) {
        if (!paramsNames[i].compare(CREATE_TABLE_D) || !paramsNames[i].compare(DROP_TABLE_D) ||
            !paramsNames[i].compare(INSERT_INTO_D) || !paramsNames[i].compare(DELETE_FROM_D) ||
            !paramsNames[i].compare(TRUNCATE_TABLE_D) || !paramsNames[i].compare(UPDATE_D) ||
            !paramsNames[i].compare(SELECT_D) || !paramsNames[i].compare(SELECTALLFROM_D) ||
            !paramsNames[i].compare(ORDER_BY_D) || !paramsNames[i].compare("where") || !paramsNames[i].compare("set")) {
            check = true;
        }
    }
    return check;
}

vector<string> SqlEngine::splitPreservingArgsInQuote(string command) {

    vector<string> ret;

    try {
        int len = command.length();
        bool qot = false, sqot = false;
        int arglen;
        for (int i = 0; i < len; i++) {
            int start = i;
            if (command[i] == '\"') {
                qot = true;
            } else if (command[i] == '\'') sqot = true;

            if (qot) {
                i++;
                start++;
                while (i < len && command[i] != '\"')
                    i++;
                if (i < len)
                    qot = false;
                arglen = i - start;
                i++;
            } else if (sqot) {
                i++;
                while (i < len && command[i] != '\'')
                    i++;
                if (i < len)
                    sqot = false;
                arglen = i - start;
                i++;
            } else {
                while (i < len && command[i] != ',')
                    i++;
                arglen = i - start;
            }
            ret.push_back(command.substr(start, arglen));
        }

        removeSubstrs(ret.back(), ");");


        if (qot || sqot) {
            throw invalid_argument("WARNING: One of the quotes is open");
        }

    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }

    return ret;

}

string SqlEngine::getPkey(vector<string> paramsRaw) {
    string retValue = "";
    for (int i = 0; i < paramsRaw.size(); i++){
        if (paramsRaw[i].find("primarykey") != string::npos){
            retValue.assign(paramsRaw[i]);
            removeSubstrs(retValue, "primarykey");
            break;
        }
    }
    return retValue;
}

bool SqlEngine::checkPkeyPos(vector<string> params) {
    bool valid = false;
    string tmp = "";
    if (params[params.size() - 1].find("primarykey") != string::npos){
        tmp.assign(params[params.size() - 1]);
        removeSubstrs(tmp, "primarykey");
        if ((params[params.size() - 1].find("int") == string::npos) &&
                (params[params.size() - 1].find("float") == string::npos) &&
                (params[params.size() - 1].find("char") == string::npos) &&
                (params[params.size() - 1].find("text") == string::npos) &&
                (params[params.size() - 1].find("date") == string::npos) &&
                (params[params.size() - 1].find("time") == string::npos)
                ){
            valid = true;
        }
    }
    return valid;
}



















