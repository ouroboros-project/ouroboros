#ifndef OUROBOROS_SCRIPT_PYTHON_MODULES_H_
#define OUROBOROS_SCRIPT_PYTHON_MODULES_H_

#include <opa/script.h>
#include <languages/python/pythonwrapper.h>

namespace opa {
namespace python {

void AddModule(const Module<inittype> &);

void RegisterModules(PythonWrapper* wrapper);

} /* namespace python */
} /* namespace opa */


#endif /* OUROBOROS_SCRIPT_PYTHON_MODULES_H_ */
