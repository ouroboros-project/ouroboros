
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
    
    virtual std::string WrapperName() const = 0;
    
    virtual Ptr<ConverterProvider> GetConverterProvider() const = 0;

    virtual std::string FileHeader() const = 0;
    virtual std::string FinishFile() const = 0;
    
    virtual std::string WrapFunction(const Ptr<const Function>& obj) const = 0;
    /*virtual std::string WrapVariable(const Ptr<const Variable>& obj) = 0;
    virtual std::string WrapClass(const Ptr<const Class>& obj) = 0;
    virtual std::string WrapNamespace(const Ptr<const Namespace>& obj) = 0;
    virtual std::string WrapEnum(const Ptr<const Enum>& obj) = 0;*/
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_WRAPPERSPECIFICATION_H_


