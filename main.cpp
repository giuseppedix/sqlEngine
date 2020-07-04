#include <iostream>
#include "SqlEngine/SqlEngine.h"
#include "DateTime/Date.h"
#include "DateTime/Time.h"

using namespace std;

int main() {
    string dbName = "SqlDb";
    SqlEngine sqlEngine;
    Date date;
    Time time;
    sqlEngine.loadState(dbName);

    cout << "Date: " << date << endl;
    auto start = chrono::system_clock::now();
    string command_input;
    bool quit = false;
    do {
        string command;
        cout << sqlEngine.getState().getDbName() << ">> ";
        do {
            getline(cin, command_input);
            command += command_input;
            transform(command.begin(), command.end(), command.begin(), ::tolower);
        } while (command_input.back() != ';');

        if (command.find("quit()") != string::npos) {
            quit = true;
        } else {
            sqlEngine.execute(command);
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end-start;
            time_t end_time = chrono::system_clock::to_time_t(end);
            cout << "finished computation at " << ctime(&end_time)
                 << "elapsed time: " << elapsed_seconds.count() << "ms\n"; //verifica
        }
        cout << endl;
    } while (!quit);

    return 0;
}
