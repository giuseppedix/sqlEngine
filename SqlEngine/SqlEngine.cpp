#include "SqlEngine.h"


int SqlEngine::execute(const string &command) {
    string command_down;
    int ret = 0;

    try {
        command_down.assign(command);
        transform(command_down.begin(), command_down.end(), command_down.begin(), ::tolower);
        switch (getCommand((command_down))) {
            case CREATE_TABLE:
                //TODO -> executeCreateTable
                executeCreateTable(command_down);
                break;
            case DROP_TABLE:
                //TODO -> executeDropTable
                cout << "Drop la tabella";
                break;
            case INSERT_INTO:
                //TODO -> executeInsertInto
                cout << "insert la tabella";
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
                cout << "Non hai selezionato nessun comando a dispozione" << endl;
                throw invalid_argument("ERROR: command not valid");
        }
    }
    catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
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

string SqlEngine::getParamsInBrakets(string command) {
    int id, age;
    float salary;
    string name, address;


    return std::string();
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
        string::size_type i = out_str.find(CREATE_TABLE_D);

        if (i != 0)
            throw invalid_argument("ERROR: command not valid");

        removeSubstrs(out_str,CREATE_TABLE_D);
        string nameTable = findNameTable(out_str);

        if (nameTable.compare("") == 0)
            throw invalid_argument("ERROR: Table name not defined");

        removeSubstrs(out_str,nameTable);
        cout << "Create Table: " << nameTable << endl;
        cout << "Parameters: ";
        vector<string> params = getParams(out_str);
        vector<int> paramsMasks;
        vector<string> paramsNames;
        for(int i = 0; i < params.size(); i++){
            cout << params[i] << ", ";
            //TODO getParamInfo
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

void SqlEngine::removeSubstrs(string& s, string p) {
    string::size_type n = p.length();
    for (string::size_type i = s.find(p);
         i != string::npos;
         i = s.find(p))
        s.erase(i, n);
}

string SqlEngine::findNameTable(string c) {
    string nameTable = "";
    string ch = "(";
    string::size_type st = c.find(ch);
    if (st != string::npos){
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

vector<string> SqlEngine::splitValueByDelimiter(string s, string delimiter) {
    vector<string> ret;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        ret.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    ret.push_back(s);
    return ret;
}

void SqlEngine::getParamInfo(const string &s, int &paramMask, string &name) {

    int i = 0;

    if(s.find("int") != string::npos){
        i = s.find("int");
        paramMask |= MASK_INT;
    }
    else if(s.find("float") != string::npos){
        i = s.find("float");
        paramMask |= MASK_FLOAT;
    }
    else if(s.find("text") != string::npos){
        i = s.find("text");
        paramMask |= MASK_TEXT;
    }
    else if(s.find("date") != string::npos){
        i = s.find("date");
        paramMask |= MASK_DATE;
    }
    else if(s.find("time") != string::npos){
        i = s.find("time");
        paramMask |= MASK_TIME;
    }
    else if(s.find("char") != string::npos){
        i = s.find("char");
        paramMask |= MASK_CHAR;
    }
    string str;
    str.assign(s);
    str.erase(i, str.length() - i);
    name = str;
    cout << name << endl;
    if(s.find("notnull") != string::npos){
        paramMask |= MASK_NOTNULL;
    }
    if(s.find("autoincrement") != string::npos){
        paramMask |= MASK_AUTOINCREMENT;
    }

    bitset<8> x(paramMask);
    cout << x << endl;

}













