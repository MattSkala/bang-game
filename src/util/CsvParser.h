#ifndef BANG_CSVPARSER_H
#define BANG_CSVPARSER_H


#include <vector>
#include <string>

using namespace std;


/// A helper class for parsing CSV file into vectors
class CsvParser {
private:
    vector<string> parseRow(string line, char separator);
public:
    /**
     * \brief Parses a file into vectors.
     * \param filename The path to file which should be parsed.
     * \param separator A separator character dividing columns.
     * \param ignore_first_line Whether the first line is a comment and should be ignored.
     * \return Vector of vectors representing rows and columns.
     */
    vector<vector<string>> parseFile(string filename, char separator, bool ignore_first_line);
};


#endif //BANG_CSVPARSER_H
