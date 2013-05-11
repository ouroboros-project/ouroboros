#include <opwig/md/class.h>
#include <opwig/md/function.h>

namespace opwig {
namespace md {

bool Class::AddNestedFunction (Ptr<Function> nested) {
    if (nested->name() == name_ && nested->return_type() == "") {
        nested->set_access(functions_.GetCurrentAccessSpecifier());
        constructors_.push_back(nested);
        return true;
    }
    else if (nested->name() == ("~"+name_) && nested->return_type() == "") {
        if (static_cast<bool>(destructor_))
            throw SemanticError("Classes may have only one destructor!", __FILE__, __LINE__);
        nested->set_access(functions_.GetCurrentAccessSpecifier());
        destructor_ = nested;
        return true;
    }
    return Scope::AddNestedFunction(nested);
}

}
}
