#include <vector>
#include <string>

using namespace std;


#ifndef BANG_CSVPARSER_H
#define BANG_CSVPARSER_H


class CsvParser {
private:
    vector<string> parseRow(string line);
public:
    vector< vector<string> > parseFile(string filename, bool ignore_first_line);
};


#endif //BANG_CSVPARSER_H
