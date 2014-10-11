
#include <opwig/opwig.h>

#include <opwig/gen/proxygenerator.h>
#include <opwig/gen/wrappergenerator.h>
#include <opwig/md/ptr.h>
#include <opwig/md/namespace.h>
#include <opwig/json/reader.h>

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

bool OpenHeader(const string& filename, ifstream& in) {
    in.open(filename);
    if (in.good())
        return true;
    in.close();
    return false;
}

}

namespace opwig {
namespace gen {

int Execute (const string& module_name, const list<string>& inputs,
             const Ptr<WrapperSpecification>& language_spec, const string& output_file) {
    
    Ptr<opwig::md::Namespace> global = opwig::md::Namespace::Create("");
    list<string> headers;
   
    for (string input : inputs) {
        ifstream in;
        if (!OpenHeader(input, in)) {
            std::cerr << OPWIG_MARK << "Failed to open JSON file \"" << input << "\". Error:" << std::endl;
            return EXIT_FAILURE;
        }
        opwig::json::Reader parser(in, global);           
        
        std::cout << OPWIG_MARK << "Parsing JSON file \"" << input << "\"" << std::endl;
        if (!parser.parse()) {
            std::cerr << OPWIG_MARK << "Failed to JSON metadata." << std::endl;
            return EXIT_FAILURE;
        }
        for (const string& header : parser.metadata_headers())
            headers.push_back(header);
    }

    opwig::gen::WrapperGenerator(headers, output_file).Generate(module_name, global, language_spec);
    return EXIT_SUCCESS;
}

} // namespace gen 
} // namespace opwig
