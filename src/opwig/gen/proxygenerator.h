
#ifndef OPWIG_GEN_PROXYGENERATOR_H_
#define OPWIG_GEN_PROXYGENERATOR_H_

#include <opwig/md/ptr.h>

namespace opwig {

namespace md {

class Scope;

}

namespace gen {

class ProxyGenerator final {
  public:
    void Generate(const md::Ptr<md::Scope>& the_scope);
};

} // namespace gen
} // namespacie opwig 

#endif

