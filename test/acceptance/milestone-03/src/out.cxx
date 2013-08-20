
#include "out.h"

#include <string>
#include <iostream>

namespace out {

using std::cout;
using std::endl;

Sender::Sender ()
    : break_line(true), TALKER_NAME("Bob Singer") {}

void Sender::send_message (const char* str) {
    if (break_line)
        cout << TALKER_NAME << ": " << str << endl;
    else {
        cout << TALKER_NAME << ": " << str;
        cout.flush();
    }
}

void Sender::send_multimessage (const char* str, int n) {
    for (int i=0; i < n; i++)
        cout << TALKER_NAME << ": " << str << endl;
}

} // namespace out

