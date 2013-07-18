
#ifndef OPWIG_GEN_WRAPPERSPECIFICATION_H_
#define OPWIG_GEN_WRAPPERSPECIFICATION_H_

#include <opwig/md/ptr.h>
#include <string>

namespace opwig {

namespace md {
class Function;
}

namespace gen {

/// Base class for specification of a scripting language for wrapper generation.
class WrapperSpecification {

  public:
    /// constructor
    WrapperSpecification() {}
    
    /// Virtual destructor.
    virtual ~WrapperSpecification() {}
    
    virtual std::string WrapperName() const = 0;
    
    virtual md::Ptr<ConverterProvider> GetConverterProvider() const = 0;

    virtual std::string FileHeader() const = 0;
    virtual std::string FinishFile() const = 0;
    
    virtual std::string WrapFunction(const md::Ptr<const md::Function>& obj) const = 0;
    /*virtual std::string WrapVariable(const md::Ptr<const md::Variable>& obj) = 0;
    virtual std::string WrapClass(const md::Ptr<const md::Class>& obj) = 0;
    virtual std::string WrapNamespace(const md::Ptr<const md::Namespace>& obj) = 0;
    virtual std::string WrapEnum(const md::Ptr<const md::Enum>& obj) = 0;*/
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_WRAPPERSPECIFICATION_H_


