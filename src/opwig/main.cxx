
#include <opwig/mdparser/mdparser.h>

#include <cstdlib>
#include <iostream>
#include <fstream>

int main (int argc, char** argv) {
  for (++argv, --argc; argc; ++argv, --argc) {
    std::ifstream in(*argv);
    opwig::MDParser parser(in);
    parser.parse();
  }
  return EXIT_SUCCESS;
}

