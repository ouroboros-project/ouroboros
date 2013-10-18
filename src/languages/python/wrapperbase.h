#ifndef OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_
#define OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_

#include <Python.h>
#include <string>
#include <typeinfo>

namespace opa {
namespace python {
namespace wrapper {

/**************
Specialized helper functions for python type generation.
**************/

/// Generic python type __new__ method for C++ type T.
template <typename T>
PyObject* GenericNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    T* self;
    self = (T*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->construct(args, kwds);
        if (self->obj == nullptr)
        {
            // the construct method should set the appropriate exceptions
            Py_DECREF(self);
            return NULL;
        }
    }
    else {
        std::string tname (typeid(self).name());
        std::string pyname (type->tp_name);
        std::string msg = "could not allocate a new " + pyname + " [T: " + tname + "] object.";
        PyErr_SetString(PyExc_RuntimeError, msg.c_str());
    }
    return (PyObject*)self;
}

/// Generic python type dealloc method for C++ type T.
template <typename T>
void GenericDealloc(T* self)
{
    delete static_cast<typename T::type*>(self->obj);
    self->ob_type->tp_free((PyObject*)self);
}

/// Generic python type __repr__ method for C++ type T.
template <typename T>
PyObject* GenericRepr(T* self)
{
    return PyString_FromFormat("<OuroborosWrap: instance of '%s' at %p>",
                               self->ob_type->tp_name, self->obj);
}

/**************
Helper functions for other wrapping module needs.
**************/

/// Check if the number if elements in the tuple args matches the given number.
bool NumArgsOk(PyObject* args, int num);

/// Adds a module to a parent module, finding it first.
void AddToParentModule(PyObject* mChild, const string& childName, const string& fullParentName);

/// Handles C++ exceptions for python.
PyObject* FuncErrorHandling(const std::exception& e);

/// Adds a type to a module.
void AddTypeToModule(PyObject* module, const char* typeName, PyTypeObject* type);

} /* namespace wrapper */
} /* namespace python */
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_ */
