
#include <opwig/md/namespace.h>

namespace opwig {
namespace md {

using std::string;

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

bool Namespace::AddGlobalVariable (const Variable::Ptr& variable) {
    auto check = global_variables_.find(variable->name());
    if (check != global_variables_.end())
        return false;
    global_variables_[variable->name()] = variable;
    return true;
}

Variable::ConstPtr Namespace::GlobalVariable (const std::string& name) const {
    auto get = global_variables_.find(name);
    return get != global_variables_.end()
        ? get->second
        : Variable::ConstPtr();
}

Variable::Ptr Namespace::GlobalVariable (const std::string& name) {
    auto get = global_variables_.find(name);
    return get != global_variables_.end()
        ? get->second
        : Variable::Ptr();
}

} // namespace md
} // namespace opwig

