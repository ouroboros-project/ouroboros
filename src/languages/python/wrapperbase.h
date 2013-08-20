#ifndef OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_
#define OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_

#include <Python.h>

namespace opa {
namespace python {
namespace wrapper {

typedef struct {
    PyObject_HEAD
    // atributos de um objeto padrão de wrap
    void* obj;
    
    // esses atributos possivelmente teriam q ser gerados pra cada tipo, em outras structs
    typedef TIPO type;   // isso não altera o tamanho retornado pelo sizeof(essa struct)

} PyWrapBase;

static PyTypeObject PyWrapBaseType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "wat.PyWrapBase",          /*tp_name*/
    sizeof(PyWrapBase),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Noddy_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, /*tp_flags*/
    "Base python type for wrapped C++ types",           /* tp_doc */
    (traverseproc)Noddy_traverse,   /* tp_traverse */
    (inquiry)Noddy_clear,           /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Noddy_methods,             /* tp_methods */
    Noddy_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Noddy_init,      /* tp_init */
    0,                         /* tp_alloc */
    Noddy_new,                 /* tp_new */
};

} /* namespace wrapper */
} /* namespace python */
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_WRAPPERBASE_H_ */
