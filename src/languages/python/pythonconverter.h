#ifndef OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_
#define OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_

#include <Python.h>
#include <opa/converter.h>
#include <string>

namespace opa {
namespace python {

class PythonConverter : public Converter<PyObject*> {
public:
    PythonConverter() {}
    virtual ~PythonConverter() {}

    virtual bool        ScriptToBool    (PyObject* value);
    virtual short       ScriptToShort   (PyObject* value);
    virtual int         ScriptToInt     (PyObject* value);
    virtual long        ScriptToLong    (PyObject* value);
    virtual float       ScriptToFloat   (PyObject* value);
    virtual double      ScriptToDouble  (PyObject* value);
    virtual char        ScriptToChar    (PyObject* value);
    virtual const char* ScriptToCStr    (PyObject* value);
    
    virtual PyObject* BoolToScript      (bool value);
    virtual PyObject* ShortToScript     (short value);
    virtual PyObject* IntToScript       (int value);
    virtual PyObject* LongToScript      (long value);
    virtual PyObject* FloatToScript     (float value);
    virtual PyObject* DoubleToScript    (double value);
    virtual PyObject* CharToScript      (char value);
    virtual PyObject* CStrToScript      (const char* value);

    CONVERTER_IMPL_TYPE_TO_SCRIPT(PyObject*) {
        //implement templatized TypeToScript<T> method
        //it receives a T value parameter and should return a ScriptObj
        std::string tname (typeid(value).name());
        throw ConversionError("Python", "conversion method TypeToScript<"+tname+"> not implemented");
    }
    
    CONVERTER_IMPL_SCRIPT_TO_TYPE(PyObject*) {
        //implement templatized ScriptToType<T> method
        //it receives a ScriptObj value parameter and should return a T
        throw ConversionError("Python", "conversion method ScriptToType<T> not implemented");
    }
    
    template <typename T>
    T PyArgToType(PyObject* args, int index) {
        PyObject* arg = PyTuple_GetItem(args, static_cast<Py_ssize_t>(index));
        std::string i
        if (arg == nullptr) throw ConversionError("Python", "Failed to get ARGS tuple element at index "+std::to_string(index));
        return ScriptToType<T>(arg);
    }
};

PRIMITIVE_CONVERTER_TEMPLATES(PythonConverter, PyObject*)

}
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_ */
