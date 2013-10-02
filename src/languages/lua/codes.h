
#ifndef OPWIG_GEN_LUA_CODES_H_
#define OPWIG_GEN_LUA_CODES_H_

#include <languages/lua/wraps.h>

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>

#include <string>

namespace opwig {

namespace md {
class Class;
}
}

namespace opa {
namespace lua {

using WrappedMember = std::list<DataWrap> (ModuleWrap::*);

inline std::string GetWrapName (const std::string& type, const std::string& name) {
    return "wrap_"+type+"_"+name;
}

std::string WrapList (const opwig::md::Ptr<ModuleWrap>& module, WrappedMember member,
                      const std::string& type);

std::string MiddleBlockCode (const std::string& module_name);

std::string CheckAndCloseNamespace (bool open, const std::string& name = "generated");

std::string CloseModuleBlock (const opwig::md::Ptr<ModuleWrap>& module);

} // namespace lua
} // namespace opa

#endif

