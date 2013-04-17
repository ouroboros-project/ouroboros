
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

    /// Gives the access specifier of the metadata object identified by the given name.
    AccessSpecifier GetAccessSpecifier (const std::string& name) const;
    
    /// Gets the current access specifier for this container.
    AccessSpecifier GetCurrentAccessSpecifier () const { return access_; }
    
    /// Sets the current access specifier for this container.
    void SetCurrentAccessSpecifier (AccessSpecifier access) { access_ = access; }
    
    /// Checks if the given name exists within this container.
    bool HasName(const std::string& name) const;

  protected:
  
    AccessSpecifier access_;
    struct MDObjAccessPair final {
        Ptr<T> object_;
        AccessSpecifier access_;
        MDObjAccessPair(Ptr<T> object, AccessSpecifier access) : object_(object), access_(access) {}
    };
    std::map<std::string, MDObjAccessPair> objects_;
    
};

template <class T>
inline size_t Container::Num () const {
    return objects_.size();
}

template <class T>
inline bool Container::Add (const string& name, Ptr<T> mdObj) {
    auto check = objects_.find(name);
    if (check != objects_.end())
        return false;
    objects_[name] = MDObjAccessPair(mdObj, access_);
    return true;
}

template <class T>
inline Ptr<const T> Container::Get (const string& name) const {
    auto get = objects_.find(name);
    return get != objects_.end()
        ? get->second.object_;
        : Ptr<const T>();
}

template <class T>
inline Ptr<T> Container::Get (const string& name) {
    auto get = objects_.find(name);
    return get != objects_.end()
        ? get->second.object_;
        : Ptr<T>();
}

template <class T>
inline AccessSpecifier GetAccessSpecifier (const std::string& name) const {
    auto get = objects_.find(name);
    return get != objects_.end()
        ? get->second.access_;
        : AccessSpecifier::PRIVATE;
}

template <class T>
inline bool HasName(const std::string& name) const {
    return objects_.count(name) == 1;
}


} // namespace md
} // namespace opwig

#endif // OPWIG_MD_CONTAINER_H_


