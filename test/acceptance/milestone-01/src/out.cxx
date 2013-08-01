
#include "out.h"

#include <iostream>

namespace out {

using std::cout;
using std::endl;

void send_message (const char* str) {
  cout << str << endl;
}

void send_multimessage (const char* str, int n) {
    for (int i=0; i < n; i++)
        cout << str << endl;
}

} // namespace out

