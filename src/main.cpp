#include <stdlib.h>
#include <iostream>
#include "Application.h"


int main(int argc, char * argv[]) {
    if (argc < 2) {
        cout << "You must provide cards configuration file" << endl;
        return EXIT_FAILURE;
    }

    try {
        Application().init(argv[1]);
    } catch (const char *err) {
        cerr << err;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
