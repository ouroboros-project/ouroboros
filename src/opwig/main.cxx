
#include <opwig/parser/mdparser.h>
#include <opwig/gen/proxygenerator.h>
#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/gen/lua/wrapperspecification.h>
#include <opwig/gen/python/pythonspecification.h>
#include <opwig/md/ptr.h>

#include <cstdlib>

#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include <memory>
#include <exception>

using std::list;
using std::string;
using opwig::md::Ptr;
using opwig::gen::WrapperSpecification;

namespace {
  const string OPWIG_MARK = "[opwig] ";
}

int main (int argc, char** argv) {
    list<string> inputs;
    string       module_name = "Module";
    for (++argv, --argc; argc; ++argv, --argc) {
        size_t len = sizeof("--module-name=")-1;
        string arg = *argv;
        if (arg.substr(0, len) == "--module-name=")
            module_name = arg.substr(len);
        else
            inputs.push_back(*argv);
    }
    for (string input : inputs) {
        string header_path = input;
        std::ifstream in(header_path);
        if (!in.good()) {
            std::cout << OPWIG_MARK << "Failed to open source \"" << input << "\"" << std::endl;
            return EXIT_FAILURE;
        }
        opwig::MDParser parser(in);
        
        std::cout << OPWIG_MARK << "Parsing source \"" << input << "\"" << std::endl;
        if (parser.parse()) {
            std::cout << OPWIG_MARK << "Failed to parse C++ code." << std::endl;
            return EXIT_FAILURE;
        }

        opwig::gen::ProxyGenerator("./", header_path).Generate(parser.global_namespace());
        opwig::gen::WrapperGenerator("./").Generate(
            module_name,
            parser.global_namespace(),
            Ptr<WrapperSpecification>(new opwig::gen::lua::WrapperSpecification(input))
        );
        opwig::gen::WrapperGenerator("./").Generate(
            module_name, 
            parser.global_namespace(),
            Ptr<WrapperSpecification>(new opwig::gen::python::PythonSpecification(input))
        );
    }
    return EXIT_SUCCESS;
}

