#include <sstream>
#include "utils.h"

vector<string> explode(string data, char delimiter) {
    vector <string> parts;
    istringstream iss(data);
    string part;
    while (getline(iss, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}
