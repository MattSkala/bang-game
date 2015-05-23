/**
 * \mainpage
 * Bang! is a Wild West-themed card game, designed by Emiliano Sciarra and released by Italian publisher daVinci Editrice in 2002.
 *
 * This C++ implementation, developed by Matouš Skála as a semestral work in the course Programming and Algorithmics 2 at FIT CTU, is inspired by the original game rules, but it is a little bit simplified.
 *
 * The architecture is based on a client–server model. When a user hosts a game, GameServer is started as a separate process on his computer. Then all the users (even the host himself) are connecting with GameClient to that server via TCP stream sockets. Each client keeps 2 opened connections – one for synchronous two-way communication (API) and the other one for game state change events from the server. The port on which server is running must be accessible by other players' computers.
 *
 * \file
 * \brief The main program file.
 */

#include <stdlib.h>
#include <iostream>
#include "Exception.h"
#include "Application.h"


int main(int argc, char * argv[]) {
    if (argc < 2) {
        cout << "You must provide cards configuration file" << endl;
        return EXIT_FAILURE;
    }

    try {
        Application().init(argv[1]);
    } catch (Exception err) {
        cerr << err.getMessage() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
