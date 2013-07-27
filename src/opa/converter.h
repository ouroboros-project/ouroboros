/**
The following is an example of how to implement a converter for a 'Dummy' language,
inheriting from this base class and following our Converter conventions.
---------------------------------------------------------------------------------

#this is whatever you want to represent a value from script (ie, in python it is PythonObj*)
struct ScriptObj {}

DummyConverter : public Converter<ScriptObj> {
    #implement constructors and destructor as see fit

    #implement inherited abstract methods (ScriptToBool, BoolToScript, ...)

    CONVERTER_TEMPLATE_TO_SCRIPT(ScriptObj) {
        #implement templatized TypeToScript<T> method
        #it receives a T value parameter and should return a ScriptObj
    }
    
    CONVERTER_TEMPLATE_FROM_SCRIPT(ScriptObj) {
        #implement templatized ScriptToType<T> method
        #it receives a ScriptObj value parameter and should return a T
    }
};

PRIMITIVE_CONVERTER_TEMPLATES(DummyConverter, ScriptObj)
-----------------------------------------------------------------------------------
*/

#ifndef OUROBOROS_SCRIPT_CONVERTER_H_
#define OUROBOROS_SCRIPT_CONVERTER_H_

//used internally
#define CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, CppType, CppTypeName) \
    template<> \
    ScriptType Class::TypeToScript<CppType>(CppType value) { \
        return CppTypeName##ToScript (value); \
    } \
    template<> \
    CppType Class::ScriptToType<CppType>(ScriptType value) { \
        return ScriptTo##CppTypeName (value); \
    } 

// Defines to/from template specialization for primitive values
#define PRIMITIVE_CONVERTER_TEMPLATES(Class, ScriptType) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, bool, Bool) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, short, Short) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, int, Int) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, float, Float) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, double, Double) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, char, Char) \
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, char*, CStr) \
    
// Defines the signature of the TypeToScript template method
#define CONVERTER_TEMPLATE_TO_SCRIPT(ScriptType) \
    template <typename T> \
    ScriptType TypeToScript(T value)
  
// Defines the signature of the ScriptToType template method  
#define CONVERTER_TEMPLATE_FROM_SCRIPT(ScriptType) \
    template <typename T> \
    T ScriptToType(ScriptType value)
    
namespace opa {

/** Base class for a converter for a scripting language.
    The abstract methods defined here allow for conversion of
    simple types to and from the scripting language. */
template <typename T>
class Converter {
  public:
    virtual ~Converter() {}
    
    bool        ScriptToBool    (T t) = 0;
    short       ScriptToShort   (T t) = 0;
    int         ScriptToInt     (T t) = 0;
    float       ScriptToFloat   (T t) = 0;
    double      ScriptToDouble  (T t) = 0;
    char        ScriptToChar    (T t) = 0;
    const char* ScriptToCStr    (T t) = 0;
    
    T BoolToScript      (bool value) = 0;
    T ShortToScript     (short value) = 0;
    T IntToScript       (int value) = 0;
    T FloatToScript     (float value) = 0;
    T DoubleToScript    (double value) = 0;
    T CharToScript      (char value) = 0;
    T CStrToScript      (const char* value) = 0;
 
  protected:
    Converter() {}
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_CONVERTER_H_ */
