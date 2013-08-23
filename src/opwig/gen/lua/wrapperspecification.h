
#ifndef OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_
#define OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

#include <opwig/gen/wrapperspecification.h>
#include <opwig/gen/lua/wraps.h>
#include <opwig/gen/lua/wrapperstate.h>
#include <opwig/md/ptr.h>
#include <list>
#include <string>
#include <ostream>

namespace opwig {
namespace gen {
namespace lua {

class WrapperSpecification final : public ::opwig::gen::WrapperSpecification {

  public:

    WrapperSpecification (const std::string& the_module_name);

    std::string wrapper_name () const;

    std::string FileHeader() const;
    std::string MiddleBlock() const;
    std::string FinishFile() const;

    std::string WrapFunction(const md::Ptr<const md::Function>& obj);
    std::string WrapVariable(const md::Ptr<const md::Variable>& obj);
    std::string WrapEnum(const md::Ptr<const md::Enum>& obj);

    std::string OpenClass(const md::Ptr<const md::Class>& obj);
    std::string CloseClass(const md::Ptr<const md::Class>& obj);
    std::string OpenNamespace(const md::Ptr<const md::Namespace>& obj);
    std::string CloseNamespace(const md::Ptr<const md::Namespace>& obj);

    std::string LoadFuncSignature() const;
    std::list<ScriptModule> GetGeneratedModules() const;
    
  private:

    std::list<md::Ptr<ModuleWrap>>  modules_;
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
    if (!state_.current_module()->open) {
        output << "namespace generated {\n\n";
        state_.current_module()->open = true;
    }
}

} // namespace lua
} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

