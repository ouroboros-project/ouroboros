
#ifndef OPWIG_MD_ENUM_H_
#define OPWIG_MD_ENUM_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>

#include <vector>
#include <string>

namespace opwig {
namespace md {

/// Metadata interface for C++ ENUMs.
class Enum : public MetadataObject {
  
  public:

    /// Virtual festructor.
    virtual ~Enum() {}

    /// Creates a new Function object. Must be used in place of the constructor.
    static Ptr<Enum> Create(const std::string& the_name, const std::vector<std::string>& the_bases, const std::vector<std::string>& the_values);

    /// Gets the list of bases (base type specifier) of the enum.
    const std::vector<std::string> bases() const { return bases_; }
    
    /// Gets the list of values of this enum.
    const std::vector<std::string> values() const { return values_; }

  private:
    std::vector<std::string> bases_;
    std::vector<std::string> values_;
   
    Enum(const std::string& the_name, const std::vector<std::string>& the_bases, const std::vector<std::string>& the_values);

};

inline Enum::Enum(const std::string& the_name, const std::vector<std::string>& the_bases, const std::vector<std::string>& the_values)
    : MetadataObject(the_name), bases_(the_bases), values_(the_values) { }

inline Ptr<Enum> Enum::Create(const std::string& the_name, const std::vector<std::string>& the_bases, const std::vector<std::string>& the_values) {
    return Ptr<Enum>(new Enum(the_name, the_bases, the_values));
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_ENUM_H_

