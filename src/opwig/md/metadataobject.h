
#ifndef OPWIG_MD_METADATAOBJECT_H_
#define OPWIG_MD_METADATAOBJECT_H_

#include <opwig/md/accessspecifier.h>

namespace opwig {
namespace md {

/// Base class for metadata objects.
class MetadataObject {

  public:
    /// Virtual destructor.
    virtual ~MetadataObject () {}

    /// Gives the access specifier of this metadata object.
    virtual AccessSpecifier access () const { return access_; }
    
    /// Sets the current access specifier for this container.
    void set_access (AccessSpecifier access) { access_ = access; }
    
  protected:
  
    MetadataObject() {};
    
    AccessSpecifier access_;
};




} // namespace md
} // namespace opwig

#endif // OPWIG_MD_METADATAOBJECT_H_


