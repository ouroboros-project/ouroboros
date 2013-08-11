
#ifndef OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_
#define OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

#include <opwig/gen/wrapperspecification.h>
#include <opwig/md/ptr.h>
#include <list>
#include <string>

namespace opwig {
namespace gen {
namespace lua {

class WrapperSpecification final : public ::opwig::gen::WrapperSpecification {

  public:

    WrapperSpecification ();

    std::string wrapper_name () const;

    std::string FileHeader() const;
    std::string MiddleBlock() const;
    std::string FinishFile() const;

    std::string WrapFunction(const md::Ptr<const md::Function>& obj);
    std::string WrapVariable(const md::Ptr<const md::Variable>& obj);
    std::string WrapClass(const md::Ptr<const md::Class>& obj);
    std::string WrapNamespace(const md::Ptr<const md::Namespace>& obj, bool closing);
    std::string WrapEnum(const md::Ptr<const md::Enum>& obj);

    std::string LoadFuncSignature() const;
    std::list<ScriptModule> GetGeneratedModules() const;
    
  private:

    struct DataWrap {
        std::string name;
        std::string nesting;
    };

    struct Module {
        std::string                 name, path;
        std::list<DataWrap>         functions;
        std::list<DataWrap>         getters;
        std::list<md::Ptr<Module>>  children;
        md::WeakPtr<Module>         parent;
        bool                        open;
    };

    std::list<md::Ptr<Module>>  modules_;
    std::list<md::Ptr<Module>>  module_stack_;

    std::string DumpNamespaceNesting () const;

    md::Ptr<Module> current_module () const;

};

inline std::string WrapperSpecification::wrapper_name () const {
    return "Lua";
}

inline std::string WrapperSpecification::LoadFuncSignature () const {
    return "int(*)(lua_State*)";
}

inline md::Ptr<WrapperSpecification::Module> WrapperSpecification::current_module () const {
    return module_stack_.back();
}

} // namespace lua
} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

