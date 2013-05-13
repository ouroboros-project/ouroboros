
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
    
    //try {
        parser.parse();
    //}
    //catch (std::exception const &exc) {
    //    std::cout << "ExceptionMessage: " << exc.what() << std::endl;
    //    throw exc;
    //}
  }
  return EXIT_SUCCESS;
}

