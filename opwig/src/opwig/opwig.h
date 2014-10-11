
#ifndef OPWIG_MAIN_H_
#define OPWIG_MAIN_H_

#include <opwig/md/ptr.h>
#include <list>
#include <string>

namespace opwig {
namespace gen {

class WrapperSpecification;

/// Main function for OPWIG.
/** It executes OPWIG to generate a module with the given name in the specified output directory,
    wrapping the given input files, for a given language.
* @param module_name The name of the generated module.
* @param inputs List of path to C++ files to be parsed.
* @param language_spec Smart pointer to a language specification.
* @param output_dir The path to the directory to generate the files in.
* @return int Determines if the wrapper generation was successfull or not, based on common
    C/C++ main() return values (EXIT_SUCCESS/EXIT_FAILURE).
*/
int Execute (const std::string& module_name, const std::list<std::string>& inputs,
             const md::Ptr<gen::WrapperSpecification>& language_spec,
             const std::string& output_file);

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
    string        output_file = "";
    for (++argv, --argc; argc; ++argv, --argc) {
        size_t namearg_len = sizeof("--module-name=")-1,
               dirarg_len  = sizeof("--output-file=")-1;
        string arg = *argv;
        if (arg.substr(0, namearg_len) == "--module-name=")
            module_name = arg.substr(namearg_len);
        else if (arg.substr(0, dirarg_len) == "--output-file=")
            output_file = arg.substr(dirarg_len);
        else
            inputs.push_back(*argv);
    }
    auto spec = Ptr<WrapperSpecification>(new Specification(module_name));
    if (output_file.empty()) {
        output_file = spec->wrapper_name() + "_" + module_name + "_wrap.cxx";
    }
    return Execute(
        module_name,
        inputs,
        spec,
        output_file
    );
}

} // namespace gen
} // namespace opwig

#endif // OPWIG_MAIN_H_


