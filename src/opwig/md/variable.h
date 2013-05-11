
#ifndef OPWIG_MD_VARIABLE_H_
#define OPWIG_MD_VARIABLE_H_

#include <opwig/md/metadataobject.h>
#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace md {

/// Metadata class for C++ variables.
class Variable : public MetadataObject {

  public:
    /// Creates a new Variable object. Must be used in place of the
    /// constructor.
    static Ptr<Variable> Create (const std::string& name, const std::string& type);

    const std::string& type() const { return type_; }

  private:

    std::string type_;

    Variable (const std::string& name, const std::string& type) : MetadataObject(name), type_(type) { }

};

inline Ptr<Variable> Variable::Create (const std::string& name, const std::string& type) {
  return Ptr<Variable>(new Variable(name, type) );
}


} // namespace md
} // namespace opwig

#endif // OPWIG_MD_VARIABLE_H_
