#include <fstream>
#include "CsvParser.h"


vector<vector<string>> CsvParser::parseFile(string filename, bool ignore_first_line) {
    vector<vector<string> > data;

    // open file
    ifstream in;
    in.open(filename);

    if (!in.is_open()) {
        throw "cannot open file";
    }

    string line;

    if (ignore_first_line) {
        getline(in, line);
    }

    while (getline(in, line)) {
        vector<string> row = parseRow(line);
        data.push_back(row);
    };

    in.close();

    return data;
}

vector<string> CsvParser::parseRow(string line) {
    vector<string> row;

    char separator = ';';
    string cell;

    for (unsigned int i = 0; i < line.size(); i++) {
        if (line[i] == separator) {
            row.push_back(cell);
            cell.clear();
        } else {
            cell.push_back(line[i]);
        }
    }

    row.push_back(cell);

    return row;
}
