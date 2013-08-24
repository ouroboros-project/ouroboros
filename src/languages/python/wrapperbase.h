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

///> OPEN CLASS
typedef struct _PyWrapBaseObject {
    PyObject_HEAD
    // atributos de um objeto padrão de wrap
    void* obj;
    
    // esses atributos possivelmente teriam q ser gerados pra cada tipo, em outras structs
    typedef TIPO type;   // isso não altera o tamanho retornado pelo sizeof(essa struct)

    void construct(PyObject* args, PyObject* kwds) {
        COISAS
    }
} PyWrapBase;

///> wrap de metodos/ getters e setters de atributos


///> CLOSE CLASS

TABELA_METODOS
TABELA_GETSETs

static PyTypeObject PyWrapBaseType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "wat.PyWrapBase",          /*tp_name*/
    sizeof(PyWrapBase),        /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Noddy_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    (reprfunc)Noddy_repr,      /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Base python type for wrapped C++ types",           /* tp_doc */
    0,                      /* tp_traverse */
    0,                      /* tp_clear */
    0,		                /* tp_richcompare */
    0,		                /* tp_weaklistoffset */
    0,		                /* tp_iter */
    0,		                /* tp_iternext */
    0,             /* tp_methods */
    0,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    Noddy_new,                 /* tp_new */
};

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
