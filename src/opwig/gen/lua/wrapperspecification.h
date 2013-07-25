
#ifndef OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_
#define OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

#include <opwig/gen/wrapperspecification.h>
#include <list>
#include <string>

namespace opwig {
namespace gen {
namespace lua {

class WrapperSpecification final : public ::opwig::gen::WrapperSpecification {

  public:

    std::string wrapper_name () const;

    md::Ptr<ConverterProvider> GetConverterProvider() const;

    std::string FileHeader() const;

    std::string FinishFile() const;

    std::string WrapFunction(const md::Ptr<const md::Function>& obj);
    std::string WrapVariable(const md::Ptr<const md::Variable>& obj);
    std::string WrapClass(const md::Ptr<const md::Class>& obj);
    std::string WrapNamespace(const md::Ptr<const md::Namespace>& obj);
    std::string WrapEnum(const md::Ptr<const md::Enum>& obj);

  private:

    std::list<std::string> wrapped_functions_;

};

inline std::string WrapperSpecification::wrapper_name () const {
    return "Lua";
}

} // namespace lua
} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_LUA_WRAPPERSPECIFICATION_H_

