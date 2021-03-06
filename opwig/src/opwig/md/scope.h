
#ifndef OPWIG_MD_SCOPE_H_
#define OPWIG_MD_SCOPE_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/container.h>

#include <string>

namespace opwig {
namespace md {

// Forward declarations.
class Namespace;
class Class;
class Variable;
class Function;
class Enum;

/// Metadata class for C++ scopes.
/** Metadata class representing C++ scope objects.
    Scopes are objects which contain other metadata objects, and possibly some metadata of themselves.
    This class is the base for such scope metadata objects in the system. For the actual scope classes,
    check the derived classes such as opwig::md::Class and opwig::md::Namespace. */
class Scope : public MetadataObject, public std::enable_shared_from_this<Scope> {

  public:

    /// Virtual destructor.
    virtual ~Scope () {}

    /*****************************************************/
    /*** NAMESPACE METHODS ***/
    
    /// Tells how many nested namespaces exist in this scope.
    virtual size_t NestedNamespacesNum () const;

    /// Adds a nested namespace.
    virtual bool AddNestedNamespace (Ptr<Namespace> nested);

    /// Gives the nested namespace identified by the given name (const version).
    virtual Ptr<const Namespace> NestedNamespace (const std::string& nmspace_id) const;
    
    /// Gives the nested namespace identified by the given name.
    virtual Ptr<Namespace> NestedNamespace (const std::string& nmspace_id);
    
    /// Iterate thru this scope's namespaces.
    virtual Container<Namespace>::Iterable IterateNamespaces () const;

    /*****************************************************/
    /*** VARIABLE METHODS ***/
    
    /// Tells how many variables exist in this scope.
    virtual size_t GlobalVariablesNum ()  const;
    
    /// Adds a global variable to this scope.
    virtual bool AddGlobalVariable (Ptr<Variable> variable);

    /// Gives the global variable identified by the given name (const version).
    virtual Ptr<const Variable> GlobalVariable (const std::string& var_id) const;
    
    /// Gives the global variable identified by the given name.
    virtual Ptr<Variable> GlobalVariable (const std::string& var_id);
    
    /// Iterate thru this scope's variables.
    virtual Container<Variable>::Iterable IterateVariables () const;
    
    /*****************************************************/
    /*** CLASS METHODS ***/
    
    /// Tells how many nested classes exist in this scope.
    virtual size_t NestedClassesNum () const;

    /// Adds a nested class.
    virtual bool AddNestedClass (Ptr<Class> nested);

    /// Gives the nested class identified by the given name (const version).
    virtual Ptr<const Class> NestedClass (const std::string& class_id) const;
    
    /// Gives the nested class identified by the given name.
    virtual Ptr<Class> NestedClass (const std::string& class_id);

    /// Iterate thru this scope's classes.
    virtual Container<Class>::Iterable IterateClasses () const;

    /*****************************************************/
    /*** FUNCTION METHODS ***/
    
    /// Tells how many functions exist in this scope.
    virtual size_t NestedFunctionsNum () const;

    /// Adds a function to the scope. In classes, it will be a method.
    virtual bool AddNestedFunction (Ptr<Function> nested);

    /// Gives the function identified by the given name (const version).
    virtual Ptr<const Function> NestedFunction (const std::string& func_id) const;
    
    /// Gives the function identified by the given name.
    virtual Ptr<Function> NestedFunction (const std::string& func_id);
    
    /// Iterate thru this scope's functions.
    virtual Container<Function>::Iterable IterateFunctions () const;
    
    /*****************************************************/
    /*** ENUM METHODS ***/
    
    /// Tells how many functions exist in this scope.
    virtual size_t NestedEnumsNum () const;

    /// Adds a function to the scope. In classes, it will be a method.
    virtual bool AddNestedEnum (Ptr<Enum> nested);

    /// Gives the function identified by the given name (const version).
    virtual Ptr<const Enum> NestedEnum (const std::string& enum_id) const;
    
    /// Gives the function identified by the given name.
    virtual Ptr<Enum> NestedEnum (const std::string& enum_id);
    
    /// Iterate thru this scope's enums.
    virtual Container<Enum>::Iterable IterateEnums () const;
    
    /*****************************************************/
    /*** GENERAL METHODS ***/
       
    /// Checks if the given id exists within this scope.
    virtual bool HasID(const std::string& obj_id) const;

    /// Checks if the given name exists within this scope.
    virtual bool HasName(const std::string& obj_name) const;

    /// Checks if the given id exists within this scope.
    virtual bool HasObject(const Ptr<const MetadataObject>& obj) const;
    
  protected:

    Scope (const std::string& scope_name) : MetadataObject(scope_name) { }
    
    Container<Namespace>    namespaces_;
    Container<Variable>     variables_;
    Container<Class>        classes_;
    Container<Function>     functions_;
    Container<Enum>         enums_;

    bool hasNameNonFunction(const std::string& obj_name) const;

  private:

};

} // namespace md
} // namespace opwig

#endif // OPWIG_MDNAMESPACE_H_


