
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
 ** wrapping the given input files, for a given language.
 **/
int Execute (const std::string& module_name, const std::list<std::string>& inputs,
             const md::Ptr<gen::WrapperSpecification>& language_spec,
             const std::string& output_dir);

} // namespace gen
} // namespace opwig

#endif // OPWIG_MAIN_H_


