
#ifndef OPWIG_JSON_EXCEPTIONS_H_
#define OPWIG_JSON_EXCEPTIONS_H_

#include <string>
#include <exception>

namespace opwig {
namespace json {

class MissingScope : public std::exception {
public:
    MissingScope(const std::string& scope_name, const std::string& full_name)
     : reason_("Missing scope '" + scope_name +
               "' for qualified name: '" + full_name + "'.") {}
               
    const char* what() const override {
        return reason_.c_str();
    }
private:
    std::string reason_;
};

} // namespace json
} // namespace opwig

#endif // OPWIG_JSON_EXCEPTIONS_H_


