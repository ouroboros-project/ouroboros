#include <languages/python/pythonconverter.h>

namespace opa {
namespace python {

#define RAISE(message) throw ConversionError("Python", message)

#define CHECK_NULL(obj, message) \
    if (obj == nullptr) RAISE(message)

//////////////////
// script -> C++ methods
bool PythonConverter::ScriptToBool(PyObject* value) {
    return !!PyObject_IsTrue(value);
}

short PythonConverter::ScriptToShort(PyObject* value) {
    return static_cast<short>(ScriptToInt(value));
}

int PythonConverter::ScriptToInt(PyObject* value) {
    int v = static_cast<int>(PyInt_AsLong(value));
    if ( PyErr_Occurred() != nullptr )
        RAISE("error occurred while trying to get int value from script");
    return v;
}

long PythonConverter::ScriptToLong(PyObject* value) {
    long v = PyLong_AsLong(value);
    if ( PyErr_Occurred() != nullptr )
        RAISE("error occurred while trying to get long value from script");
    return v;
}

float PythonConverter::ScriptToFloat(PyObject* value) {
    return static_cast<float>(ScriptToDouble(value));
}

double PythonConverter::ScriptToDouble(PyObject* value) {
    double d = PyFloat_AsDouble(value);
    if ( PyErr_Occurred() != nullptr )
        RAISE("error occurred while trying to get double value from script");
    return d;
}

char PythonConverter::ScriptToChar(PyObject* value) {
    return static_cast<char>(ScriptToInt(value));
}

const char* PythonConverter::ScriptToCStr(PyObject* value) {
    char* s = PyString_AsString(value);
    CHECK_NULL(s, "error getting string value from script");
    return s;
}

/////////////
//  C++ -> script methods
PyObject* PythonConverter::BoolToScript(bool value) {
    PyObject* b = PyBool_FromLong(static_cast<long>(value));
    CHECK_NULL(b, "could not convert boolean to script");
    return b; // returns new ref
}

PyObject* PythonConverter::ShortToScript(short value) {
    return IntToScript(static_cast<int>(value)); // returns new ref
}

PyObject* PythonConverter::IntToScript(int value) {
    PyObject* l = PyInt_FromLong(static_cast<long>(value));
    CHECK_NULL(l, "could not convert int to script");
    return l; // returns new ref
}

PyObject* PythonConverter::LongToScript(long value) {
    PyObject* l = PyLong_FromLong(value);
    CHECK_NULL(l, "could not convert long to script");
    return l; // returns new ref
}

PyObject* PythonConverter::FloatToScript(float value) {
    return DoubleToScript(static_cast<double>(value)); // returns new ref
}

PyObject* PythonConverter::DoubleToScript(double value) {
    PyObject* d = PyFloat_FromDouble(value);
    CHECK_NULL(d, "could not convert double to script");
    return d; // returns new ref
}

PyObject* PythonConverter::CharToScript(char value) {
    return IntToScript(static_cast<int>(value)); // returns new ref
}

PyObject* PythonConverter::CStrToScript(const char* value) {
    PyObject* s = PyString_FromString(value);
    CHECK_NULL(s, "could not convert string (C str) to script");
    return s; // returns new ref
}

} //namespace python
} //namespace opa
