
#ifndef OPWIG_GEN_WRAPPERSPECIFICATION_H_
#define OPWIG_GEN_WRAPPERSPECIFICATION_H_

#include <opwig/md/ptr.h>
#include <string>

namespace opwig {
namespace gen {

/// Base class for specification of a scripting language for wrapper generation.
class WrapperSpecification {

  public:
    /// constructor
    WrapperSpecification() {}
    
    /// Virtual destructor.
    virtual ~WrapperSpecification() {}


    std::string FileHeader();
    std::string FinishFile();
    
    std::string Function(const Ptr<const Function>& obj);
    std::string Variable(const Ptr<const Variable>& obj);
    std::string Class(const Ptr<const Class>& obj);
    std::string Namespace(const Ptr<const Namespace>& obj);
    std::string Enum(const Ptr<const Enum>& obj);
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_WRAPPERSPECIFICATION_H_


