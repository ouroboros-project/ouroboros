
#include <opwig/md/namespace.h>

namespace opwig {
namespace md {

using std::string;

bool Namespace::AddNestedNamespace (const string& name, const Ptr& nested) {
  auto check = nested_namespaces_.find(name);
  if (check != nested_namespaces_.end())
    return false;
  nested_namespaces_[name] = nested;
  return true;
}

Namespace::ConstPtr Namespace::NestedNamespace (const string& name) const {
  auto get = nested_namespaces_.find(name);
  return get != nested_namespaces_.end()
    ? get->second
    : ConstPtr();
}

Namespace::Ptr Namespace::NestedNamespace (const string& name) {
  auto get = nested_namespaces_.find(name);
  return get != nested_namespaces_.end()
    ? get->second
    : Ptr();
}

} // namespace md
} // namespace opwig

