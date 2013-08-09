
#ifndef OPWIG_MD_PARAMETER_H_
#define OPWIG_MD_PARAMETER_H_

#include <opwig/md/type.h>
#include <vector>
#include <initializer_list>

namespace opwig {
namespace md {

/// Function parameter information.
struct Parameter final {
    Ptr<Type> type;
    std::string name;
    Parameter ();
    Parameter (const Ptr<Type>& the_type, const std::string& the_name = "");
    bool operator ==(const Parameter& rhs) const;
};

/// List of function parameter information.
using ParameterList = std::vector<Parameter>;

inline Parameter::Parameter () : name("") {}

inline Parameter::Parameter (const Ptr<Type>& the_type, const std::string& the_name)
    : type(the_type), name(the_name) {}

inline bool Parameter::operator ==(const Parameter& rhs) const {
    return type == rhs.type;
}

} // namespace md
} // namespace opwig

#endif // OPWIG_MD_PARAMETER_H_

