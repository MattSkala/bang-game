#include <stdlib.h>
#include <iostream>
#include "Controller.h"

using namespace std;


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
