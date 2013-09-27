#ifndef OUROBOROS_SCRIPT_H_
#define OUROBOROS_SCRIPT_H_

namespace opa {

// Functions
void InitScripts();

// Public classes
class VirtualMachine;
class VirtualData;
class VirtualObj;

template <typename T> class Converter;
template <typename loader_t> class InheritableVirtualMachine;

template <typename loader_t>
class Module;

// Managers
class ScriptManager;

namespace lua {

// Public Lua classes
class LuaData;

// Lua Managers
class LuaMachine;

} /* namespace lua */

namespace python {

// Public Python classes
class PythonData;

// Python Managers
class PythonMachine;

} /* namespace python */

}  // namespace opa

#endif /* OUROBOROS_SCRIPT_H_ */
