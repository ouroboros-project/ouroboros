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
    inline ScriptType Class::TypeToScript<CppType>(CppType value) { \
        return CppTypeName##ToScript (value); \
    } \
    template<> \
    inline CppType Class::ScriptToType<CppType>(ScriptType value) { \
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
    CONVERTER_TEMPLATE_SPECIALIZATION(Class, ScriptType, const char*, CStr) \
    
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
    
    virtual bool        ScriptToBool    (T t) = 0;
    virtual short       ScriptToShort   (T t) = 0;
    virtual int         ScriptToInt     (T t) = 0;
    virtual float       ScriptToFloat   (T t) = 0;
    virtual double      ScriptToDouble  (T t) = 0;
    virtual char        ScriptToChar    (T t) = 0;
    virtual const char* ScriptToCStr    (T t) = 0;
    
    virtual T BoolToScript      (bool value) = 0;
    virtual T ShortToScript     (short value) = 0;
    virtual T IntToScript       (int value) = 0;
    virtual T FloatToScript     (float value) = 0;
    virtual T DoubleToScript    (double value) = 0;
    virtual T CharToScript      (char value) = 0;
    virtual T CStrToScript      (const char* value) = 0;
 
  protected:
    Converter() {}
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_CONVERTER_H_ */
