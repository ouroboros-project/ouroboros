#ifndef OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_
#define OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_

#include <Python.h>
#include <opa/converter.h>
#include <opa/exceptions.h>
#include <string>
#include <typeinfo>

namespace opa {
namespace python {

class PythonConverter : public Converter<PyObject*> {
public:
    PythonConverter(bool in_wrapper_code=false) : in_wrapper_(in_wrapper_code) {}
    virtual ~PythonConverter() {}

    virtual bool        ScriptToBool    (PyObject* value) const;
    virtual short       ScriptToShort   (PyObject* value) const;
    virtual int         ScriptToInt     (PyObject* value) const;
    virtual long        ScriptToLong    (PyObject* value) const;
    virtual float       ScriptToFloat   (PyObject* value) const;
    virtual double      ScriptToDouble  (PyObject* value) const;
    virtual char        ScriptToChar    (PyObject* value) const;
    virtual const char* ScriptToCStr    (PyObject* value) const;
    
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
        throw InternalVMError("Python", "[Converter] conversion method TypeToScript<"+tname+"> not implemented");
    }
    
    CONVERTER_IMPL_SCRIPT_TO_TYPE(PyObject*) {
        //implement templatized ScriptToType<T> method
        //it receives a ScriptObj value parameter and should return a T
        throw InternalVMError("Python", "[Converter] conversion method ScriptToType<T> not implemented");
    }
    
    template <typename T>
    T PyArgToType(PyObject* args, int index) {
        PyObject* arg = PyTuple_GetItem(args, static_cast<Py_ssize_t>(index));
        if (arg == nullptr) throw InternalVMError("Python", "[Converter] Failed to get ARGS tuple element at index "+std::to_string(index));
        arg_index_ = index;
        return ScriptToType<T>(arg);
    }

private:
    bool in_wrapper_;
    int arg_index_;
};

PRIMITIVE_CONVERTER_TEMPLATES(PythonConverter, PyObject*)

}
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_ */
