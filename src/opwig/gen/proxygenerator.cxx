
#include <opwig/gen/proxygenerator.h>

#include <opwig/md/scope.h>
#include <opwig/md/class.h>
#include <opwig/md/function.h>

#include <fstream>

namespace opwig {

namespace gen {

using md::Ptr;
using md::Scope;
using std::ofstream;
using std::ios_base;
using std::endl;

size_t ProxyGenerator::Generate (const Ptr<Scope>& the_scope) {
    size_t virtual_count = 0;
    for (auto entry : the_scope->IterateClasses())
        if (entry.second->destructor() && entry.second->destructor()->is_virtual()) {
            ++virtual_count;
            ofstream generated_file;
            generated_file.open(output_dir_+"/"+entry.second->name()+"_proxy.h", ios_base::out);
            generated_file
                << "#ifndef OUROBOROS_GENERATED_" << entry.second->name() << "_H_" << endl;
            generated_file.close();
        }
    return virtual_count;
}

} // namespace gen

} // namespace opwig

