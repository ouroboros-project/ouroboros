#ifndef OUROBOROS_SCRIPT_PYTHON_PYTHONDATA_H_
#define OUROBOROS_SCRIPT_PYTHON_PYTHONDATA_H_

#include <Python.h>
#include <vector>
#include <opa/virtualdata.h>
#include <python/opa/pythonmachine.h>
#include <python/opa/pythonconverter.h>

namespace opa {

class VirtualObj;
class VirtualType;

namespace python {

class PythonData : public VirtualData {
  public:
    PythonData(PythonMachine* pyvm, PyObject* data_object, bool owns_ref) : VirtualData(), 
        machine_(pyvm), own_ref_(owns_ref), py_data_(data_object) {}
        
    virtual ~PythonData() {
        if (own_ref_)
            Py_XDECREF(py_data_);
    }

    /// Tries to unwrap the data contained in this object using the given type.
    virtual void* Unwrap(const VirtualType& type, bool disown) const;
    const char* UnwrapString() const;
    bool UnwrapBoolean() const;
    int UnwrapInteger() const;
    double UnwrapNumber() const;
    Vector UnwrapVector() const;
    List UnwrapList() const;
    Map UnwrapMap() const;

    /// Tries to wrap the given data with the given type into this object.
    virtual void Wrap(void* data, const VirtualType& type);
    virtual void WrapString(const char* str);
    virtual void WrapBoolean(bool boolean);
    virtual void WrapInteger(int number);
    virtual void WrapNumber(double number);

    virtual VirtualMachine* machine () const { return machine_; }

    /// Tries to execute ourselves as a function in a script language,
    /// passing thee given arguments and returning the result.
    virtual Ptr Execute(const std::vector<Ptr>& args);

    /// Tries to get a attribute with the given name from this object.
    virtual Ptr GetAttribute(Ptr key);

    virtual Ptr SetAttribute(Ptr key, Ptr value);
    
    void* unsafe_data() const {
        return static_cast<void*>(py_data_);
    }

  protected:
    PythonMachine* machine_;
    bool own_ref_; //if true, we own a ref to our PyObject* (py_data_), so we need to DECREF it in due time.
    PyObject* py_data_;
    PythonConverter converter_;
};

}
} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_PYTHON_PYTHONDATA_H_ */
