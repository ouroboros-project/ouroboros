#ifndef OPWIG_GEN_PYTHON_WRAPSCOPE_H_
#define OPWIG_GEN_PYTHON_WRAPSCOPE_H_

#include <opwig/md/ptr.h>
#include <list>
#include <string>

namespace opwig {

namespace md {
class Function;
class Variable;
}
namespace gen {
struct ScriptModule;
}
}

namespace opa {
namespace python {

class WrapScope final {
  public:
    WrapScope(const std::string& name)
     : name_(name), is_class_(false) {}
    WrapScope(const std::string& name, const opwig::md::Ptr<WrapScope>& parent)
     : name_(name), is_class_(false), parent_(parent) {}
    WrapScope(const std::string& name, bool is_class);
    WrapScope(const std::string& name, bool is_class, const opwig::md::Ptr<WrapScope>& parent);
    ~WrapScope() {}

    void AddFunction(const opwig::md::Ptr<const opwig::md::Function>& func);
    void AddVariable(const opwig::md::Ptr<const opwig::md::Variable>& var);

    std::string GetMethodTableName() const;
    std::string GenerateMethodTable(const std::string& base_nspace) const;

    std::string GetGetSetTableName() const;
    std::string GenerateGetSetTable(const std::string& base_nspace) const;

    void AddSubModule(const opwig::md::Ptr<WrapScope>& subm) { sub_modules_.push_back(subm); }
    
    /** Returns the name of this scope. It should be equal to the name of the md::Namespace that generated this scope, it will
        be diferent for md::Classes */
    std::string name() const { return name_; }
    /// Returns the name of the class that generated this scope (should be equal to the name of the md::Class that generated this).
    std::string class_name() const;
    /** Returns a dot-separated path name, from the root scope to this one (generating the
        dotted-path notation for python). */
    std::string full_dotted_name() const;
    /** Returns a '::'-separated path name, from the root scope to this one (should be equal to the nested_name()
        of the md::Scope that generated this). */
    std::string nested_name(bool use_class_name=false) const;

    bool is_class() const { return is_class_; }
    const opwig::md::Ptr<WrapScope>& parent() const { return parent_; }
    const std::list<opwig::md::Ptr<WrapScope>>& sub_modules() const { return sub_modules_; }

    std::list<opwig::gen::ScriptModule> ConvertTreeToScriptModuleList() const;

  private:
    std::string name_;
    bool is_class_;
    std::list<opwig::md::Ptr<const opwig::md::Function>> functions_;
    std::list<opwig::md::Ptr<const opwig::md::Variable>> variables_;
    opwig::md::Ptr<WrapScope> parent_;
    std::list<opwig::md::Ptr<WrapScope>> sub_modules_;

    void auxConvertToModuleList(std::list<opwig::gen::ScriptModule>& moduleList) const;
};

} // namespace python
} // namespace opa

#endif // OPWIG_GEN_PYTHON_WRAPSCOPE_H_

