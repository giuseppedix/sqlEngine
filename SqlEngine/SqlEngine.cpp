#include "SqlEngine.h"


SqlEngine::~SqlEngine(){
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
                try{
                    executeCreateTable(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case DROP_TABLE:
                //TODO -> executeDropTable
                cout << "Drop la tabella";
                break;
            case INSERT_INTO:
                try{
                    executeInsertInto(command_down);
                } catch (invalid_argument &exec) {
                    throw 0;
                }
                break;
            case DELETE_FROM:
                //TODO -> executeDeleteFrom
                cout << "delete la tabella";
                break;
            case TRUNCATE_TABLE:
                //TODO -> executeTruncateTable
                cout << "truncate la tabella";
                break;
            case UPDATE:
                //TODO -> executeUpdate
                cout << "update la tabella";
                break;
            case SELECT:
                //TODO -> executeSelect
                cout << "select la tabella";
                break;
            case ORDER_BY:
                //TODO -> executeOrderBy
                cout << "order la tabella";
                break;

            default:
                throw invalid_argument("ERROR: command not valid");
        }
        return ret;
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
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
    //TODO
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

        if (i != 0)
            throw invalid_argument("ERROR: command not valid");

        removeSubstrs(out_str, CREATE_TABLE_D);
        string nameTable = findNameTable(out_str);

        if (nameTable == "")
            throw invalid_argument("ERROR: Table name not defined");

        size_t in = out_str.find('(');
        out_str.erase(0, in);
        cout << "Create Table: " << nameTable << endl;
        cout << "Parameters: ";

        vector<string> params = getParams(out_str);
        vector<int> paramsMasks;
        vector<string> paramsNames;

        for (int i = 0; i < params.size(); i++) {
            cout << params[i] << ", ";
            int paramMask = MASK_INVALID;
            string paramName = "";

            getParamInfo(params[i], paramMask, paramName);
            paramsMasks.push_back(paramMask);
            paramsNames.push_back(paramName);
        }
        cout << endl;
        state.addTable(nameTable, paramsMasks, paramsNames);
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}

void SqlEngine::removeSubstrs(string &s, string p) {
    size_t n = p.length();
    for (size_t i = s.find(p);
         i != string::npos;
         i = s.find(p))
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
        if (s.find("autoincrement") != string::npos) {
            q = s.find("autoincrement");
            paramMask |= MASK_AUTOINCREMENT;
        }

        if ((j != 0) && (q != 0)){
            if (((j > q) || (i > j) || (q < i))) {
                throw invalid_argument("ERROR: Wrong order parameters");
            }
        } else if (q == 0 && j != 0){
            if (i > j) {
                throw invalid_argument("ERROR: Wrong order parameters");
            }
        } else if (j == 0 && q != 0){
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
        vector<string> quoteargs = getValueInQuote(command);
        for (int b = 0; b < quoteargs.size(); b++){
            string removed = removeSpace(quoteargs[b]);
            removeSubstrs(out_str, removed);
        }
        removeSubstrs(out_str, ",\"\"");
        vector<string> args = splitValueByDelimiter(out_str, ")(");
        vector<string> fields = getParams(args[0]);
        vector<string> values = getParams(args[1]);
        vector<string> valuesTxt;
        for (int b = 0; b < quoteargs.size(); b++){
            valuesTxt.push_back(quoteargs[b]);
        }
        state.setParamsTable(nameTable, fields, values, valuesTxt);
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}













