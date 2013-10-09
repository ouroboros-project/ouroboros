
#ifndef OPWIG_MAIN_H_
#define OPWIG_MAIN_H_

#include <opwig/md/ptr.h>
#include <list>
#include <string>

namespace opwig {
namespace gen {

class WrapperSpecification;

void IncludeDirectory (const std::string& dir);

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
             const std::string& output_dir);

} // namespace gen
} // namespace opwig

#endif // OPWIG_MAIN_H_


