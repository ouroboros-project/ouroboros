#include <python/opwig/wrapperbase.h>

#include <Python.h>

#include <iostream>
#include <string>
#include <exception>

#include <python/opa/pythonconverter.h>

namespace opa {
namespace python {
namespace wrapper {

using std::cout;
using std::endl;
using std::string;
using std::to_string;

PyObject* GenericRepr(OPWIGPyObject* self)
{
    return PyString_FromFormat("<OuroborosWrap: instance of '%s' at %p>",
                               self->ob_type->tp_name, self->obj);
}

bool NumArgsOk(PyObject* args, int num) {
    if (static_cast<int>(PyTuple_Size(args)) != num) {
        string msg = "expected "+to_string(num)+" parameters, but received "+to_string(PyTuple_Size(args))+".";
        PyErr_SetString(PyExc_RuntimeError, msg.c_str());
        return false;
    }
    return true;
}

void AddToParentModule(PyObject* mChild, const string& childName, const string& fullParentName) {
    PyObject* mParent = PyImport_ImportModule(fullParentName.c_str()); //newref
    if (mParent == nullptr) {
        string msg = "Initializing '"+childName+"', could not import '"+fullParentName+"'";
        PyErr_SetString(PyExc_RuntimeError, msg.c_str());
    }
    else {
        Py_INCREF(mChild);
        if (PyModule_AddObject(mParent, childName.c_str(), mChild) == -1) {
            string msg = "could not add submodule '"+childName+"' to module '"+fullParentName+"'";
            PyErr_SetString(PyExc_RuntimeError, msg.c_str());
        }
        Py_DECREF(mParent);
    }
}

PyObject* FuncErrorHandling(const std::exception& e) {
    cout << "[Python - ERROR IN C++ function wrapping]" << endl;
    if (PyErr_Occurred()==nullptr)   PyErr_SetString(PyExc_RuntimeError, e.what());
    return nullptr;
}

void AddTypeToModule(PyObject* module, const char* typeName, PyTypeObject* type, const std::type_index& type_id) {
    if (PyType_Ready(type) < 0)
        return;
    Py_INCREF(type);
    PyModule_AddObject(module, typeName, (PyObject*)type);
    PythonConverter::RegisterWrappedType(type_id, type);
}

} /* namespace wrapper */
} /* namespace python */
} /* namespace opa */
