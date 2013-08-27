/**
The following is an example of how to implement a converter for a 'Dummy' language,
inheriting from this base class and following our Converter conventions.
---------------------------------------------------------------------------------

#this is whatever you want to represent a value from script (ie, in python it is PythonObj*)
struct ScriptObj {}

DummyConverter : public Converter<ScriptObj> {
    #implement constructors and destructor as see fit

    #implement inherited abstract methods (ScriptToBool, BoolToScript, ...)

    CONVERTER_IMPL_TYPE_TO_SCRIPT(ScriptObj) {
        #implement templatized TypeToScript<T> method
        #it receives a T value parameter and should return a ScriptObj
    }
    
    CONVERTER_IMPL_SCRIPT_TO_TYPE(ScriptObj) {
        #implement templatized ScriptToType<T> method
        #it receives a ScriptObj value parameter and should return a T
    }
};

PRIMITIVE_CONVERTER_TEMPLATES(DummyConverter, ScriptObj)
-----------------------------------------------------------------------------------
*/

#ifndef OUROBOROS_SCRIPT_CONVERTER_H_
#define OUROBOROS_SCRIPT_CONVERTER_H_

#include <string>
#include <exception>

//used internally
#define CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, CppType, CppTypeName) \
    template<> \
    inline ScriptType Class::TypeToScript<CppType>(CppType value) { \
        return CppTypeName##ToScript (value); \
    } \
    template<> \
    inline CppType Class::ScriptToType<CppType>(ScriptType value) const { \
        return ScriptTo##CppTypeName (value); \
    } 

// Defines to/from template specialization for primitive values
#define PRIMITIVE_CONVERTER_TEMPLATES(Class, ScriptType) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, bool, Bool) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, short, Short) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, int, Int) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, long, Long) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, float, Float) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, double, Double) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, char, Char) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, const char*, CStr) \
    
// Defines the signature of the TypeToScript template method
#define CONVERTER_IMPL_TYPE_TO_SCRIPT(ScriptType) \
    template <typename T> \
    ScriptType TypeToScript(T value)
  
// Defines the signature of the ScriptToType template method  
#define CONVERTER_IMPL_SCRIPT_TO_TYPE(ScriptType) \
    template <typename T> \
    T ScriptToType(ScriptType value) const 
    
namespace opa {

/** Base class for a converter for a scripting language.
    The abstract methods defined here allow for conversion of
    simple types to and from the scripting language. */
template <typename T>
class Converter {
  public:
    virtual ~Converter() {}
    
    virtual bool        ScriptToBool    (T t) const = 0;
    virtual short       ScriptToShort   (T t) const = 0;
    virtual int         ScriptToInt     (T t) const = 0;
    virtual long        ScriptToLong    (T t) const = 0;
    virtual float       ScriptToFloat   (T t) const = 0;
    virtual double      ScriptToDouble  (T t) const = 0;
    virtual char        ScriptToChar    (T t) const = 0;
    virtual const char* ScriptToCStr    (T t) const = 0;
    
    virtual T BoolToScript      (bool value) = 0;
    virtual T ShortToScript     (short value) = 0;
    virtual T IntToScript       (int value) = 0;
    virtual T LongToScript      (long value) = 0;
    virtual T FloatToScript     (float value) = 0;
    virtual T DoubleToScript    (double value) = 0;
    virtual T CharToScript      (char value) = 0;
    virtual T CStrToScript      (const char* value) = 0;
 
  protected:
    Converter() {}
};

/** Class representing a conversion exception. To be used by the converter
    if they need to throw errors. */
class ConversionError : public std::exception {
  public:
    ConversionError (const char* wrapper_name, const std::string& message)
        : wrapper_name_(wrapper_name), message_(message) {}
    ConversionError (const char* wrapper_name, const char* message)
        : wrapper_name_(wrapper_name), message_(message) {}
    
    virtual const char* what() const throw() override {
        return ("[Error in "+wrapper_name_+"Converter] "+message_).c_str();
    }
  
  protected:
    std::string wrapper_name_;
    std::string message_;
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_CONVERTER_H_ */
