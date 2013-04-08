
#ifndef OPWIG_MD_PARAMETER_H_
#define OPWIG_MD_PARAMETER_H_

#include <vector>

namespace opwig {
namespace md {

/// Function parameter information.
struct Parameter final {
    std::string type;
    std::string name;
    bool operator ==(const Parameter& rhs) const;
};

/// List of function parameter information.
using ParameterList = std::vector<Parameter>;

inline bool Parameter::operator ==(const Parameter& rhs) const {
  return type == rhs.type && name == rhs.name;
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_PARAMETER_H_

