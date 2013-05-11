
#include <opwig/md/scope.h>

#include <opwig/md/namespace.h>
#include <opwig/md/variable.h>
#include <opwig/md/class.h>
#include <opwig/md/function.h>

namespace opwig {
namespace md {

using std::string;

/*****************************************************/
/*** NAMESPACE METHODS ***/

size_t Scope::NestedNamespacesNum () const {
    return namespaces_.Num();
}

bool Scope::AddNestedNamespace (const string& nmspace_name, Ptr<Namespace> nested) {
    if (HasName(nmspace_name)) 
        throw SemanticError("Can't add namespace '"+nmspace_name+"' to scope, it already exists.", __FILE__, __LINE__);
    return namespaces_.Add(nmspace_name, nested);
}

Ptr<const Namespace> Scope::NestedNamespace (const string& nmspace_name) const {
    return namespaces_.Get(nmspace_name);
}

Ptr<Namespace> Scope::NestedNamespace (const string& nmspace_name) {
    return namespaces_.Get(nmspace_name);
}

/*****************************************************/
/*** VARIABLE METHODS ***/

size_t Scope::GlobalVariablesNum ()  const {
    return variables_.Num();
}

bool Scope::AddGlobalVariable (Ptr<Variable> variable) {
    if (HasName(variable->name())) 
        throw SemanticError("Can't add variable '"+variable->name()+"' to scope, it already exists.", __FILE__, __LINE__);
    return variables_.Add(variable->name(), variable);
}

Ptr<const Variable> Scope::GlobalVariable (const string& var_name) const {
    return variables_.Get(var_name);
}

Ptr<Variable> Scope::GlobalVariable (const string& var_name) {
    return variables_.Get(var_name);
}

/*****************************************************/
/*** CLASS METHODS ***/

size_t Scope::NestedClassesNum () const {
    return classes_.Num();
}

bool Scope::AddNestedClass (const string& class_name, Ptr<Class> nested) {
    if (HasName(class_name)) 
        throw SemanticError("Can't add class '"+class_name+"' to scope, it already exists.", __FILE__, __LINE__);
    return classes_.Add(class_name, nested);
}

Ptr<const Class> Scope::NestedClass (const string& class_name) const {
    return classes_.Get(class_name);
}

Ptr<Class> Scope::NestedClass (const string& class_name) {
    return classes_.Get(class_name);
}

/*****************************************************/
/*** FUNCTION METHODS ***/

size_t Scope::NestedFunctionsNum () const {
    return functions_.Num();
}

bool Scope::AddNestedFunction (Ptr<Function> nested) {
    if (HasName(nested->name())) 
        throw SemanticError("Can't add function '"+nested->name()+"' to scope, it already exists.", __FILE__, __LINE__);
    return functions_.Add(nested->name(), nested);
}

Ptr<const Function> Scope::NestedFunction (const string& func_name) const {
    return functions_.Get(func_name);
}

Ptr<Function> Scope::NestedFunction (const string& func_name) {
    return functions_.Get(func_name);
}

/*****************************************************/
/*** GENERAL METHODS ***/

AccessSpecifier Scope::GetAccessSpecifier () const {
    // Since we set all containers' access specifier at the same time, here we only need to grab one of them
    return namespaces_.GetCurrentAccessSpecifier();
}

void Scope::SetAccessSpecifier(AccessSpecifier new_access) {
    namespaces_.SetCurrentAccessSpecifier(new_access);
    variables_.SetCurrentAccessSpecifier(new_access);
    classes_.SetCurrentAccessSpecifier(new_access);
    functions_.SetCurrentAccessSpecifier(new_access);
}

bool Scope::HasName(const string& obj_name) const {
    return namespaces_.HasName(obj_name) || 
           variables_.HasName(obj_name) ||
           classes_.HasName(obj_name) ||
           functions_.HasName(obj_name);
}


} // namespace md
} // namespace opwig

