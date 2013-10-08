
#include <opwig/parser/mdparser.h>
#include <opwig/gen/proxygenerator.h>
#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/md/ptr.h>
#include <opwig/md/namespace.h>


#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

using std::list;
using std::string;
using std::ifstream;
using opwig::md::Ptr;
using opwig::gen::WrapperSpecification;

namespace {

const string  OPWIG_MARK = "[opwig] ";
list<string>  include_dirs;

bool OpenHeader (const string& filename, ifstream& in) {
    for (auto include_dir : include_dirs) {
        in.open(include_dir+"/"+filename);
        if (in.good())
            return true;
        in.close();
    }
    in.open(filename);
    if (in.good())
        return true;
    in.close();
    return false;
}

}

namespace opwig {
namespace gen {

void IncludeDirectory (const string& dir) {
    include_dirs.push_back(dir);
}

int Execute (const string& module_name, const list<string>& inputs,
             const Ptr<WrapperSpecification>& language_spec, const string& output_dir) {
    
    Ptr<opwig::md::Namespace> global = opwig::md::Namespace::Create("");
    for (string input : inputs) {
        ifstream in;
        if (!OpenHeader(input, in)) {
            std::cerr << OPWIG_MARK << "Failed to open source \"" << input << "\". Error:" << std::endl;
            return EXIT_FAILURE;
        }
        opwig::MDParser parser(in, global);
        
        std::cout << OPWIG_MARK << "Parsing source \"" << input << "\"" << std::endl;
        if (parser.parse()) {
            std::cerr << OPWIG_MARK << "Failed to parse C++ code." << std::endl;
            return EXIT_FAILURE;
        }
    }

    opwig::gen::WrapperGenerator(inputs, output_dir).Generate(module_name, global, language_spec);
    return EXIT_SUCCESS;
}

} // namespace gen 
} // namespace opwig
