
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

} // namespace md
} // namespace opwig

