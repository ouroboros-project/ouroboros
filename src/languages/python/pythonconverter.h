#ifndef OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_
#define OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_

#include <Python.h>

#include <string>
#include <map>
#include <typeinfo>
#include <typeindex>

#include <opa/converter.h>
#include <opa/exceptions.h>
#include <languages/python/wrapperbase.h>


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
        return Conversion<T>().TypeToScript(value);
    }
    
    CONVERTER_IMPL_SCRIPT_TO_TYPE(PyObject*) {
        //implement templatized ScriptToType<T> method
        //it receives a ScriptObj value parameter and should return a T
        return Conversion<T>().ScriptToType(value);
    }
    
    template <typename T>
    T PyArgToType(PyObject* args, int index) {
        PyObject* arg = PyTuple_GetItem(args, static_cast<Py_ssize_t>(index));
        if (arg == nullptr) throw InternalVMError("Python", "[Converter] Failed to get ARGS tuple element at index "+std::to_string(index));
        arg_index_ = index;
        return ScriptToType<T>(arg);
    }

    /** Registers a python type that is a wrap to a C++ type, in order to
        enable conversion of said type between C++ and Python. */
    static void RegisterWrappedType(const std::type_index& type_id, PyTypeObject* py_type) {
        wrapped_types_[type_id] = py_type;
    }

private:
    bool in_wrapper_;
    int arg_index_;

    static std::map<std::type_index, PyTypeObject* > wrapped_types_;

    template <typename T>
    class Conversion;
};

PRIMITIVE_CONVERTER_TEMPLATES(PythonConverter, PyObject*)

/// basic generic conversion implementation
template <typename T>
class PythonConverter::Conversion {
public:
    PyObject* TypeToScript(T value) {
        std::string tname (typeid(value).name());
        throw InternalVMError("Python", "[Converter] conversion method TypeToScript<"+tname+"> not implemented");
    }
    
    T ScriptToType(PyObject* value) {
        std::string tname (typeid(T).name());
        throw InternalVMError("Python", "[Converter] conversion method ScriptToType<"+tname+"> not implemented");
    }
};

/// generic user_class* conversion implementation
template <typename T>
class PythonConverter::Conversion<T*> {
public:
    PyObject* TypeToScript(T* value) {
        /*
        -if value == nullptr, return PyNone
        -get PyTypeObject of T  [WAT]
        -use tp_new/tp_alloc to allocate python object, place value in it and return it
        */
        std::string tname (typeid(T).name());
        throw InternalVMError("Python", "[Converter] conversion method TypeToScript<"+tname+"> not implemented FOR T* PARTIAL");
        return nullptr;
    }
    
    T* ScriptToType(PyObject* value) {
        /*
        -check if value is PyNone, if so, return nullptr
        -check if value's type is of one of ours  [WAT]
        -cast value to OPWIGPyObject
        -check value type_id (from OPWIGPyObject) to typeid(T)
        -if type ids match, cast value->obj to T and return it.
        */
        std::string tname (typeid(T).name());
        throw InternalVMError("Python", "[Converter] conversion method ScriptToType<"+tname+"> not implemented FOR T* PARTIAL");
        return nullptr;
    }
};

}
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_ */
