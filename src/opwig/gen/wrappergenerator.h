
#ifndef OPWIG_GEN_WRAPPERGENERATOR_H_
#define OPWIG_GEN_WRAPPERGENERATOR_H_

#include <opwig/md/ptr.h>

namespace opwig {

namespace md {
class Scope;
}

namespace gen {

/// Metadata interface for C++ scopes.
class WrapperGenerator {

  public:
    /// constructor
    WrapperGenerator() {}
    
    /// Virtual destructor.
    virtual ~WrapperGenerator() {}

    ///
    void Generate(const Ptr<const Scope>& root);
    
  protected:


};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_WRAPPERGENERATOR_H_


