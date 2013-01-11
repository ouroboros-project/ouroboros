#ifndef OUROBOROS_SCRIPT_H_
#define OUROBOROS_SCRIPT_H_

namespace ouroboros {

// Functions
void InitScripts();

// Public classes
class LangWrapper;
class VirtualData;
class VirtualObj;

template <class loader_t> class InheritableLangWrapper;

template <class loader_t>
class Module;

// Managers
class ScriptManager;

namespace lua {

// Public Lua classes
class LuaData;

// Lua Managers
class LuaWrapper;

} /* namespace lua */

namespace python {

// Public Python classes
class PythonData;

// Python Managers
class PythonWrapper;

} /* namespace python */

}  // namespace ouroboros

#endif /* OUROBOROS_SCRIPT_H_ */
