
#include <opwig/md/scope.h>

namespace opwig {
namespace md {

using std::string;

/*****************************************************/
/*** NAMESPACE METHODS ***/

size_t Scope::NestedNamespacesNum () const {
    return namespaces_.Num();
}

bool Scope::AddNestedNamespace (const string& name, Ptr<Namespace> nested) {
    return namespaces_.Add(name, nested);
}

Ptr<const Namespace> Scope::NestedNamespace (const string& name) const {
    return namespaces_.Get(name);
}

Ptr<Namespace> Scope::NestedNamespace (const string& name) {
    return namespaces_.Get(name);
}

/*****************************************************/
/*** VARIABLE METHODS ***/

size_t Scope::GlobalVariablesNum ()  const {
    return variables_.Num();
}

bool Scope::AddGlobalVariable (Ptr<Variable> variable) {
    return variables_.Add(variable->name(), variable);
}

Ptr<const Variable> Scope::GlobalVariable (const string& name) const {
    return variables_.Get(name);
}

Ptr<Variable> Scope::GlobalVariable (const string& name) {
    return variables_.Get(name);
}

/*****************************************************/
/*** CLASS METHODS ***/

size_t Scope::NestedClassesNum () const {
    return classes_.Num();
}

bool Scope::AddNestedClass (const string& name, Ptr<Class> nested) {
    return classes_.Add(name, nested);
}

Ptr<const Class> Scope::NestedClass (const string& name) const {
    return classes_.Get(name);
}

Ptr<Class> Scope::NestedClass (const string& name) {
    return classes_.Get(name);
}

/*****************************************************/
/*** FUNCTION METHODS ***/

size_t Scope::NestedFunctionsNum () const {
    return functions_.Num();
}

bool Scope::AddNestedFunction (Ptr<Function> nested) {
    return functions_.Add(nested->name(), nested);
}

Ptr<const Function> Scope::NestedFunction (const string& name) const {
    return functions_.Get(name);
}

Ptr<Function> Scope::NestedFunction (const string& name) {
    return functions_.Get(name);
}

/*****************************************************/
/*** GENERAL METHODS ***/

AccessSpecifier Scope::GetAccessSpecifier (const string& name) const {
    // Since we set all containers' access specifier at the same time, here we only need to grab one of them
    return namespaces_.GetCurrentAccessSpecifier();
}

void Scope::SetAccessSpecifier(AccessSpecifier access) {
    namespaces_.SetCurrentAccessSpecifier(access);
    variables_.SetCurrentAccessSpecifier(access);
    classes_.SetCurrentAccessSpecifier(access);
    functions_.SetCurrentAccessSpecifier(access);
}

bool Scope::HasName(const string& name) const {
    return namespaces_.HasName(name) || 
           variables_.HasName(name) ||
           classes_.HasName(name) ||
           functions_.HasName(name);
}

} // namespace md
} // namespace opwig

