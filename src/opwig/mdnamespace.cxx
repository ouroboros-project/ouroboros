
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

} // namespace opwig

