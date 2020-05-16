#include <iostream>
#include "SqlEngine/SqlEngine.h"

using namespace std;

int main() {
    string dbName = "SqlDb";
    SqlEngine sqlEngine;
    sqlEngine.loadState(dbName);

    string command_input;
    bool quit = false;
    do{
        string command;
        cout << sqlEngine.getState().getDbName() << ">> ";
        do{
            getline(cin, command_input);
            command+=command_input;
        }
        while (command_input.back() != ';');

        if(command.find("QUIT")!= string::npos){
            quit = true;
        }
        else{
            sqlEngine.execute(command);
        }
        cout << endl;
    }
    while (!quit);
    return 0;
}
