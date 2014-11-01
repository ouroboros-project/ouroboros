
#ifndef OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_
#define OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

#include <opwig/gen/wrapperspecification.h>
#include <lua/opwig/wraps.h>
#include <lua/opwig/wrapperstate.h>
#include <opwig/md/ptr.h>
#include <list>
#include <string>
#include <ostream>

namespace opa {
namespace lua {

class WrapperSpecification final : public ::opwig::gen::WrapperSpecification {

  public:

    WrapperSpecification (const std::string& the_module_name);

    std::string wrapper_name () const;

    std::string FileHeader() const;
    std::string MiddleBlock() const;
    std::string FinishFile() const;

    std::string WrapFunction(const opwig::md::Ptr<const opwig::md::Function>& obj);
    std::string WrapVariable(const opwig::md::Ptr<const opwig::md::Variable>& obj);
    std::string WrapEnum(const opwig::md::Ptr<const opwig::md::Enum>& obj);

    std::string OpenClass(const opwig::md::Ptr<const opwig::md::Class>& obj);
    std::string CloseClass(const opwig::md::Ptr<const opwig::md::Class>& obj);
    std::string OpenNamespace(const opwig::md::Ptr<const opwig::md::Namespace>& obj);
    std::string CloseNamespace(const opwig::md::Ptr<const opwig::md::Namespace>& obj);

    std::string LoadFuncSignature() const;
    std::list<opwig::gen::ScriptModule> GetGeneratedModules() const;
    
  private:

    std::list<opwig::md::Ptr<ModuleWrap>>  modules_;
    WrapperState                    state_;

    std::string DumpNamespaceNesting () const;

    void CheckAndOpenNamespace (std::ostream& output);

};

inline std::string WrapperSpecification::wrapper_name () const {
    return "Lua";
}

inline std::string WrapperSpecification::LoadFuncSignature () const {
    return "int(*)(lua_State*)";
}

inline void WrapperSpecification::CheckAndOpenNamespace (std::ostream& output) {
    if (!state_.current_module()->has_wraps())
        output << "namespace generated {\n\n";
}

} // namespace lua
} // namespace opa

#endif // OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

