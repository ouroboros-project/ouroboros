
#ifndef OPWIG_GEN_CONVERTERPROVIDER_H_
#define OPWIG_GEN_CONVERTERPROVIDER_H_

#include <string>

namespace opwig {
namespace gen {

/// Base class that produces the code strings required to write a 
/// class (named 'Converter') that converts values from a scripting 
/// language to and from C++.
class ConverterProvider {

  public:
    /// constructor
    ConverterProvider() {}
    
    /// Virtual destructor.
    virtual ~ConverterProvider() {}


    virtual std::string GetConstructorCode() const = 0;
    virtual std::string GetDestructorCode() const = 0;
    virtual std::string GetToFunctionsCode() const = 0;
    virtual std::string GetFromFunctionsCode() const = 0;
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_CONVERTERPROVIDER_H_


