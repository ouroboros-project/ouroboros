
#include "input.h"

#include <iostream>
#include <string>

namespace input {

using std::cin;
using std::endl;
using std::string;

const char* receive_message () {
    string value;
    getline(cin, value);
    return value.c_str();
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

