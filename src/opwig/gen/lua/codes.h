
#ifndef OPWIG_GEN_LUA_CODES_H_
#define OPWIG_GEN_LUA_CODES_H_

#include <opwig/gen/lua/wraps.h>

#include <string>

namespace opwig {
namespace gen {
namespace lua {

using WrappedMember = std::list<DataWrap> (Module::*);

inline std::string GetWrapName (const std::string& type, const std::string& name) {
    return "OPWIG_wrap_"+type+"_"+name;
}

std::string WrapList (const md::Ptr<Module>& module, WrappedMember member, const std::string& type);

std::string Utilities ();

} // namespace lua
} // namespace gen
} // namespace opwig

#endif

