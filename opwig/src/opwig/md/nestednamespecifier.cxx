#include <opwig/md/nestednamespecifier.h>
#include <opwig/md/scope.h>
#include <opwig/md/namespace.h>
#include <opwig/md/class.h>
#include <opwig/md/semanticerror.h>

namespace opwig {
namespace md {

void NestedNameSpecifier::AddPath(const std::string& path) {
    paths_.push_back(path);
}

Ptr<Scope> NestedNameSpecifier::FindNearestNestingScope(Ptr<Scope> initial_scope) const {
    Ptr<Scope> scope = initial_scope;
    
    if (global_) {
        while (scope->parent()) {
            scope = scope->parent();
        }
    }

    for (const std::string& it : paths_) {
        if (scope->NestedNamespace(it))
            scope = scope->NestedNamespace(it);
        else if (scope->NestedClass(it))
            scope = scope->NestedClass(it);
        else
            throw SemanticError(
                "Invalid path '"+(it)+"' in NestedNameSpecifier("+ToString()+").",
                __FILE__, __LINE__
            );
    }
    return scope;
}

std::string NestedNameSpecifier::ToString() const {
    std::string result = "";
    if (global_) result += "::";
    
    for (const std::string& it : paths_)
        result += it + "::";
    return result + name_;
}

} // namespace md
} // namespace opwig

