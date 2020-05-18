#include "Parser.h"
#include <algorithm>
#include <vector>

#define INT_ "int"
#define TEXT_ "text"
#define FLOAT_ "float"

int Parser::execute(const string &command) {
    string command_down;
    int ret = 0;
    int pos = 0;
    vector<string> sql_vector;

    try {
        command_down.assign(command);
        transform(command_down.begin(), command_down.end(), command_down.begin(), ::tolower);
        switch (getCommand((command_down))) {
            case CREATE_TABLE:
                //TODO -> executeCreateTable
                cout << "Create Table" << endl;
                getParamsInBrakets(command_down);
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
                throw invalid_argument("ERROR: command not valid");
        }
    }
    catch (invalid_argument &exc1) {
        cerr << exc1.what() << endl;
    }

    return ret;
}

Command Parser::getCommand(string command) {

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

int Parser::getParamsInBrakets(string command_down) {

    transform(command_down.begin(), command_down.end(), command_down.begin(), ::tolower);

    string start = "(";
    string end = ")";
    string delim = ",";
    size_t p = 0;
    size_t space = 0;
    int cont;

    command_down.erase(0, command_down.find(start) + 1);
    command_down.erase(command_down.find(')'), command_down.find(';'));
    command_down += ',';
    vector<string> tmp;
    vector<string>params;
    cont = 0;
    while ((p = command_down.find(delim)) != string::npos) {
        tmp.push_back(command_down.substr(0, p));
        params.push_back(removeSpace(tmp.at(cont)));
        cont++;
        command_down.erase(0, p + delim.length());
    }

    vector<string> value;

    for(int i = 0; i != params.size(); i++) {
         size_t lastdelim = 0;
         size_t firstdelim = 0;
         lastdelim = params.at(i).find(',') != string ::npos;
         firstdelim = params.at(i).find(' ') != string::npos;
         params.at(i).erase(0, params.size()-lastdelim);
         value.push_back(params.at(i).erase(0, params.size()-firstdelim));

    }



    return 0;
}


Type Parser::getValuesType(vector<string> value) {

  Type type_enum;


    try {

        if (type_enum != INT)
            throw runtime_error("ERROR: Parameter is not an int");
    }
    catch (runtime_error &exc) {
        cerr << exc.what() << endl;

    }


    try {

        if (type_enum != FLOAT)
            throw runtime_error("ERROR: Parameter is not a float");
    }
    catch (runtime_error &exc) {
        cerr << exc.what() << endl;

    }

    try {

        if (type_enum != TEXT)
            throw runtime_error("ERROR: Parameter is not a text");
    }
    catch (runtime_error &exc) {
        cerr << exc.what() << endl;

    }



    return type_enum;








}

string Parser::removeSpace(string input) {

    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    return input;

}






















