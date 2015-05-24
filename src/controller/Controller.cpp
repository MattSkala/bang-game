#include <stdlib.h>
#include <iostream>
#include "Controller.h"

using namespace std;


Controller::~Controller() { }

void Controller::clearScreen() {
    #ifdef WINDOWS
        system("cls");
    #else
        system ("clear");
    #endif
}

void Controller::printLogo() {
    cout << "  ____                      _ \n"
            " |  _ \\                    | |\n"
            " | |_) | __ _ _ __   __ _  | |\n"
            " |  _ < / _` | '_ \\ / _` | | |\n"
            " | |_) | (_| | | | | (_| | |_|\n"
            " |____/ \\__,_|_| |_|\\__, | (_)\n"
            "                     __/ |    \n"
            "                    |___/     \n" << endl << endl;
}

bool Controller::scanInt(string in, int & res) {
    char c;
    if (scanChar(in, c)) {
        res = c - '0';
        return true;
    }
    return false;
}

bool Controller::scanChar(string in, char & res) {
    if (cin.eof()) {
        throw "EOF";
    }
    if (in.size() == 1 || (in.size() == 2 && in[1] == '\n')) {
        res = in[0];
        return true;
    }
    return false;
}
