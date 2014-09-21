#include <python/opa/pythonconverter.h>
#include <opa/exceptions.h>
#include <string>

namespace opa {
namespace python {

#define WRAPPER_MSG_ADDENDUM(isReturn) \
    ((in_wrapper_)  \
        ? ((isReturn)    \
            ? " [in Return value]"    \
            : " [in parameter "+std::to_string(arg_index_)+"]") \
        : "")

#define RAISE(message, isReturn) \
    std::string error_msg = "[Converter] "; \
    error_msg += message+WRAPPER_MSG_ADDENDUM(isReturn); \
    throw InternalVMError("Python", error_msg)

#define CHECK_NULL(obj, message) \
    if (obj == nullptr) { RAISE(message, false); }

#define CHECK_NULL_TO_SCRIPT(obj, message) \
    if (obj == nullptr) { RAISE(message, true); }

#define CHECK_PY_ERROR(message) \
    if ( PyErr_Occurred() != nullptr) { \
        RAISE(message, false); \
    }

//defining the static wrapped types map.
std::map<std::type_index, PyTypeObject* > PythonConverter::wrapped_types_ ;
std::unordered_set<PyTypeObject* > PythonConverter::types_set_;

//////////////////
// script -> C++ methods
bool PythonConverter::ScriptToBool(PyObject* value) const {
    return !!PyObject_IsTrue(value);
}

short PythonConverter::ScriptToShort(PyObject* value) const {
    return static_cast<short>(ScriptToInt(value));
}

int PythonConverter::ScriptToInt(PyObject* value) const {
    int v = static_cast<int>(PyInt_AsLong(value));
    CHECK_PY_ERROR("error occurred while trying to get int value from script");
    return v;
}

long PythonConverter::ScriptToLong(PyObject* value) const {
    long v = PyLong_AsLong(value);
    CHECK_PY_ERROR("error occurred while trying to get long value from script");
    return v;
}

float PythonConverter::ScriptToFloat(PyObject* value) const {
    return static_cast<float>(ScriptToDouble(value));
}

double PythonConverter::ScriptToDouble(PyObject* value) const {
    double d = PyFloat_AsDouble(value);
    CHECK_PY_ERROR("error occurred while trying to get double value from script");
    return d;
}

char PythonConverter::ScriptToChar(PyObject* value) const {
    return static_cast<char>(ScriptToInt(value));
}

const char* PythonConverter::ScriptToCStr(PyObject* value) const {
    char* s = PyString_AsString(value);
    CHECK_NULL(s, "error getting string value from script");
    return s;
}

/////////////
//  C++ -> script methods
PyObject* PythonConverter::BoolToScript(bool value) {
    PyObject* b = PyBool_FromLong(static_cast<long>(value));
    CHECK_NULL_TO_SCRIPT(b, "could not convert boolean to script");
    return b; // returns new ref
}

PyObject* PythonConverter::ShortToScript(short value) {
    return IntToScript(static_cast<int>(value)); // returns new ref
}

PyObject* PythonConverter::IntToScript(int value) {
    PyObject* l = PyInt_FromLong(static_cast<long>(value));
    CHECK_NULL_TO_SCRIPT(l, "could not convert int to script");
    return l; // returns new ref
}

PyObject* PythonConverter::LongToScript(long value) {
    PyObject* l = PyLong_FromLong(value);
    CHECK_NULL_TO_SCRIPT(l, "could not convert long to script");
    return l; // returns new ref
}

PyObject* PythonConverter::FloatToScript(float value) {
    return DoubleToScript(static_cast<double>(value)); // returns new ref
}

PyObject* PythonConverter::DoubleToScript(double value) {
    PyObject* d = PyFloat_FromDouble(value);
    CHECK_NULL_TO_SCRIPT(d, "could not convert double to script");
    return d; // returns new ref
}

PyObject* PythonConverter::CharToScript(char value) {
    return IntToScript(static_cast<int>(value)); // returns new ref
}

PyObject* PythonConverter::CStrToScript(const char* value) {
    PyObject* s = PyString_FromString(value);
    CHECK_NULL_TO_SCRIPT(s, "could not convert string (C str) to script");
    return s; // returns new ref
}

} //namespace python
} //namespace opa
