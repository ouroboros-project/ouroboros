
#ifndef OPWIG_MD_NESTEDNAMESPECIFIER_H_
#define OPWIG_MD_NESTEDNAMESPECIFIER_H_

#include <string>
#include <vector>

namespace opwig {
namespace md {

/// Base class for metadata objects.
class NestedNameSpecifier {
  public:
    NestedNameSpecifier() : global_(false) {}
    NestedNameSpecifier(const std::string& the_name) : global_(false), name_(the_name) {}
    NestedNameSpecifier(const std::string& the_name, bool global) :
        global_(global), name_(the_name) {}
  
    /// Virtual destructor.
    virtual ~NestedNameSpecifier () {}
    
    bool is_global() const { return global_; }
    void set_global(bool global) { global_ = global; }

    void AddPath(const std::string& path); { paths_.push_back(path); }

    const std::string& name() const { return name_; }
    void set_name(const std::string& the_name) { name_ = the_name; }

    Ptr<Scope> Evaluate(Ptr<Scope> initial_scope);
    
  protected:
    bool global_;
    std::vector<std::string> paths_;
    std::string name_;
};




} // namespace md
} // namespace opwig

#endif // OPWIG_MD_NESTEDNAMESPECIFIER_H_


