
#include <opwig/md/namespace.h>

namespace opwig {
namespace md {

using std::string;

/*** NAMESPACE METHODS ***/

bool Namespace::AddNestedNamespace (const string& name, Ptr<Namespace> nested) {
    auto check = nested_namespaces_.find(name);
    if (check != nested_namespaces_.end())
        return false;
    nested_namespaces_[name] = nested;
    return true;
}

Ptr<const Namespace> Namespace::NestedNamespace (const string& name) const {
    auto get = nested_namespaces_.find(name);
    return get != nested_namespaces_.end()
        ? get->second
        : Ptr<const Namespace>();
}

Ptr<Namespace> Namespace::NestedNamespace (const string& name) {
    auto get = nested_namespaces_.find(name);
    return get != nested_namespaces_.end()
        ? get->second
        : Ptr<Namespace>();
}

/*** VARIABLE METHODS ***/

bool Namespace::AddGlobalVariable (Ptr<Variable> variable) {
    auto check = global_variables_.find(variable->name());
    if (check != global_variables_.end())
        return false;
    global_variables_[variable->name()] = variable;
    return true;
}

Ptr<const Variable> Namespace::GlobalVariable (const std::string& name) const {
    auto get = global_variables_.find(name);
    return get != global_variables_.end()
        ? get->second
        : Ptr<const Variable>();
}

Ptr<Variable> Namespace::GlobalVariable (const std::string& name) {
    auto get = global_variables_.find(name);
    return get != global_variables_.end()
        ? get->second
        : Ptr<Variable>();
}

/*** CLASS METHODS ***/

bool Namespace::AddNestedClass (const std::string& name, Ptr<Class> nested) {
    auto check = nested_classes_.find(name);
    if (check != nested_classes_.end())
        return false;
    nested_classes_[name] = nested;
    return true;
}

Ptr<const Class> Namespace::NestedClass (const std::string& name) const {
    auto get = nested_classes_.find(name);
    return get != nested_classes_.end()
        ? get->second
        : Ptr<const Class>();
}

Ptr<Class> Namespace::NestedClass (const std::string& name) {
    auto get = nested_classes_.find(name);
    return get != nested_classes_.end()
        ? get->second
        : Ptr<Class>();
}

} // namespace md
} // namespace opwig

