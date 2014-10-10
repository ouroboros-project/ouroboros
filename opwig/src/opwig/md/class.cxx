#include <opwig/md/class.h>
#include <opwig/md/function.h>
#include <opwig/md/type.h>

namespace opwig {
namespace md {

bool Class::AddNestedFunction (Ptr<Function> nested) {
    if (nested->name() == name_ && !nested->return_type()) {
        constructors_.push_back(nested);
        nested->set_parent( shared_from_this() );
        return true;
    }
    else if (nested->name() == ("~"+name_) && !nested->return_type()) {
        if (static_cast<bool>(destructor_))
            throw SemanticError("Classes may have only one destructor!", __FILE__, __LINE__);
        destructor_ = nested;
        nested->set_parent( shared_from_this() );
        return true;
    }
    return Scope::AddNestedFunction(nested);
}

bool Class::AddNestedClass(Ptr<Class> nested) {
    if (nested->name() == name_) {
        throw SemanticError("A nested class can't have the same name as the parent class.");
    }
    return Scope::AddNestedClass(nested);
}

}
}
