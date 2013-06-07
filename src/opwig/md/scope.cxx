
#include <opwig/md/scope.h>

#include <opwig/md/namespace.h>
#include <opwig/md/variable.h>
#include <opwig/md/class.h>
#include <opwig/md/function.h>
#include <opwig/md/enum.h>

namespace opwig {
namespace md {

using std::string;

/*****************************************************/
/*** NAMESPACE METHODS ***/

size_t Scope::NestedNamespacesNum () const {
    return namespaces_.Num();
}

bool Scope::AddNestedNamespace (Ptr<Namespace> nested) {
    string id = nested->id();
    if (HasObject(nested)) 
        throw SemanticError("Can't add namespace '"+id+"' to scope, it already exists.", __FILE__, __LINE__);
    bool ok = namespaces_.Add(nested);
    if (ok) nested->set_parent( shared_from_this() );
    return ok;
}

Ptr<const Namespace> Scope::NestedNamespace (const string& nmspace_id) const {
    return namespaces_.Get(nmspace_id);
}

Ptr<Namespace> Scope::NestedNamespace (const string& nmspace_id) {
    return namespaces_.Get(nmspace_id);
}

/*****************************************************/
/*** VARIABLE METHODS ***/

size_t Scope::GlobalVariablesNum ()  const {
    return variables_.Num();
}

bool Scope::AddGlobalVariable (Ptr<Variable> variable) {
    if (HasObject(variable)) 
        throw SemanticError("Can't add variable '"+variable->id()+"' to scope, it already exists.", __FILE__, __LINE__);
    bool ok = variables_.Add(variable);
    if (ok) variable->set_parent( shared_from_this() );
    return ok;
}

Ptr<const Variable> Scope::GlobalVariable (const string& var_id) const {
    return variables_.Get(var_id);
}

Ptr<Variable> Scope::GlobalVariable (const string& var_id) {
    return variables_.Get(var_id);
}

/*****************************************************/
/*** CLASS METHODS ***/

size_t Scope::NestedClassesNum () const {
    return classes_.Num();
}

bool Scope::AddNestedClass (Ptr<Class> nested) {
    if (HasObject(nested)) 
        throw SemanticError("Can't add class '"+nested->id()+"' to scope, it already exists.", __FILE__, __LINE__);
    bool ok = classes_.Add(nested);
    if (ok) nested->set_parent( shared_from_this() );
    return ok;
}

Ptr<const Class> Scope::NestedClass (const string& class_id) const {
    return classes_.Get(class_id);
}

Ptr<Class> Scope::NestedClass (const string& class_id) {
    return classes_.Get(class_id);
}

/*****************************************************/
/*** FUNCTION METHODS ***/

size_t Scope::NestedFunctionsNum () const {
    return functions_.Num();
}

bool Scope::AddNestedFunction (Ptr<Function> nested) {
    string id = nested->id();
    if (HasID(id) || hasNameNonFunction(nested->name()) ) 
        throw SemanticError("Can't add function '"+id+"' to scope, it already exists.", __FILE__, __LINE__);
    bool ok = functions_.Add(nested);
    if (ok) nested->set_parent( shared_from_this() );
    return ok;
}

Ptr<const Function> Scope::NestedFunction (const string& func_id) const {
    return functions_.Get(func_id);
}

Ptr<Function> Scope::NestedFunction (const string& func_id) {
    return functions_.Get(func_id);
}

/*****************************************************/
/*** ENUM METHODS ***/

size_t Scope::NestedEnumsNum () const {
    return enums_.Num();
}

bool Scope::AddNestedEnum (Ptr<Enum> nested) {
    if (HasObject(nested)) 
        throw SemanticError("Can't add enum '"+nested->id()+"' to scope, it already exists.", __FILE__, __LINE__);
    bool ok = enums_.Add(nested);
    if (ok) nested->set_parent( shared_from_this() );
    return ok;
}

Ptr<const Enum> Scope::NestedEnum (const string& enum_id) const {
    return enums_.Get(enum_id);
}

Ptr<Enum> Scope::NestedEnum (const string& enum_id) {
    return enums_.Get(enum_id);
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
    enums_.SetCurrentAccessSpecifier(new_access);
}

bool Scope::HasID(const string& obj_id) const {
    return namespaces_.HasID(obj_id) || 
           variables_.HasID(obj_id) ||
           classes_.HasID(obj_id) ||
           functions_.HasID(obj_id) ||
           enums_.HasID(obj_id);
}

bool Scope::HasName(const std::string& obj_name) const {
    return functions_.HasName(obj_name) || hasNameNonFunction(obj_name);
}

bool Scope::HasObject(const Ptr<const MetadataObject>& obj) const {
    return HasName(obj->name()) || HasID(obj->id());
}

bool Scope::hasNameNonFunction(const std::string& obj_name) const {
    return namespaces_.HasName(obj_name) || 
           variables_.HasName(obj_name) ||
           classes_.HasName(obj_name) ||
           enums_.HasName(obj_name);
}

} // namespace md
} // namespace opwig

