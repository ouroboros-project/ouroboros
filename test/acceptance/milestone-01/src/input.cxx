
#include "input.h"

#include <iostream>
#include <string>

#include <cstring>

namespace input {

using std::cin;
using std::endl;
using std::string;

namespace {
char buffer[256];
}

const char* receive_message () {
    string value;
    getline(cin, value);
    strcpy(buffer, value.c_str());
    return buffer;
}

double receive_number () {
    double value;
    cin >> value;
    return value;
}

bool receive_confirmation () {
    string value;
    cin >> value;
    if (value == "yes")
        return true;
    return false;
}

} // namespace input

