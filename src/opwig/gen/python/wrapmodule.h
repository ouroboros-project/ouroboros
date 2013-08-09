#ifndef OPWIG_GEN_PYTHON_WRAPMODULE_H_
#define OPWIG_GEN_PYTHON_WRAPMODULE_H_

#include <opwig/md/ptr.h>
#include <list>
#include <string>

namespace opwig {

namespace md {
class Function;
}

namespace gen {

struct ScriptModule;

namespace python {

class WrapModule final {
  public:
    WrapModule(const std::string& name) : name_(name) {}
    WrapModule(const std::string& name, const md::Ptr<WrapModule>& parent) : name_(name), parent_(parent) {}
    ~WrapModule() {}

    void AddFunction(const md::Ptr<const md::Function>& func);

    std::string GetMethodTableName() const;
    std::string GenerateMethodTable(const std::string& base_nspace) const;

    void AddSubModule(const md::Ptr<WrapModule>& subm) { sub_modules_.push_back(subm); }
    
    std::string name() { return name_; }
    void set_name(const std::string& name) { name_ = name; }
    const md::Ptr<WrapModule>& parent() const { return parent_; }
    const std::list<md::Ptr<WrapModule>>& sub_modules() const { return sub_modules_; }

    std::list<ScriptModule> ConvertTreeToScriptModuleList() const;

  private:
    std::string name_;
    std::list<md::Ptr<const md::Function>> functions_;
    md::Ptr<WrapModule> parent_;
    std::list<md::Ptr<WrapModule>> sub_modules_;

    void auxConvertToModuleList(std::list<ScriptModule>& moduleList) const;
};

} // namespace python
} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_PYTHON_WRAPMODULE_H_

