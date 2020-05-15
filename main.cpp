#include <iostream>
#include "Parser/Parser.h"

using namespace std;

int main() {

    Parser parser;

    string command_input;
    bool quit = false;


    do{
        string command;
        cout << "MyDB> ";

        do{
            getline(cin, command_input);
            command+=command_input;
        }
        while (command_input.back() != ';');

        if(command.find("QUIT")!= string::npos){
            quit = true;
        }
        else{
            parser.execute(command);
        }

    }

    while (!quit);




    return 0;
}
