
#ifndef OPWIG_MD_METADATAOBJECT_H_
#define OPWIG_MD_METADATAOBJECT_H_

#include <opwig/md/accessspecifier.h>
#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace md {

class Scope;

/// Base class for metadata objects.
class MetadataObject {

  public:
    /// Virtual destructor.
    virtual ~MetadataObject () {}
    
    /// Gets the metadata object name.
    const std::string& name() const { return name_; }

    /** Gets the metadata object unique ID. A scope cannot have 2 MOs with the same ID.
        By default, the ID is the same as the name. */
    virtual const std::string& id() const { return name_; }

    /// Gives the access specifier of this metadata object.
    virtual AccessSpecifier access () const { return access_; }
    
    /// Sets the current access specifier for this container.
    void set_access (AccessSpecifier new_access) { access_ = new_access; }
    
    /// Gets the parent scope of this metadata object.
    const Ptr<Scope> parent() const { return parent_; }
    
    /// Sets the parent scope of this metadata object.
    void set_parent(Ptr<Scope> parent_scope) { parent_ = parent_scope; }
    
    /** Gets the string representation of the implicit nested name of this metadata object.
        The returned nested name will start from the global namespace. */
    std::string nested_name(const std::string& separator="::") const;
    
  protected:
  
    MetadataObject(const std::string& obj_name) : name_(obj_name) {};
    
    std::string name_;
    AccessSpecifier access_;
    Ptr<Scope> parent_;
};

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_METADATAOBJECT_H_


