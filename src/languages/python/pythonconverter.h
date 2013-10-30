#ifndef OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_
#define OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_

#include <Python.h>

#include <string>
#include <map>
#include <unordered_set>
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
        if (!types_set_.insert(py_type).second) {
            std::string tname (type_id.name());
            throw InternalVMError("Python", "[Converter] could not register wrapped type '"+tname+"'.");
        }
    }

private:
    bool in_wrapper_;
    int arg_index_;

    static std::map<std::type_index, PyTypeObject* > wrapped_types_;
    static std::unordered_set<PyTypeObject* > types_set_;

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
        // special case - check if nullptr for None
        if (value == nullptr) {
            Py_RETURN_NONE;
        }
        // check if we know the target type
        if (!wrapped_types_.count(typeid(T))) {
            std::string tname (typeid(T).name());
            throw InternalVMError("Python", "[Converter] TypeToScript conversion failed - type '"+tname+"' not registered.");
        }
        // get the python type object corresponding to target type
        PyTypeObject* type = wrapped_types_[typeid(T)];
        // allocate new python object of our target python type
        PyObject* py_value = type->tp_alloc(type, 0); //TODO: this PyObject allocation/initialization might be wrong...
        if (py_value == nullptr) {
            std::string pyname (type->tp_name);
            throw InternalVMError("Python", "[Converter] TypeToScript conversion failed - could not allocate a '"+pyname+"' object.");
        }
        // cast it to one of our python objects
        wrapper::OPWIGPyObject* opwig_value = reinterpret_cast<wrapper::OPWIGPyObject*>(py_value);
        if (opwig_value->obj != nullptr)
            throw InternalVMError("Python", "[Converter] TypeToScript conversion failed - new python object with non-null obj.");
        // set the values and return the python object
        opwig_value->obj = static_cast<void*>(value);
        opwig_value->type_id = typeid(T);
        return py_value;
    }
    
    T* ScriptToType(PyObject* value) {
        // special case - check if is None
        if (value == Py_None) {
            return nullptr;
        }
        // check if we know the type we received
        if (!types_set_.count(value->ob_type)) {
            std::string pyname (value->ob_type->tp_name);
            throw InternalVMError("Python", "[Conversion] ScriptToType conversion failed - unregistered given type '"+pyname+"'.");
        }
        // cast it to one of our objects
        wrapper::OPWIGPyObject* py_value = reinterpret_cast<wrapper::OPWIGPyObject*>(value);
        // check if types match
        if (py_value->type_id != typeid(T)) {
            std::string expname (typeid(T).name());
            std::string givname (py_value->type_id.name());
            throw InternalVMError("Python",
                "[Converter] ScriptToType conversion failed - given type '"+givname+"' uncompatible with expected type '"+expname+"'");
        }
        // return contained C++ pointer
        return static_cast<T*>(py_value->obj);
    }
};

}
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_ */
