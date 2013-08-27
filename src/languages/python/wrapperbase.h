#ifndef OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_
#define OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_

#include <Python.h>

namespace opa {
namespace python {
namespace wrapper {

template <typename T>
PyObject* GenericNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    T* self;
    self = (T*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->construct(args, kwds);
        if (self->obj == nullptr)
        {
            // solta exceção em python?
            Py_DECREF(self);
            return NULL;
        }
    }
    else {
        // solta exceção python?
    }
    return (PyObject*)self;
}

template <typename T>
void GenericDealloc(T* self)
{
    delete static_cast<T*>(self->obj);
    self->ob_type->tp_free((PyObject*)self);
}

template <typename T>
PyObject* GenericRepr(T* self)
{
    return PyString_FromFormat("<OuroborosWrap: instance of '%s' at %d>",
                               self->ob_type->tp_name, (int)self->obj);
}

/******************/
/*** COISAS PRA GERAR **/

///> wrap de metodos/ getters e setters de atributos


///> CLOSE CLASS


bool TypeReady (PyTypeObject* type_obj) {
    //se tiver que inicializar coisa do tipo pode colocar aqui
    if (PyType_Ready(type_obj) < 0) {
        // solta exceção em python?
        return false;
    }
    return true;
}

} /* namespace wrapper */
} /* namespace python */
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_ */
