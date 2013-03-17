
#include <opwig/mdnamespace.h>

namespace opwig {

using std::string;

bool MDNamespace::AddNestedNamespace (const string& name, const Ptr& nested) {
  auto check = nested_namespaces_.find(name);
  if (check != nested_namespaces_.end())
    return false;
  nested_namespaces_[name] = nested;
  return true;
}

MDNamespace::ConstPtr MDNamespace::NestedNamespace (const string& name) const {
  auto get = nested_namespaces_.find(name);
  return get != nested_namespaces_.end()
    ? get->second
    : ConstPtr();
}

MDNamespace::Ptr MDNamespace::NestedNamespace (const string& name) {
  auto get = nested_namespaces_.find(name);
  return get != nested_namespaces_.end()
    ? get->second
    : Ptr();
}

} // namespace opwig

