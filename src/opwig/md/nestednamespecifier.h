
#ifndef OPWIG_MD_NESTEDNAMESPECIFIER_H_
#define OPWIG_MD_NESTEDNAMESPECIFIER_H_

#include <opwig/md/ptr.h>
#include <string>
#include <vector>

namespace opwig {
namespace md {

class Scope;

/// Represents a nested-name specifier.
/** Represents a nested-name specifier, which is a C++ grammar structure
    to define the path/name to something (a metadata object). It follows the
    pattern:

    <i>scopeName1::scopeName2::ObjectName</i>

    Where <i>ObjectName</i> is something (a metadata object, such as a function, class, etc), contained
    in the scope <i>scopeName2</i>, which is contained in the scope <i>scopeName1</i>, which should be accessible 
    from the current scope where this nested-name was declared.
    It's possible to add a '::' to the start of the nested-name, in which case the path will start from the global
    namespace.
*/
class NestedNameSpecifier {
  public:
    NestedNameSpecifier() : global_(false) {}
    NestedNameSpecifier(const std::string& the_name) : global_(false), name_(the_name) {}
    NestedNameSpecifier(const std::string& the_name, bool global) :
        global_(global), name_(the_name) {}
  
    /// Virtual destructor.
    virtual ~NestedNameSpecifier () {}
    
    /// Returns if this NestedNameSpecifier should start from the global namespace.
    bool is_global() const { return global_; }
    /// Sets if this NestedNameSpecifier should start from the global namespace (true).
    void set_global(bool global) { global_ = global; }

    /// Adds a scope name to the nested-name-specifier path.
    void AddPath(const std::string& path);

    /// Gets the target name of the nested-name-specifier.
    const std::string& name() const { return name_; }
    /// Sets the target name of the nested-name-specifier.
    void set_name(const std::string& the_name) { name_ = the_name; }

    /// Evaluates the nested-name-specifier, starting from the given initial scope, and returns the
    /// scope in which the 'name' belongs.
    Ptr<Scope> FindNearestNestingScope(Ptr<Scope> initial_scope) const;
    
    /// Returns the textual representation of this nested-name-specifier.
    std::string ToString() const;
    
  protected:
    bool global_;
    std::vector<std::string> paths_;
    std::string name_;
};



} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NESTEDNAMESPECIFIER_H_


