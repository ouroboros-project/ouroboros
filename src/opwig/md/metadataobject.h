
#ifndef OPWIG_MD_METADATAOBJECT_H_
#define OPWIG_MD_METADATAOBJECT_H_

#include <opwig/md/accessspecifier.h>

#include <string>

namespace opwig {
namespace md {

/// Base class for metadata objects.
class MetadataObject {

  public:
    /// Virtual destructor.
    virtual ~MetadataObject () {}
    
    /// Gets the metadata object name.
    const std::string& name() const { return name_; }

    /// Gives the access specifier of this metadata object.
    virtual AccessSpecifier access () const { return access_; }
    
    /// Sets the current access specifier for this container.
    void set_access (AccessSpecifier access) { access_ = access; }
    
  protected:
  
    MetadataObject(const std::string& name) : name_(name) {};
    
    std::string name_;
    AccessSpecifier access_;
};




} // namespace md
} // namespace opwig

#endif // OPWIG_MD_METADATAOBJECT_H_


