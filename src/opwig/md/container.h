
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

    typedef std::map<std::string, Ptr<T>> Table;

    class Iterable {
      public:
        typename Table::const_iterator begin() const { return objects_.begin(); }
        typename Table::const_iterator end() const { return objects_.end(); }
      private:
        friend class Container;
        const Table& objects_;
        Iterable(const Table& the_objects)
            : objects_(the_objects) {}
    };

    Container() : current_access_(AccessSpecifier::PRIVATE) {}
    
    /// Virtual destructor.
    virtual ~Container () {}

    /// Tells how many metadata objects there are in this container.
    size_t Num () const;

    /// Adds a new metadata object with the current access specifier.
    bool Add (Ptr<T> mdObj);

    /// Gives the metadata object identified by the given name (const version).
    Ptr<const T> Get (const std::string& id) const;
    
    /// Gives the metadata object identified by the given name.
    Ptr<T> Get (const std::string& id);

    /// Returns STL-iterable interface.
    Iterable Iterate () const { return Iterable(objects_); }

    /// Gets the current access specifier for this container.
    AccessSpecifier GetCurrentAccessSpecifier () const { return current_access_; }
    
    /// Sets the current access specifier for this container.
    void SetCurrentAccessSpecifier (AccessSpecifier current_access) {
        current_access_ = current_access;
    }
    
    /// Checks if the given id exists within this container.
    bool HasID(const std::string& id) const;

    /// Checks if the given id exists within this container.
    bool HasName(const std::string& name) const;

  protected:
  
    AccessSpecifier current_access_;
<<<<<<< HEAD
    std::map<std::string, Ptr<T>> objects_;
    std::map<std::string, std::string> name_to_ids_;
=======
    Table           objects_;
    
>>>>>>> [opwig] Added Scpoe::IterateClasses().
};

template <class T>
inline size_t Container<T>::Num () const {
    return objects_.size();
}

template <class T>
inline bool Container<T>::Add (Ptr<T> mdObj) {
    std::string id = mdObj->id();
    auto check = objects_.find(id);
    if (check != objects_.end())
        return false;
    mdObj->set_access(current_access_);
    objects_[id] = mdObj;
    name_to_ids_[mdObj->name()] = id;
    return true;
}

template <class T>
inline Ptr<const T> Container<T>::Get (const std::string& id) const {
    auto get = objects_.find(id);
    return get != objects_.end() 
        ? get->second 
        : Ptr<const T>();
}

template <class T>
inline Ptr<T> Container<T>::Get (const std::string& id) {
    auto get = objects_.find(id);
    return get != objects_.end()
        ? get->second
        : Ptr<T>();
}

template <class T>
inline bool Container<T>::HasID(const std::string& id) const {
    return objects_.count(id) == 1;
}

template <class T>
inline bool Container<T>::HasName(const std::string& name) const {
    return name_to_ids_.count(name) == 1;
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_CONTAINER_H_


