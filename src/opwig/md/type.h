
#ifndef OPWIG_MD_TYPE_H_
#define OPWIG_MD_TYPE_H_

#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace md {

/// Class representing a C++ type in the metadatas.
class Type : public std::enable_shared_from_this<Type> {

  public:
    static Ptr<Type> Create (const std::string& base, bool is_const);
    static Ptr<Type> ChainTypes(const Ptr<Type>& base, const Ptr<Type>& pointer);

    const std::string& base() const { return base_; }
    bool is_const() const { return const_; }

    void set_pointee(const Ptr<Type>& pointee) { pointee_ = pointee; }
    Ptr<Type> pointee() const { return pointee_; }

    bool operator ==(const Type& rhs) const;
    bool operator !=(const Type& rhs) const;

    std::string full_type() const;

    Ptr<const Type> actual_base_type() const;

  private:
    std::string base_;
    bool const_;
    
    Ptr<Type> pointee_;

    Type (const std::string& base, bool is_const) : base_(base), const_(is_const) { }

};

inline Ptr<Type> Type::Create (const std::string& base, bool is_const) {
  return Ptr<Type>(new Type(base, is_const) );
}

inline Ptr<Type> Type::ChainTypes(const Ptr<Type>& base, const Ptr<Type>& pointer) {
    if (pointer) {
        pointer->set_pointee(base);
        return pointer;
    }
    return base;
}

inline bool Type::operator ==(const Type& rhs) const {
    return full_type() == rhs.full_type();
}

inline bool Type::operator !=(const Type& rhs) const {
    return full_type() != rhs.full_type();
}

inline std::string Type::full_type() const {
    if (pointee_)
        return pointee_->full_type() + " " + base_ + ((const_) ? " const" : "");
    return ((const_) ? "const " : "") + base_;
}

inline Ptr<const Type> Type::actual_base_type() const {
    if (pointee_)
        return pointee_->actual_base_type();
    return shared_from_this();
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_TYPE_H_

