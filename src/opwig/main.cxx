
#include <opwig/parser/mdparser.h>
#include <opwig/gen/proxygenerator.h>

#include <cstdlib>

#include <iostream>
#include <fstream>

#include <memory>
#include <exception>

int main (int argc, char** argv) {
  for (++argv, --argc; argc; ++argv, --argc) {
    std::ifstream in(*argv);
    opwig::MDParser parser(in);
    
    if (parser.parse())
      std::cout << "Failed to parse C++ code." << std::endl;

    opwig::gen::ProxyGenerator("./").Generate(parser.global_namespace());

  }
  return EXIT_SUCCESS;
}

