
#ifndef OPWIG_GEN_WRAPPERSPECIFICATION_H_
#define OPWIG_GEN_WRAPPERSPECIFICATION_H_

#include <opwig/md/ptr.h>
#include <string>

namespace opwig {

namespace md {
class Function;
class Variable;
class Class;
class Namespace;
class Enum;
}

namespace gen {

/** Base class for specification of a scripting language for wrapper generation.
Conversion of types between C++ and this script is provided by a <>Converter class
in OPA. */
class WrapperSpecification {

  public:
    /// constructor
    WrapperSpecification() {}
    
    /// Virtual destructor.
    virtual ~WrapperSpecification() {}
    
    /// Name of this wrapper specification (name of the language it wraps).
    virtual std::string wrapper_name() const = 0;
    
    /** Returns the header block of text of the generated source files.
        This text block is written FIRST in the generated files, and as such
        you should put anything you will otherwise need here... */
    virtual std::string FileHeader() const = 0;
    
    /** Returns the final block of text of the generated source files.
        This text block is written LAST in the generated files, and as such
        you should put anything to finish the wrap code here...
        Most scripting languages (Python, Lua, etc) wrapping module code 
        have a 'init' function, defined last in the code, which initializes
        the module. You should write that here as well. */
    virtual std::string FinishFile() const = 0;
    
    
    /** These functions are called to wrap a specific kind of metadata.
        They should return whatever code block is required to wrap such things.
        Remember that to convert values between C++ and script you should use
        the <>Converter class, defined by the ConverterProvider we use.
        And also that using the <>Converter (constructor, methods, etc)  
        depends on how you defined it in the ConverterProvider. */
    
    virtual std::string WrapFunction(const md::Ptr<const md::Function>& obj) = 0;
    virtual std::string WrapVariable(const md::Ptr<const md::Variable>& obj) = 0;
    virtual std::string WrapClass(const md::Ptr<const md::Class>& obj) = 0;
    virtual std::string WrapNamespace(const md::Ptr<const md::Namespace>& obj) = 0;
    virtual std::string WrapEnum(const md::Ptr<const md::Enum>& obj) = 0;
    
    /// Gets the name of the module currently being wrapped.
    const std::string& module_name() const { return module_name_; }
    
    /// Sets the name of the module currently being wrapped.
    void set_module_name(const std::string& name) { module_name_ = name; }
    
  protected:
    std::string module_name_;
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_WRAPPERSPECIFICATION_H_


