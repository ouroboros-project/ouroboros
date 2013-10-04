#include <opwig/opwig.h>
#include <opwig/gen/wrapperspecification.h>
#include <languages/python/pythonspecification.h>
#include <opwig/md/ptr.h>

#include <cstdlib>
#include <list>
#include <string>
#include <memory>

using std::list;
using std::string;
using opwig::md::Ptr;
using opwig::gen::WrapperSpecification;

int main (int argc, char** argv) {
    list<string>  inputs;
    string        module_name = "Module";
    string        output_dir  = ".";
    for (++argv, --argc; argc; ++argv, --argc) {
        size_t namearg_len = sizeof("--module-name=")-1,
               dirarg_len  = sizeof("--output-dir=")-1;
        string arg = *argv;
        if (arg.substr(0, namearg_len) == "--module-name=")
            module_name = arg.substr(namearg_len);
        else if (arg.substr(0, dirarg_len) == "--output-dir=")
            output_dir = arg.substr(dirarg_len);
        else
            inputs.push_back(*argv);
    }
    return opwig::gen::Execute(
        module_name,
        inputs,
        Ptr<WrapperSpecification>(new opa::python::PythonSpecification),
        output_dir
    );
}
