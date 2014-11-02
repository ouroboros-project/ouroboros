#include <Python.h>

#include <python/opa/pythonmachine.h>

#include <string>
#include <cstdlib>

#include <memory>

#include <opa/config.h>
#include <python/opa/pythondata.h>
#include <opa/virtualobj.h>
#include <opa/scriptmanager.h>
#include <python/opa/modules.h>

namespace opa {
namespace python {

using std::shared_ptr;
using std::string;

VirtualData::Ptr PythonMachine::NewData() {
    VirtualData::Ptr vdata( new PythonData(this, nullptr, false) ); 
    return vdata;
}

void PythonMachine::ExecuteCode(const string& code) {
    PyObject* main_mod = PyImport_AddModule("__main__"); //borrowed ref
    PyObject* main_dict = PyModule_GetDict(main_mod); //borrowed
    PyObject* result = PyRun_String(code.c_str(), Py_single_input, main_dict, main_dict); //result = new ref
    if (result == nullptr) {
        string exc_info = GetPythonExceptionDetails();
        throw InternalVMError("Python", "[ExecuteCode] raised exception:\n"+exc_info);
        return;
    }
    //TODO: we should put this in a VObj and return it... 
    Py_DECREF(result);
}

VirtualObj PythonMachine::LoadModule(const std::string& name) {
    std::string dotted_name = SCRIPT_MANAGER()->ConvertPathToDottedNotation(name);
    PyObject* module = PyImport_ImportModule(dotted_name.c_str()); //new ref
    if (module == nullptr) {
        string exc_info = GetPythonExceptionDetails();
        throw InternalVMError("Python", "[LoadModule] module '"+dotted_name+"' raised exception:\n"+exc_info);
        return VirtualObj();
    }
#ifdef DEBUG
    printf("[Python] Loaded module '%s'.\n", dotted_name.c_str());
#endif
    VirtualData::Ptr vdata( new PythonData(this, module, true) ); //PythonData takes care of the ref.
    return VirtualObj(vdata);
}

/// Initializes the VirtualMachine (that is, the language's API. Returns bool telling if (true=) no problems occured.
bool PythonMachine::Initialize() {
#ifdef WIN32
    Py_NoSiteFlag = 1;
#endif
    Py_Initialize();

    PyObject *path = PySys_GetObject("path");
    PyList_Append(path, PyString_FromString( SCRIPT_MANAGER()->scripts_path().c_str() ));

    RegisterModules(this);

    std::vector<PythonModule>::iterator it;
    for (it = modules_.begin(); it != modules_.end(); ++it) {
        (*it->init_func())();
    }
    return true;
}

/// Finalizes the VirtualMachine, finalizing any language specific stuff.
void PythonMachine::Finalize() {
    Py_Finalize();
}

string PythonMachine::GetPythonExceptionDetails() {
    if(PyErr_Occurred() == nullptr) {
        puts("No Exception.");
        return "<no exception>";
    }
    PyObject *temp, *exc_typ, *exc_val, *exc_tb;
    PyErr_Fetch(&exc_typ,&exc_val,&exc_tb);
    PyErr_NormalizeException(&exc_typ,&exc_val,&exc_tb);

    string info = "";
    temp = PyObject_GetAttrString(exc_typ, "__name__");
    if (temp != nullptr) {
        //fprintf(stderr, "%s: ", PyString_AsString(temp));
        info.append( PyString_AsString(temp) );
        info += ": ";
        Py_DECREF(temp);
    }
    Py_DECREF(exc_typ);

    if(exc_val != nullptr) {
        temp = PyObject_Str(exc_val);
        if (temp != nullptr) {
            //fprintf(stderr, "%s", PyString_AsString(temp));
            info.append( PyString_AsString(temp) );
            info += "\n";
            Py_DECREF(temp);
        }
        Py_DECREF(exc_val);
    }

    //fprintf(stderr, "\n");
    if(exc_tb == nullptr) {
        if (info.size() <= 0) return "<no information available (no traceback)>";
        return info;
    }
    
    PyObject *pName = PyString_FromString("traceback");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if(pModule == nullptr) {
        if (info.size() <= 0) return "<no information available (no traceback module)>";
        return info;
    }

    PyObject *pFunc = PyObject_GetAttrString(pModule, "format_tb");
    
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject *pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, exc_tb);

        PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != nullptr) {
            Py_ssize_t len = PyList_Size(pValue);
            PyObject *t;
            for (Py_ssize_t i = 0; i < len; i++) {
                PyObject *tt = PyList_GetItem(pValue,i);
                t = Py_BuildValue("(O)",tt);
                
                char *buffer;
                if(!PyArg_ParseTuple(t,"s",&buffer)) break;

                string line (buffer);
                info = info + line + "\n";
            }
        }
        Py_DECREF(pValue);
        Py_DECREF(pArgs);
    }
    Py_DECREF(pFunc);

    Py_DECREF(pModule);
    if (info.size() <= 0) return "<no information available (N/A)>";
    return info;
}

}
}
