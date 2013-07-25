
#include <opwig/parser/mdparser.h>
#include <opwig/gen/proxygenerator.h>
#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/gen/lua/wrapperspecification.h>
#include <opwig/md/ptr.h>

#include <cstdlib>

#include <string>
#include <iostream>
#include <fstream>

#include <memory>
#include <exception>

using std::string;
using opwig::md::Ptr;
using opwig::gen::WrapperSpecification;

int main (int argc, char** argv) {
  for (++argv, --argc; argc; ++argv, --argc) {
    string header_path = *argv;
    std::ifstream in(header_path);
    opwig::MDParser parser(in);
    
    if (parser.parse()) {
      std::cout << "Failed to parse C++ code." << std::endl;
      return EXIT_FAILURE;
    }

    opwig::gen::ProxyGenerator("./", header_path).Generate(parser.global_namespace());
    opwig::gen::WrapperGenerator("./")
      .Generate("module", parser.global_namespace(),
                Ptr<WrapperSpecification>(new opwig::gen::lua::WrapperSpecification));

  }
  return EXIT_SUCCESS;
}

