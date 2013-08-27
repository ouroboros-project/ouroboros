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

namespace python {

class WrapScope final {
  public:
    WrapScope(const std::string& name)
     : name_(name), is_class_(false) {}
    WrapScope(const std::string& name, const md::Ptr<WrapScope>& parent)
     : name_(name), is_class_(false), parent_(parent) {}
    WrapScope(const std::string& name, bool is_class)
     : name_(name), is_class_(is_class) {}
    WrapScope(const std::string& name, bool is_class, const md::Ptr<WrapScope>& parent)
     : name_(name), is_class_(is_class), parent_(parent) {}
    ~WrapScope() {}

    void AddFunction(const md::Ptr<const md::Function>& func);
    void AddVariable(const md::Ptr<const md::Variable>& var);

    std::string GetMethodTableName() const;
    std::string GenerateMethodTable(const std::string& base_nspace) const;

    std::string GetGetSetTableName() const;
    std::string GenerateGetSetTable(const std::string& base_nspace) const;

    void AddSubModule(const md::Ptr<WrapScope>& subm) { sub_modules_.push_back(subm); }
    
    /// Returns the name of this scope (should be equal to the name of the md::Scope that generated this).
    std::string name() const { return name_; }
    /** Returns a dot-separated path name, from the root scope to this one (generating the
        dotted-path notation for python). */
    std::string full_dotted_name() const;
    /** Returns a '::'-separated path name, from the root scope to this one (should be equal to the nested_name()
        of the md::Scope that generated this). */
    std::string nested_name() const;

    void set_name(const std::string& name) { name_ = name; }
    bool is_class() const { return is_class_; }
    const md::Ptr<WrapScope>& parent() const { return parent_; }
    const std::list<md::Ptr<WrapScope>>& sub_modules() const { return sub_modules_; }

    std::list<ScriptModule> ConvertTreeToScriptModuleList() const;

  private:
    std::string name_;
    bool is_class_;
    std::list<md::Ptr<const md::Function>> functions_;
    std::list<md::Ptr<const md::Variable>> variables_;
    md::Ptr<WrapScope> parent_;
    std::list<md::Ptr<WrapScope>> sub_modules_;

    void auxConvertToModuleList(std::list<ScriptModule>& moduleList) const;
};

} // namespace python
} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_PYTHON_WRAPSCOPE_H_

