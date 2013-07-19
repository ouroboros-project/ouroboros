
#ifndef OPWIG_GEN_PROXYGENERATOR_H_
#define OPWIG_GEN_PROXYGENERATOR_H_

#include <opwig/md/ptr.h>

#include <string>

namespace opwig {

namespace md {
class Scope;
}

namespace gen {

class ProxyGenerator final {

  public:

    ProxyGenerator (const std::string& the_output_dir);

    /// Generates proxies from the classes in the given scope.
    //  @param the_scope Scope whose classes will be proxied.
    //  @return The number of proxied classes.
    size_t Generate (const md::Ptr<const md::Scope>& the_scope);

  private:

    std::string output_dir_;

};

inline ProxyGenerator::ProxyGenerator (const std::string& the_output_dir)
    : output_dir_(the_output_dir) {}

} // namespace gen
} // namespacie opwig 

#endif

