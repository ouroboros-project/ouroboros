
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

    ProxyGenerator (const std::string& the_output_dir, const std::string& the_header_path);

    /// Generates proxies from the classes in the given scope.
    /**  @param the_scope Scope whose classes will be proxied.
         @return The number of proxied classes. */
    size_t Generate (const md::Ptr<const md::Scope>& the_scope);

  private:

    std::string output_dir_;
    std::string header_path_;

};

inline ProxyGenerator::ProxyGenerator (const std::string& the_output_dir,
                                       const std::string& the_header_path)
    : output_dir_(the_output_dir), header_path_(the_header_path) {}

} // namespace gen
} // namespacie opwig 

#endif

