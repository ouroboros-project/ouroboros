
#ifndef OPWIG_GEN_CONVERTERPROVIDER_H_
#define OPWIG_GEN_CONVERTERPROVIDER_H_

#include <string>

namespace opwig {
namespace gen {

/// Base class that produces the code strings required to write a 
/// class (named '<>Converter', where '<>' is the name of the wrapper
/// that is using this ConverterProvider) that converts values from a
/// scripting language to and from C++.
class ConverterProvider {

  public:
    /// constructor
    ConverterProvider() {}
    
    /// Virtual destructor.
    virtual ~ConverterProvider() {}

    /// Gets the constructor code block of the Converter.
    /// The text returned here should define the constructor(s) of the
    /// Converter class.
    virtual std::string GetConstructorCode() const = 0;
    
    /// Gets the destructor code block of the Converter.
    /// The text returned here should define the destructor of the
    /// Converter class. As the the Converter does not derive from
    /// anything and is final, there's no need for the destructor to be virtual.
    virtual std::string GetDestructorCode() const = 0;
    
    /// Gets the 'to script' conversion methods code block for the Converter.
    /// This should return definition of methods that perform the steps required 
    /// to convert a C++ type to script.
    /// It is advised to make them as easy as possible to use in the wrapping functions.
    /// Such as making them templatized methods, possibly with specialization for
    /// for specific types such as primitives or STL containers.
    virtual std::string GetToFunctionsCode() const = 0;
    
    /// Gets the 'from script' conversion methods code block for the Converter.
    /// This should return definition of methods that perform the steps required 
    /// to convert a script object to a C++ type.
    /// It is advised to make them as easy as possible to use in the wrapping functions.
    /// Such as making them templatized methods, possibly with specialization for
    /// for specific types such as primitives or STL containers.
    virtual std::string GetFromFunctionsCode() const = 0;
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_CONVERTERPROVIDER_H_


