
#ifndef OPWIG_MD_TYPE_H_
#define OPWIG_MD_TYPE_H_

#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace md {

/// Class representing a C++ type in the metadatas.
/** This class represents a C++ type.
    While some of the metadata classes (such as opwig::md::Class, or opwig::md::Enum)
    represent C++ types themselves, this class is a different interface to represent
    the <i>C++ type of a metadata object or element</i>, such as the return type of
    a function, or the type of a variable.

    Since types (again, like the type of a function parameter) may be simply the type,
    a pointer or reference to the type or other combinations of these, this class
    can represent such cases by containing a chain of opwig::md::Type (kinda like a
    linked-list of Type).

    So a Type can be simply a type (which we call "the base type" in this context),
    or a pointer/reference to another Type (thus creating the chain).
    A Type instance that is a pointer (<i>*</i>) or reference (<i>&</i>) to another Type
    contains the appropriate symbol as its base string value, and a actual smart pointer
    to another Type instance.
*/
class Type : public std::enable_shared_from_this<Type> {

  public:
    /// Creates a new 'base' type instance.
    /** Creates a new 'base' type instance with given values.
    * @param base Name of the 'base' type.
    * @param is_const Indicates if it is const or not.
    * @return A smart pointer containing a new Type instance.
    */
    static Ptr<Type> Create (const std::string& base, bool is_const);
    /// Links two Types together, forming a chain.
    /** @param base The 'base' Type.
    * @param pointer The type that points to (or references) the 'base' type.
    * @return If pointer was null, simply returns the base Type. Else it makes the pointer
        Type contain the base Type, and returns the pointer Type. In other words, returns
        whichever of the received Types that comes first in the chain, if there is one.
    */
    static Ptr<Type> ChainTypes(const Ptr<Type>& base, const Ptr<Type>& pointer);

    /// Gets the string representation of the value of this type.
    /** Gets the string representation of the value of this type, without
        consideration for the chain, if there is one.
        For actual 'base' Types, this will be the name of the type (such as 'int', 'std::string'
        or the name of a class).
        For Types that point to or reference another Type, it will be '*' or '&', respectively.
    * @return The string representation of the value of this type, without consideration
        for the chain if there is one.
    */
    const std::string& base() const { return base_; }
    /// If this Type was declared as const or not.
    bool is_const() const { return const_; }

    /// Sets the Type to which this Type points to (or references).
    void set_pointee(const Ptr<Type>& pointee) { pointee_ = pointee; }
    /// Gets the Type that this Type points to (or references).
    Ptr<Type> pointee() const { return pointee_; }

    bool operator ==(const Type& rhs) const;
    bool operator !=(const Type& rhs) const;

    /// Gets a string with the full type.
    /** Gets a string containing the definition of the actual type, represented
        by the entire chain (from this Type to the next, to the next, and so on).
    * @return The actual full type string representation.
    */
    std::string full_type() const;

    /// Gets the actual 'base' type of this type chain.
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

