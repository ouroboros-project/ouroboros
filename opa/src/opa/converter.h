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

/// Utility macro to define specializations to a templatized conversion method in the Converter class.
/** This macro defines specializations for the primitive types to the template
    methods ScriptToType (conversion script -> C++) and TypeToScript (C++ -> script).
    These specializations call the ScriptTo<PrimitiveName> or <PrimitiveName>ToScript methods of the
    Converter class, which the given class should derive from.
* @param Class is the class to define the specializations in. It should define the template methods
    and derive from Converter, implementing its interface.
* @param ScriptType is the C++ type representing a script value (used as the template parameter of
    the Converter class).
* @see opa::Converter
* @see CONVERTER_IMPL_TYPE_TO_SCRIPT
* @see CONVERTER_IMPL_SCRIPT_TO_TYPE
*/
#define PRIMITIVE_CONVERTER_TEMPLATES(Class, ScriptType) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, bool, Bool) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, short, Short) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, int, Int) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, long, Long) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, float, Float) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, double, Double) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, char, Char) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, const char*, CStr) \
    
/// Utility macro to define the signature of a templatized C++->Script conversion method.
/** This macro defines the signature for a templatized C++ -> Script conversion method, named
    TypeToScript (used by the specializations created by PRIMITIVE_CONVERTER_TEMPLATES).
    This template method, which should be defined in a class that derives from Converter,
    received a C++ object (from type T - the template parameter), and should return a script 
    value (of type ScriptType).
    Note that this macro defines only the signature, the body of the template function (along 
    with the brackets) should be defined by the user after using this macro.
* @param ScriptType is the C++ type representing a script value (used as the template parameter of
    the Converter class).
* @see opa::Converter
* @see PRIMITIVE_CONVERTER_TEMPLATES
*/
#define CONVERTER_IMPL_TYPE_TO_SCRIPT(ScriptType) \
    template <typename T> \
    ScriptType TypeToScript(T value)
  
/// Utility macro to define the signature of a templatized Script->C++ conversion method.
/** This macro defines the signature for a templatized Script -> C++ conversion method, named
    ScriptToType (used by the specializations created by PRIMITIVE_CONVERTER_TEMPLATES).
    This template method, which should be defined in a class that derives from Converter,
    received a script value object (from type ScriptType), and should return a C++ object 
    value (of type T - the template parameter).
    Note that this macro defines only the signature, the body of the template function (along 
    with the brackets) should be defined by the user after using this macro.
* @param ScriptType is the C++ type representing a script value (used as the template parameter of
    the Converter class).
* @see opa::Converter
* @see PRIMITIVE_CONVERTER_TEMPLATES
*/
#define CONVERTER_IMPL_SCRIPT_TO_TYPE(ScriptType) \
    template <typename T> \
    T ScriptToType(ScriptType value) const 
    
namespace opa {

/// Base class for a C++/scripting language value converter.
/** Base class for a converter for a scripting language.
    The abstract methods defined here allow for conversion of
    simple types to and from the scripting language and C++. 
* @tparam T a type that represents a value in the scripting language 
    (contains data to identify and convert a script value).
*/
template <typename T>
class Converter {
  public:
    virtual ~Converter() {}
    
    /// Converts a script value to a boolean.
    virtual bool        ScriptToBool    (T t) const = 0;
    /// Converts a script value to a short.
    virtual short       ScriptToShort   (T t) const = 0;
    /// Converts a script value to a int.
    virtual int         ScriptToInt     (T t) const = 0;
    /// Converts a script value to a long.
    virtual long        ScriptToLong    (T t) const = 0;
    /// Converts a script value to a float.
    virtual float       ScriptToFloat   (T t) const = 0;
    /// Converts a script value to a double.
    virtual double      ScriptToDouble  (T t) const = 0;
    /// Converts a script value to a char.
    virtual char        ScriptToChar    (T t) const = 0;
    /// Converts a script value to a null-terminated C string.
    virtual const char* ScriptToCStr    (T t) const = 0;
    
    /// Converts a boolean to a script value.
    virtual T BoolToScript      (bool value) = 0;
    /// Converts a short to a script value.
    virtual T ShortToScript     (short value) = 0;
    /// Converts a int to a script value.
    virtual T IntToScript       (int value) = 0;
    /// Converts a long to a script value.
    virtual T LongToScript      (long value) = 0;
    /// Converts a float to a script value.
    virtual T FloatToScript     (float value) = 0;
    /// Converts a double to a script value.
    virtual T DoubleToScript    (double value) = 0;
    /// Converts a char to a script value.
    virtual T CharToScript      (char value) = 0;
    /// Converts a C string to a script value.
    virtual T CStrToScript      (const char* value) = 0;
 
  protected:
    Converter() {}
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_CONVERTER_H_ */
