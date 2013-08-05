
#include "out.h"

#include <string>
#include <iostream>

namespace out {

using std::cout;
using std::endl;

bool BREAK_LINE = true;

void send_message (const char* str) {
    if (BREAK_LINE)
        cout << str << endl;
    else {
        cout << str;
        cout.flush();
    }
}

void send_multimessage (const char* str, int n) {
    for (int i=0; i < n; i++)
        cout << str << endl;
}

namespace constants {

const char *TALKER_NAME = "Bob Singer";

} // namespace constants

} // namespace out

