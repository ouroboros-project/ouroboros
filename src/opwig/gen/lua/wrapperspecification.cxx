
#include <opwig/gen/lua/wrapperspecification.h>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using md::Ptr;

Ptr<ConverterProvider> WrapperSpecification::GetConverterProvider () const {
    return Ptr<ConverterProvider>();
}

string WrapperSpecification::FileHeader () const {
    return "";
}

string WrapperSpecification::FinishFile () const {
    return "";
}

string WrapperSpecification::WrapFunction(const md::Ptr<const md::Function>& obj) {
    return "";
}

string WrapperSpecification::WrapVariable(const md::Ptr<const md::Variable>& obj) {
    return "";
}

string WrapperSpecification::WrapClass(const md::Ptr<const md::Class>& obj) {
    return "";
}

string WrapperSpecification::WrapNamespace(const md::Ptr<const md::Namespace>& obj) {
    return "";
}

string WrapperSpecification::WrapEnum(const md::Ptr<const md::Enum>& obj) {
    return "";
}

} // namespace lua
} // namespace gen
} // namespace opwig

