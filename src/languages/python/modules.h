#ifndef OUROBOROS_SCRIPT_PYTHON_MODULES_H_
#define OUROBOROS_SCRIPT_PYTHON_MODULES_H_

#include <ouroboros/script.h>
#include <languages/python/pythonwrapper.h>

namespace ouroboros {
namespace python {

void AddModule(const Module<inittype> &);

void RegisterModules(PythonWrapper* wrapper);

} /* namespace python */
} /* namespace ouroboros */


#endif /* OUROBOROS_SCRIPT_PYTHON_MODULES_H_ */
