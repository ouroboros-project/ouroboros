
#ifndef OPWIG_MD_CONTAINER_H_
#define OPWIG_MD_CONTAINER_H_

#include <opwig/md/ptr.h>
#include <opwig/md/accessspecifier.h>

#include <string>
#include <map>

namespace opwig {
namespace md {


/// Container class for metadata objects.
template <class T>
class Container {

  public:

    Container() : access_(AccessSpecifier::PRIVATE) {}
    
    /// Virtual destructor.
    virtual ~Container () {}

    /// Tells how many metadata objects there are in this container.
    size_t Num () const;

    /// Adds a new metadata object with the current access specifier.
    bool Add (const std::string& name, Ptr<T> mdObj);

    /// Gives the metadata object identified by the given name (const version).
    Ptr<const T> Get (const std::string& name) const;
    
    /// Gives the metadata object identified by the given name.
    Ptr<T> Get (const std::string& name);

    /// Gets the current access specifier for this container.
    AccessSpecifier GetCurrentAccessSpecifier () const { return access_; }
    
    /// Sets the current access specifier for this container.
    void SetCurrentAccessSpecifier (AccessSpecifier access) { access_ = access; }
    
    /// Checks if the given name exists within this container.
    bool HasName(const std::string& name) const;

  protected:
  
    AccessSpecifier access_;
    std::map<std::string, Ptr<T>> objects_;
    
};

template <class T>
inline size_t Container<T>::Num () const {
    return objects_.size();
}

template <class T>
inline bool Container<T>::Add (const std::string& name, Ptr<T> mdObj) {
    auto check = objects_.find(name);
    if (check != objects_.end())
        return false;
    mdObj->set_access(access_);
    objects_[name] = mdObj;
    return true;
}

template <class T>
inline Ptr<const T> Container<T>::Get (const std::string& name) const {
    auto get = objects_.find(name);
    return get != objects_.end() 
        ? get->second 
        : Ptr<const T>();
}

template <class T>
inline Ptr<T> Container<T>::Get (const std::string& name) {
    auto get = objects_.find(name);
    return get != objects_.end()
        ? get->second
        : Ptr<T>();
}

template <class T>
inline bool Container<T>::HasName(const std::string& name) const {
    return objects_.count(name) == 1;
}


} // namespace md
} // namespace opwig

#endif // OPWIG_MD_CONTAINER_H_


