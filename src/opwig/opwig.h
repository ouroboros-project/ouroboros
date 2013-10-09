
#ifndef OPWIG_MAIN_H_
#define OPWIG_MAIN_H_

#include <opwig/md/ptr.h>
#include <list>
#include <string>

namespace opwig {
namespace gen {

class WrapperSpecification;

/// Includes a new directory to be taken into consideration while parsing source code.
/** OPWIG looks for the files to be parsed using this directories. The order in which it does
 ** so is the same order they are added using this function.
 ** @param dir The included directory.
 **/
void IncludeDirectory (const std::string& dir);

/// Main function for OPWIG.
/** It executes OPWIG to generate a module with the given name in the specified output directory,
 ** wrapping the given input files, for a given language.
 **/
int Execute (const std::string& module_name, const std::list<std::string>& inputs,
             const md::Ptr<gen::WrapperSpecification>& language_spec,
             const std::string& output_dir);

/// An "int main()"-friendlier version of OPWIG's main function.
/** @param argc C main's argc.
 ** @param argv C main's argv.
 ** @tparam The WrapperSpecification implementation class.
 ** @return int Determines whether the wrapper generation was successful or not, based on common
 **             C/C++ main() return values (EXIT_SUCCESS/EXIT_FAILURE).
 **/
template <typename Specification>
int Execute (int argc, char **argv) {
    using std::list;
    using std::string;
    using md::Ptr;
    using gen::WrapperSpecification;
    list<string>  inputs;
    string        module_name = "Module";
    string        output_dir  = ".";
    for (++argv, --argc; argc; ++argv, --argc) {
        size_t namearg_len = sizeof("--module-name=")-1,
               dirarg_len  = sizeof("--output-dir=")-1,
               include_len = sizeof("-I")-1;
        string arg = *argv;
        if (arg.substr(0, namearg_len) == "--module-name=")
            module_name = arg.substr(namearg_len);
        else if (arg.substr(0, dirarg_len) == "--output-dir=")
            output_dir = arg.substr(dirarg_len);
        else if (arg.substr(0, include_len) == "-I")
            opwig::gen::IncludeDirectory(arg.substr(include_len));
        else
            inputs.push_back(*argv);
    }
    return Execute(
        module_name,
        inputs,
        Ptr<WrapperSpecification>(new Specification(module_name)),
        output_dir
    );
}

} // namespace gen
} // namespace opwig

#endif // OPWIG_MAIN_H_


