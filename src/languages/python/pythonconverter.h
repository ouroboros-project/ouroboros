#ifndef OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_
#define OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_

#include <Python.h>
#include <opa/converter.h>

namespace opa {
namespace python {

class PythonConverter : public Converter<PyObject*> {
    //implement constructors and destructor as see fit

    //implement inherited abstract methods (ScriptToBool, BoolToScript, ...)

    CONVERTER_TEMPLATE_TO_SCRIPT(PyObject*) {
        //implement templatized TypeToScript<T> method
        //it receives a T value parameter and should return a ScriptObj
    }
    
    CONVERTER_TEMPLATE_FROM_SCRIPT(PyObject*) {
        //implement templatized ScriptToType<T> method
        //it receives a ScriptObj value parameter and should return a T
    }
};

PRIMITIVE_CONVERTER_TEMPLATES(PythonConverter, PyObject*)

}
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_CONVERTER_H_ */
