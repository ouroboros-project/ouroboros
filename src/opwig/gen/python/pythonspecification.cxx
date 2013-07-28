#include <opwig/gen/python/pythonspecification.h>
#include <opwig/md/function.h>

namespace opwig {
namespace gen {
namespace python {

using std::string;
using md::Ptr;


// HEADER BLOCK
string FileHeader() const {
}

// FINISH BLOCK
string FinishFile() const {
}

// WRAP FUNCION
string WrapFunction(const md::Ptr<const md::Function>& obj) {

}

// WRAP VARIABLE
string WrapVariable(const md::Ptr<const md::Variable>& obj) {
    return "";
}

// WRAP CLASS
string WrapClass(const md::Ptr<const md::Class>& obj) {
    return "";
}

// WRAP NAMESPACE
string WrapNamespace(const md::Ptr<const md::Namespace>& obj) {
    return "";
}

// WRAP ENUM
string WrapEnum(const md::Ptr<const md::Enum>& obj) {
    return "";
}

} // namespace python
} // namespace gen
} // namespace opwig
