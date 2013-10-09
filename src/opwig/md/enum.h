
#ifndef OPWIG_MD_ENUM_H_
#define OPWIG_MD_ENUM_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>

#include <vector>
#include <string>

namespace opwig {
namespace md {

/// Metadata class for C++ ENUMs.
class Enum : public MetadataObject {
  
  public:

    /// Virtual destructor.
    virtual ~Enum() {}

    /// Creates a new Enum instance.
    /** Creates a new Enum instance with given attributes, and returns a smart pointer to it.
    * @param the_name The name of the enumeration.
    * @param the_base Name of the base (base type specifier) of this enumeration, if any.
    * @param the_values Vector of names of the enumeration values.
    * @return A smart pointer containing a new Enum instance.
    */
    static Ptr<Enum> Create(const std::string& the_name, const std::string& the_base, const std::vector<std::string>& the_values);

    /// Gets the base (base type specifier) of the enum.
    /** @return The name of base of this enum. */
    const std::string base() const { return base_; }
    
    /// Gets the list of values of this enum.
    /** Gets the list of names of the values of this enumeration.
    * @return List of names of the values. */
    const std::vector<std::string> values() const { return values_; }

  private:
    std::string base_;
    std::vector<std::string> values_;
   
    Enum(const std::string& the_name, const std::string& the_base, const std::vector<std::string>& the_values);

};

inline Enum::Enum(const std::string& the_name, const std::string& the_base, const std::vector<std::string>& the_values)
    : MetadataObject(the_name), base_(the_base), values_(the_values) { }

inline Ptr<Enum> Enum::Create(const std::string& the_name, const std::string& the_base, const std::vector<std::string>& the_values) {
    return Ptr<Enum>(new Enum(the_name, the_base, the_values));
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_ENUM_H_

