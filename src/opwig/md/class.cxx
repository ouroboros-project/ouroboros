
#include <opwig/md/class.h>
#include <opwig/md/function.h>

namespace opwig {
namespace md {

using std::string;

/*** NAMESPACE METHODS ***/

bool Class::AddNestedNamespace (const string& name, Ptr<Namespace> nested) {
    throw SemanticError("Namespaces cannot be added in a class!", __FILE__, __LINE__);
}

Ptr<const Namespace> Class::NestedNamespace (const string& name) const {
    throw SemanticError("Classes do not have nested namespaces!", __FILE__, __LINE__);
}

Ptr<Namespace> Class::NestedNamespace (const string& name) {
    throw SemanticError("Classes do not have nested namespaces!", __FILE__, __LINE__);
}

/*** VARIABLE METHODS ***/

bool Class::AddGlobalVariable (Ptr<Variable> variable) {
    auto check = global_variables_.find(variable->name());
    if (check != global_variables_.end())
        return false;
    global_variables_[variable->name()] = variable;
    return true;
}

Ptr<const Variable> Class::GlobalVariable (const std::string& name) const {
    auto get = global_variables_.find(name);
    return get != global_variables_.end()
        ? get->second
        : Ptr<const Variable>();
}

Ptr<Variable> Class::GlobalVariable (const std::string& name) {
    auto get = global_variables_.find(name);
    return get != global_variables_.end()
        ? get->second
        : Ptr<Variable>();
}

/*** CLASS METHODS ***/

bool Class::AddNestedClass (const std::string& name, Ptr<Class> nested) {
    auto check = nested_classes_.find(name);
    if (check != nested_classes_.end())
        return false;
    nested_classes_[name] = nested;
    return true;
}

Ptr<const Class> Class::NestedClass (const std::string& name) const {
    auto get = nested_classes_.find(name);
    return get != nested_classes_.end()
        ? get->second
        : Ptr<const Class>();
}

Ptr<Class> Class::NestedClass (const std::string& name) {
    auto get = nested_classes_.find(name);
    return get != nested_classes_.end()
        ? get->second
        : Ptr<Class>();
}

/*** FUNCTION METHODS ***/

bool Class::AddNestedFunction (Ptr<Function> nested) {
    auto check = nested_functions_.find(nested->name());
    if (check != nested_functions_.end())
        return false;
    nested_functions_[nested->name()] = nested;
    return true;
}

Ptr<const Function> Class::NestedFunction (const string& name) const {
    auto get = nested_functions_.find(name);
    return get != nested_functions_.end()
        ? get->second
        : Ptr<const Function>();
}

} // namespace md
} // namespace opwig

