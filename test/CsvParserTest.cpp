#include "../src/util/CsvParser.h"
#include <assert.h>
#include <cstring>
#include <iostream>

using namespace std;

void testCsvParser() {
    try {
        CsvParser().parseFile("not-existing-file.csv", true);
    } catch (const char * ex) {
        assert(strcmp(ex, "cannot open file") == 0);
    }

    vector< vector<string> > data = CsvParser().parseFile("res/cards.csv", true);
    assert(data.size() == 42);
    assert(data[0][0] == "Sceriffo");
    assert(data[0][1] == "Šerif");
    assert(data[0][2] == "1");
    assert(data[0][3] == "role");
    assert(data[0][4] == "");
    assert(data[0][16] == "");
    assert(data[37][16] == "1");
}
