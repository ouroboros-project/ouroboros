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
    return opwig::gen::Execute(module_name, inputs, Ptr<WrapperSpecification>(new opa::python::PythonSpecification));
}
