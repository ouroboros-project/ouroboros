
#include <opwig/gen/proxygenerator.h>

#include <opwig/md/scope.h>
#include <opwig/md/class.h>

namespace opwig {

namespace gen {

using md::Ptr;
using md::Scope;

size_t ProxyGenerator::Generate (const Ptr<Scope>& the_scope) {
    size_t virtual_count = 0;
    for (auto entry : the_scope->IterateClasses())
        if (entry.second->destructor())
            ++virtual_count;
    return virtual_count;
}

} // namespace gen

} // namespace opwig

