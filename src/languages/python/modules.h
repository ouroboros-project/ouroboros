#ifndef OUROBOROS_SCRIPT_PYTHON_MODULES_H_
#define OUROBOROS_SCRIPT_PYTHON_MODULES_H_

#include <opa/script.h>
#include <languages/python/pythonmachine.h>

namespace opa {
namespace python {

void AddModule(const Module<inittype> &);

void RegisterModules(PythonMachine* machine);

} /* namespace python */
} /* namespace opa */


#endif /* OUROBOROS_SCRIPT_PYTHON_MODULES_H_ */
