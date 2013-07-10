
#include <opwig/gen/proxygenerator.h>
#include <opwig/gen/proxycodeprovider.h>

#include <opwig/md/scope.h>
#include <opwig/md/class.h>
#include <opwig/md/function.h>

#include <list>
#include <fstream>

namespace opwig {

namespace gen {

using md::Ptr;
using md::Scope;
using md::Class;
using md::Function;

using std::list;
using std::ofstream;
using std::ios_base;
using std::endl;

namespace {

list<Ptr<Function>> SelectVirtualFromMethods (const Ptr<Class>& the_class) {
    list<Ptr<Function>> virtual_methods;
    for (auto entry : the_class->IterateFunctions())
        if (entry.second->is_virtual())
            virtual_methods.push_back(entry.second);
    return virtual_methods;
}

} // unnamed namespace

size_t ProxyGenerator::Generate (const Ptr<Scope>& the_scope) {
    size_t proxy_count = 0;
    for (auto entry : the_scope->IterateClasses()) {
        auto virtual_methods = SelectVirtualFromMethods(entry.second);
        if (!virtual_methods.empty()) {
            ++proxy_count;
            ProxyCodeProvider provider(entry.second->name());
            ofstream generated_file;
            generated_file.open(output_dir_+"/"+entry.second->name()+"_proxy.h", ios_base::out);
            generated_file
                << provider.OpenProxy()
                << provider.CloseProxy();
            generated_file.close();
        }
    }
    return proxy_count;
}

} // namespace gen

} // namespace opwig

