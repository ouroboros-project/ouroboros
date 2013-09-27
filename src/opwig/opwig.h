
#ifndef OPWIG_MAIN_H_
#define OPWIG_MAIN_H_

#include <opwig/md/ptr.h>
#include <list>
#include <string>

namespace opwig {

namespace gen {
class WrapperSpecification;
}

/** Main function for OPWIG - It executes OPWIG to generate a module with the given name, 
    wrapping the given input files, for a given language. */
int Execute(const std::string& module_name, const std::list<std::string>& inputs, const md::Ptr<gen::WrapperSpecification>& language_spec);

} // namespace opwig

#endif // OPWIG_MAIN_H_


