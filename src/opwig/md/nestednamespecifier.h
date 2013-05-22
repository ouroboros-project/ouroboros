
#ifndef OPWIG_MD_NESTEDNAMESPECIFIER_H_
#define OPWIG_MD_NESTEDNAMESPECIFIER_H_

#include <opwig/md/ptr.h>
#include <string>
#include <vector>

namespace opwig {
namespace md {

class Scope;

/// Base class for metadata objects.
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

    /// Gets the name of the nested-name-specifier.
    const std::string& name() const { return name_; }
    /// Sets the name of the nested-name-specifier.
    void set_name(const std::string& the_name) { name_ = the_name; }

    /// Evaluates the nested-name-specifier, starting from the given initial scope, and returns the scope
    /// in which the 'name' belongs.
    Ptr<Scope> Evaluate(Ptr<Scope> initial_scope) const;
    
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


